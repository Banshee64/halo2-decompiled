"""Finds the functions of an XBE's code: where each starts and ends, what it
calls, and the jump tables inside it.

Starting points are the entry point, direct call targets, seeds (e.g. atlas
names), and pointers into the code from data and from code immediates that
land on a function boundary. Each is disassembled recursively. Code that
nothing reaches is picked up from the gaps between functions.

    python tools/functions.py <default.xbe>      print a summary
"""
import bisect
import struct
import sys
from dataclasses import dataclass, field

from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone import x86

from xbe import Section

MAX_TABLE = 512
MAX_BODY = 0x4000   # a jump forward farther than this leaves the function


@dataclass
class Function:
    start: int
    end: int
    calls: set = field(default_factory=set)
    tail_jumps: set = field(default_factory=set)
    tables: list = field(default_factory=list)
    sites: list = field(default_factory=list, repr=False, compare=False)  # (site va, target, 'call'|'tail')


def is_filler(ins):
    """Alignment filler: nop, int3, mov r, r, or lea r, [r + 0]."""
    if ins.mnemonic in ('nop', 'int3'):
        return True
    if len(ins.operands) != 2:
        return False
    a, b = ins.operands
    if ins.mnemonic == 'mov':
        return a.type == b.type == x86.X86_OP_REG and a.reg == b.reg
    if ins.mnemonic == 'lea':
        return (a.type == x86.X86_OP_REG and b.type == x86.X86_OP_MEM and b.mem.base == a.reg
                and b.mem.index == 0 and b.mem.disp == 0)
    return False


class _Code:
    def __init__(self, image, text):
        self.section = text if isinstance(text, Section) else image.section(text)
        self.bytes = image.section_bytes(self.section)
        self.lo = self.section.va
        self.hi = self.section.va + len(self.bytes)
        self.md = Cs(CS_ARCH_X86, CS_MODE_32)
        self.md.detail = True

    def inside(self, va):
        return self.lo <= va < self.hi

    def at(self, va):
        """The instruction at va, or None."""
        o = va - self.lo
        return next(self.md.disasm(self.bytes[o:o + 16], va), None)

    def dword(self, va):
        o = va - self.lo
        return struct.unpack_from('<I', self.bytes, o)[0] if 0 <= o <= len(self.bytes) - 4 else None

    def byte(self, va):
        o = va - self.lo
        return self.bytes[o] if 0 <= o < len(self.bytes) else None

    def boundary(self, va):
        """Whether va looks like a function start: the start of the code, after
        int3 padding or a ret, or aligned to 16."""
        if va == self.lo or va % 16 == 0:
            return True
        o = va - self.lo
        before = self.bytes[max(0, o - 3):o]
        return before[-1:] in (b'\xcc', b'\xc3') or (len(before) == 3 and before[0] == 0xC2)


def _table_count(code, block, jump):
    """How many entries the jump table of `jump` has, from the bound check
    before it. Returns (count, byte_table) where byte_table is (va, count) for
    MSVC's two-level switches, or None."""
    bound, byte_table = None, None
    for ins in reversed(block):
        if (ins.mnemonic == 'movzx' and ins.operands[1].type == x86.X86_OP_MEM
                and ins.operands[1].size == 1 and code.inside(ins.operands[1].mem.disp)):
            byte_table = ins.operands[1].mem.disp
        if ins.mnemonic == 'cmp' and ins.operands[1].type == x86.X86_OP_IMM:
            bound = ins.operands[1].imm + 1
            break
    # Retail-specific: a cmp against -1 gives a bound of 0, and byte-table indices
    # can read past the section; both mean this is not a real switch.
    if bound is None or not 0 < bound <= MAX_TABLE:
        return None, None
    if byte_table is not None:
        indices = [code.byte(byte_table + k) for k in range(bound)]
        if None in indices:
            return None, None
        return max(indices) + 1, (byte_table, bound)
    return bound, None


