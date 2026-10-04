"""Finds the functions of an XBE's code: where each starts and ends, what it
calls, and the jump tables inside it.

Starting points are the entry point, direct call targets, seeds (e.g. atlas
names), and pointers into the code from data and from code immediates that
land on a function boundary. Each is disassembled recursively. A switch's
case labels and jump tables belong to the function whose jump reads them.
Code that nothing reaches is picked up from the gaps between functions, and
again after each function is cut at the next one's start. A guessed start
(a pointer, an immediate, a gap) that lies inside an instruction another
trace decoded is dropped. A code address pushed as the same argument of the
same callee as a known function is a function too.

    python tools/functions.py <default.xbe>      print a summary
"""
import bisect
import struct
import sys
from dataclasses import dataclass, field

from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone import x86

from xbe import Section, load

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
    code: dict = field(default_factory=dict, repr=False, compare=False)  # instruction va -> size, as traced


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

    def prologue(self, va):
        """Whether va starts a frame: push ebp ; mov ebp, esp."""
        o = va - self.lo
        return self.bytes[o:o + 3] == b'\x55\x8b\xec'


def _bound_check(code, block):
    """What the code before a switch jump says about its tables: (bound,
    index) where bound is the entry count from the bound check `cmp r, imm`
    whose flags go to a `ja` (None without one), and index is the displacement
    of a `movzx r, byte ptr [r + table]` from the code after that check (None
    without one: a one-level switch)."""
    bound, index = 0, None
    for k in range(len(block) - 1, -1, -1):
        ins = block[k]
        if (index is None and ins.mnemonic == 'movzx' and ins.operands[1].type == x86.X86_OP_MEM
                and ins.operands[1].size == 1 and code.inside(ins.operands[1].mem.disp)):
            index = ins.operands[1].mem.disp
        if ins.mnemonic == 'cmp' and ins.operands[1].type == x86.X86_OP_IMM:
            reader = next((i for i in block[k + 1:] if i.mnemonic.startswith(('j', 'set', 'cmov'))), None)
            if reader is not None and reader.mnemonic == 'ja':
                bound = ins.operands[1].imm + 1
            break
    # Retail-specific: a cmp against -1 gives a bound of 0.
    return (bound if 0 < bound <= MAX_TABLE else None), index


def _entries(code, table, valid, limit=MAX_TABLE):
    """How many dwords from table, at most limit, are case labels (valid) or
    null. Retail-specific: switches without a bound check can have null
    entries, for index values the code never takes."""
    n = 0
    while n < limit:
        t = code.dword(table + 4 * n)
        if t is None or (t != 0 and (not valid(t) or table <= t < table + 4 * (n + 1))):
            break
        n += 1
    return n


def _index_table(code, va, n, bound):
    """The byte index table of a two-level switch with n case labels, at va:
    its length, or None if the bytes there are not one. With a bound the
    length is known; without one the table runs while its bytes index the n
    labels. Every label is used, so the largest index is n - 1."""
    if bound:
        indices = [code.byte(va + k) for k in range(bound)]
        if None in indices:
            return None
    else:
        indices = []
        while len(indices) < MAX_TABLE and (b := code.byte(va + len(indices))) is not None and b < n:
            indices.append(b)
    return len(indices) if indices and max(indices) == n - 1 else None


def switch_tables(code, block, jump, valid):
    """The tables of the switch jump `jmp [r*4 + table]` that ends block, as
    [(va, 4, labels), (va, 1, indices)] (the second only for a two-level
    switch), or [] if they do not read as a switch. valid(t) says whether t can
    be a case label of the function.

    MSVC 7.1 places a switch's tables after the function's code: the label
    table, then a two-level switch's byte index table right after it. A
    switch over every value of its index (no default) has no bound check,
    and the code may fold the lowest case value into the displacements, so
    the tables' extents come from their contents."""
    table = jump.operands[0].mem.disp
    bound, index = _bound_check(code, block)
    labels = _entries(code, table, valid)
    if index is not None:
        # the index table follows the label table: the longest label table
        # whose index table fits, checked with the bound first
        for b in (bound, None):
            for n in range(labels, 0, -1):
                m = _index_table(code, table + 4 * n, n, b)
                if m and index < table + 4 * n + m:
                    return [(table, 4, n), (table + 4 * n, 1, m)]
    elif bound and _entries(code, table, valid, bound) == bound:
        return [(table, 4, bound)]
    # no usable bound: as many labels as there are
    if any(code.dword(table + 4 * k) for k in range(labels)):
        return [(table, 4, labels)]
    return []