def _trace(code, start, starts):
    """Recursive descent from start. Returns the Function."""
    fn = Function(start, start)
    seen, work = set(), [start]

    def near(va):
        return code.inside(va) and 0 <= va - start <= MAX_BODY
    while work:
        va = work.pop()
        block = []
        while code.inside(va) and va not in seen:
            ins = code.at(va)
            if ins is None:
                break
            seen.add(va)
            block.append(ins)
            fn.end = max(fn.end, va + ins.size)
            m = ins.mnemonic
            op = ins.operands[0] if ins.operands else None
            if m == 'call' and op.type == x86.X86_OP_IMM:
                fn.calls.add(op.imm)
                fn.sites.append((va, op.imm, 'call'))
            elif m == 'jmp':
                if op.type == x86.X86_OP_IMM:
                    # Retail-specific: a tail call to a function nothing else
                    # references has no known start, so following it would pull
                    # that whole function into this one. int3 only occurs as
                    # padding between functions, so a target right after one,
                    # before this function, or farther than any function body
                    # is another function (size-optimized code has no padding).
                    if op.imm != start and (op.imm in starts or (
                            code.inside(op.imm) and (op.imm < start or op.imm - start > MAX_BODY
                                                   or code.byte(op.imm - 1) == 0xCC))):
                        fn.tail_jumps.add(op.imm)
                        fn.sites.append((va, op.imm, 'tail'))
                    elif code.inside(op.imm):
                        work.append(op.imm)
                elif (op.type == x86.X86_OP_MEM and op.mem.base == 0 and op.mem.index != 0
                      and op.mem.scale == 4 and code.inside(op.mem.disp)):
                    count, byte_table = _table_count(code, block, ins)
                    # a table whose entries leave the function is a misread
                    if count and all(t is not None and near(t) for t in
                                     (code.dword(op.mem.disp + 4 * k) for k in range(count))):
                        table = op.mem.disp
                        fn.tables.append((table, 4, count))
                        fn.end = max(fn.end, table + 4 * count)
                        if byte_table:
                            fn.tables.append((byte_table[0], 1, byte_table[1]))
                            fn.end = max(fn.end, byte_table[0] + byte_table[1])
                        for k in range(count):
                            work.append(code.dword(table + 4 * k))
                break
            elif m.startswith('j') or m.startswith('loop'):
                if op.type == x86.X86_OP_IMM and near(op.imm):
                    work.append(op.imm)
            elif m in ('ret', 'int3', 'hlt'):
                break
            va += ins.size
    fn.tables.sort()
    return fn


def _pointer_starts(image, code):
    """Pointers into the code from data sections, at function boundaries."""
    found = set()
    for s in image.sections:
        if s is code.section:
            continue
        data = image.section_bytes(s)
        for o in range(0, len(data) - 3, 4):
            v = struct.unpack_from('<I', data, o)[0]
            if code.inside(v) and code.boundary(v):
                found.add(v)
    return found


def _sweep_starts(code):
    """Returns (direct call targets, code immediates (push/mov) at boundaries)."""
    calls, immediates = set(), set()
    lite = Cs(CS_ARCH_X86, CS_MODE_32)
    lite.skipdata = True
    for _, _, mnemonic, op_str in lite.disasm_lite(code.bytes, code.lo):
        if mnemonic == 'call' and op_str.startswith('0x'):
            v = int(op_str, 16)
            if code.inside(v):
                calls.add(v)
        elif mnemonic in ('push', 'mov') and op_str.split(', ')[-1].startswith('0x'):
            v = int(op_str.split(', ')[-1], 16)
            if code.inside(v) and code.boundary(v) and v % 16 == 0:
                immediates.add(v)
    return calls, immediates


def _spans(functions):
    return sorted((f.start, f.end) for f in functions.values())