def _trace(code, start, starts):
    """Recursive descent from start. Returns the Function."""
    fn = Function(start, start)
    seen, work = fn.code, [start]
    padded = set()  # tail-jump targets taken for functions only for the int3 before them

    def near(va):
        return code.inside(va) and 0 <= va - start <= MAX_BODY

    def tail(target):
        """Whether a jmp to target leaves the function: 'padded' when only the
        byte before target (0xCC) says so."""
        if target == start:
            return False
        if target in starts:
            return True
        if not code.inside(target):
            return False
        if target < start or target - start > MAX_BODY or code.prologue(target):
            return True
        return 'padded' if code.byte(target - 1) == 0xCC else False

    def own_byte(va):
        """Whether this trace decoded va as part of an instruction other than int3."""
        return (any(seen.get(a, 0) > va - a for a in range(va - 15, va + 1))
                and not (seen.get(va) == 1 and code.byte(va) == 0xCC))
    while work:
        _descend(code, start, fn, seen, work, near, tail, padded)
        # 0xCC also occurs inside instructions ([ebp - 0x34]): a target whose
        # 0xCC this trace decoded as part of an instruction is the function's own
        for target in [t for t in padded if own_byte(t - 1)]:
            fn.tail_jumps.discard(target)
            fn.sites[:] = [s for s in fn.sites if not (s[1] == target and s[2] == 'tail')]
            padded.discard(target)
            work.append(target)
    _merge_tables(fn)
    return fn


def _descend(code, start, fn, seen, work, near, tail, padded):
    """The descent of _trace, until its work list is empty."""
    while work:
        va = work.pop()
        block = []
        while code.inside(va) and va not in seen:
            ins = code.at(va)
            if ins is None:
                break
            seen[va] = ins.size
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
                    # is another function (size-optimized code has no padding);
                    # so is a target that sets up a frame (push ebp ; mov ebp,
                    # esp), which no jump inside a function reaches.
                    leaves = tail(op.imm)
                    if leaves:
                        fn.tail_jumps.add(op.imm)
                        fn.sites.append((va, op.imm, 'tail'))
                        if leaves == 'padded':
                            padded.add(op.imm)
                    elif code.inside(op.imm):
                        work.append(op.imm)
                elif (op.type == x86.X86_OP_MEM and op.mem.base == 0 and op.mem.index != 0
                      and op.mem.scale == 4 and code.inside(op.mem.disp)):
                    # a switch: its labels are this function's code and its
                    # tables this function's data (an entry outside the
                    # function is not one of its labels)
                    for table, width, count in switch_tables(code, block, ins, near):
                        fn.tables.append((table, width, count))
                        fn.end = max(fn.end, table + width * count)
                        if width == 4:
                            work.extend(t for k in range(count) if (t := code.dword(table + 4 * k)))
                break
            elif m.startswith('j') or m.startswith('loop'):
                if op.type == x86.X86_OP_IMM and near(op.imm):
                    work.append(op.imm)
            elif m in ('ret', 'int3', 'hlt'):
                break
            va += ins.size


def _merge_tables(fn):
    """A label table read without a bound runs on into the next switch's."""
    label_tables = sorted({t for t, width, _ in fn.tables if width == 4})
    tables = set()
    for table, width, count in fn.tables:
        if width == 4 and (i := bisect.bisect_right(label_tables, table)) < len(label_tables):
            count = min(count, max(1, (label_tables[i] - table) // 4))
        tables.add((table, width, count))
    fn.tables = sorted(tables)


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
    """Returns (direct call targets, code immediates (push/mov) at boundaries,
    {push site: code address it pushes})."""
    calls, immediates, pushes = set(), set(), {}
    lite = Cs(CS_ARCH_X86, CS_MODE_32)
    lite.skipdata = True
    for va, _, mnemonic, op_str in lite.disasm_lite(code.bytes, code.lo):
        if mnemonic == 'call' and op_str.startswith('0x'):
            v = int(op_str, 16)
            if code.inside(v):
                calls.add(v)
        elif mnemonic in ('push', 'mov') and op_str.split(', ')[-1].startswith('0x'):
            v = int(op_str.split(', ')[-1], 16)
            if code.inside(v) and code.boundary(v) and v % 16 == 0:
                immediates.add(v)
            if mnemonic == 'push' and code.inside(v):
                pushes[va] = v
    return calls, immediates, pushes


MAX_ARGUMENT_WALK = 24  # instructions from a push to the call that takes it


def _argument_slot(code, site):
    """(callee, n) when the push at site is argument n (0 is the first) of the
    direct call it feeds: the pushes between them, following unconditional
    jumps and falling through conditional ones. None when the stack is touched
    otherwise first, or no direct call comes."""
    va, n = site + code.at(site).size, 0
    for _ in range(MAX_ARGUMENT_WALK):
        ins = code.at(va) if code.inside(va) else None
        if ins is None:
            return None
        m = ins.mnemonic
        op = ins.operands[0] if ins.operands else None
        if m == 'push':
            n += 1
        elif m == 'call':
            return (op.imm, n) if op.type == x86.X86_OP_IMM else None
        elif m == 'jmp':
            if op.type != x86.X86_OP_IMM:
                return None
            va = op.imm
            continue
        elif (m in ('pop', 'ret', 'leave', 'int3', 'hlt') or m.startswith(('push', 'pop'))
              or (ins.operands and op.type == x86.X86_OP_REG and op.reg == x86.X86_REG_ESP)):
            return None
        va += ins.size
    return None


def _argument_starts(code, pushes, slots, functions):
    """Code addresses pushed as an argument of a call that, at the same
    argument, also takes a known function start (a callback slot), where the
    traces decoded an instruction. slots caches _argument_slot per site."""
    by_slot = {}
    for site, v in pushes.items():
        if site not in slots:
            slots[site] = _argument_slot(code, site)
        if slots[site] is not None:
            by_slot.setdefault(slots[site], set()).add(v)
    decoded = _decoded(functions, {v for vs in by_slot.values() for v in vs} - functions.keys())
    return {v for vs in by_slot.values() if vs & functions.keys() for v in vs & decoded}


def _covering(functions, va):
    """The functions whose traced extent holds va, other than one starting there
    (functions is sorted by start)."""
    starts = list(functions)
    i = bisect.bisect_left(starts, va)
    out = []
    while i > 0 and starts[i - 1] > va - 4 * MAX_BODY:
        i -= 1
        if functions[starts[i]].end > va:
            out.append(functions[starts[i]])
    return out


def _decoded(functions, addresses):
    """The addresses where some trace decoded an instruction."""
    ordered = dict(sorted(functions.items()))
    return {a for a in addresses if any(a in f.code for f in _covering(ordered, a))}


def _straddled(functions, addresses):
    """The addresses that lie inside an instruction another trace decoded."""
    ordered = dict(sorted(functions.items()))
    return {a for a in addresses
            if any(f.code.get(b, 0) > a - b for f in _covering(ordered, a) for b in range(a - 15, a))}


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


def _trace_all(code, queue, starts, functions, rejected=frozenset()):
    """Traces the queued starts, and then the tail-jump targets they turn up
    (none of the rejected ones). Returns the functions traced."""
    traced = []
    while queue:
        starts |= queue
        for start in sorted(queue):
            functions[start] = _trace(code, start, starts)
            traced.append(functions[start])
        queue = {t for f in traced for t in f.tail_jumps if code.inside(t)} - functions.keys() - rejected
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
    calls, immediates, pushes = _sweep_starts(code)
    # padding is not code: no start may begin at filler, except the entry and the seeds
    strong = {s for s in {image.entry} | set(seeds) if code.inside(s)} | _real(code, calls)
    weak = _real(code, immediates | _pointer_starts(image, code))
    starts, functions = set(), {}
    _trace_all(code, set(strong), starts, functions)
    weak = sorted(weak - functions.keys())
    inside_strong = _inside_any(functions, weak)
    live, groups = [], {}  # guessed start -> the starts its tracing turned up
    rejected = set()  # guesses found not to be code, never guessed again

    def guess(start):
        traced = _trace_all(code, {start}, starts, functions, rejected)
        groups[start] = [f.start for f in traced]
        return traced
    for w in weak:
        # drop weak starts inside any function accepted so far, strong or weak
        live = [f for f in live if f.end > w]
        if w in inside_strong or w in functions or any(f.start < w for f in live):
            continue
        live += guess(w)
    gap_memo = {}

    def fill_gaps():
        while pending := _gaps(code, functions, gap_memo) - functions.keys() - rejected:
            for start in sorted(pending - functions.keys()):
                guess(start)
    fill_gaps()
    slots = {}
    while True:
        # a guessed start (weak, or a gap's) inside an instruction that another
        # trace decoded is not code: data that happens to point there (retail
        # 0x232400), or the end of such a guess. Drop it and what its tracing
        # turned up, and let the gaps take the rest.
        bogus = _straddled(functions, groups.keys() & functions.keys())
        rejected |= bogus
        for w in bogus:
            for s in groups.pop(w):
                functions.pop(s, None)
                starts.discard(s)
        # a function only ever reached through a pointer passed as an argument,
        # with no alignment or padding to show (retail 0x236973, 0x236989)
        found = _argument_starts(code, pushes, slots, functions) - functions.keys() - rejected
        if not bogus and not found:
            break
        for start in sorted(found - functions.keys()):
            guess(start)
        fill_gaps()
    functions = dict(sorted(functions.items()))
    _clamp(functions)
    # a jump that a trace followed past the next function's start reaches code
    # that clamping leaves to no function: another function's entry, jumped to
    # from elsewhere (the dispatcher at retail 0x2382d6 jumps to 0x238acf)
    while pending := _gaps(code, functions, gap_memo) - functions.keys():
        _trace_all(code, pending, starts, functions)
        functions = dict(sorted(functions.items()))
        _clamp(functions)
    return {s: f for s, f in functions.items() if f.end > f.start}


def main():
    if len(sys.argv) != 2:
        sys.exit('usage: python tools/functions.py <default.xbe>')
    found = discover(load(sys.argv[1]))
    total = sum(f.end - f.start for f in found.values())
    tables = sum(len(f.tables) for f in found.values())
    print(f'{len(found)} functions, {total} bytes, {tables} jump tables')


if __name__ == '__main__':
    main()