def _first_code(code, va, memo):
    """The first address at or after va that holds an instruction other than
    filler (stepping over undecodable bytes), or None before the section's end.
    memo caches the answer for every address stepped through."""
    path = []
    while va not in memo and va < code.hi:
        path.append(va)
        ins = code.at(va)
        if ins is None:
            va += 1
        elif is_filler(ins):
            va += ins.size
        else:
            memo[va] = va
            break
    result = memo.get(va)
    for p in path:
        memo[p] = result
    return result


def _gaps(code, functions, memo):
    """Starts of code that no function covers, after skipping filler. memo is
    shared between passes (see _first_code)."""
    found, cursor = set(), code.lo
    for start, end in _spans(functions) + [(code.hi, code.hi)]:
        first = _first_code(code, cursor, memo) if cursor < start else None
        if first is not None and first < start:
            found.add(first)
        cursor = max(cursor, end)
    return found


def _clamp(functions):
    """Cut each function at the next function's start, so extents never overlap
    (descent can run on into the next function past a call that never returns).
    Jump tables sit before the next function, so they survive. Calls and tail
    jumps whose site was cut off are dropped."""
    starts = list(functions)
    for fn, nxt in zip(functions.values(), starts[1:]):
        if fn.end > nxt:
            fn.end = nxt
            fn.calls = {t for site, t, kind in fn.sites if kind == 'call' and site < nxt}
            fn.tail_jumps = {t for site, t, kind in fn.sites if kind == 'tail' and site < nxt}
            fn.tables = [t for t in fn.tables if t[0] < nxt]


def _trace_all(code, queue, starts, functions):
    """Traces the queued starts, and then the tail-jump targets they turn up.
    Returns the functions traced."""
    traced = []
    while queue:
        starts |= queue
        for start in sorted(queue):
            functions[start] = _trace(code, start, starts)
            traced.append(functions[start])
        queue = {t for f in traced for t in f.tail_jumps if code.inside(t)} - functions.keys()
    return traced


def _inside_any(functions, addresses):
    """The addresses that lie strictly inside some function's traced extent."""
    spans = _spans(functions)
    firsts = [s for s, _ in spans]
    reach, farthest = [], 0
    for _, end in spans:
        farthest = max(farthest, end)
        reach.append(farthest)
    return {a for a in addresses
            if (i := bisect.bisect_left(firsts, a)) and reach[i - 1] > a}


def _real(code, starts):
    """The starts that hold an instruction other than filler."""
    return {s for s in starts if (ins := code.at(s)) is not None and not is_filler(ins)}


def discover(image, seeds=(), text='.text'):
    code = _Code(image, text)
    # Strong starts are certain; weak ones are guesses (a 16-aligned value in
    # data or an immediate), and are dropped when they fall inside a function
    # traced from a strong start, which is how mid-instruction bytes show up.
    calls, immediates = _sweep_starts(code)
    # padding is not code: no start may begin at filler, except the entry and the seeds
    strong = {s for s in {image.entry} | set(seeds) if code.inside(s)} | _real(code, calls)
    weak = _real(code, immediates | _pointer_starts(image, code))
    starts, functions = set(), {}
    _trace_all(code, set(strong), starts, functions)
    weak = sorted(weak - functions.keys())
    inside_strong = _inside_any(functions, weak)
    live = []
    for w in weak:
        # drop weak starts inside any function accepted so far, strong or weak
        live = [f for f in live if f.end > w]
        if w in inside_strong or w in functions or any(f.start < w for f in live):
            continue
        live += _trace_all(code, {w}, starts, functions)
    gap_memo = {}
    while pending := _gaps(code, functions, gap_memo) - functions.keys():
        _trace_all(code, pending, starts, functions)
    functions = dict(sorted(functions.items()))
    _clamp(functions)
    return {s: f for s, f in functions.items() if f.end > f.start}


def main():
    from xbe import Xbe
    found = discover(Xbe(sys.argv[1]))
    total = sum(f.end - f.start for f in found.values())
    tables = sum(len(f.tables) for f in found.values())
    print(f'{len(found)} functions, {total} bytes, {tables} jump tables')


if __name__ == '__main__':
    main()
