"""Source permuter: rewrites the body of a function marked "// @retail 0x<va>"
many ways, rebuilds, and keeps the variant with the fewest differing
instructions (0 = MATCH).

It works on a copy of src/, include/ and config/ in a scratch build root, so
the repository is never modified unless --write is given, and then only with a
strictly better variant of the one function.

    python tools/permute.py <va> [--tries N] [--seed S] [--time-limit SECONDS] [--write]
"""
import argparse
import difflib
import os
import random
import re
import shutil
import sys
import tempfile
import time
from dataclasses import dataclass

import build
import check
from inventory import read_rows
from linkmap import LinkMap
from pe import Pe
from xbe import FUNCTIONS_CSV, ROOT, Xbe, retail_xbe_path

WORST = 10 ** 6
INT_TYPES = ['long', 'int', 'short', 'unsigned long', 'dword']
CHAR_TYPES = ['char', 'byte']
NOT_NAMES = {'long', 'int', 'short', 'unsigned', 'char', 'byte', 'dword', 'word', 'const', 'void', 'static', 'sizeof',
             'float', 'real', 'double', 'struct', 'return'}
STATEMENT_STOP = ('for', 'while', 'if', 'else', 'do', 'return', 'break', 'continue', 'goto', 'switch', 'case')


# ---------------------------------------------------------------- extraction

def _skip_literal(text, i):
    """If a comment, string or character literal starts at i, the index after it, else i."""
    if text.startswith('//', i):
        j = text.find('\n', i)
        return len(text) if j < 0 else j
    if text.startswith('/*', i):
        j = text.find('*/', i + 2)
        return len(text) if j < 0 else j + 2
    if text[i] in '"\'':
        q, j = text[i], i + 1
        while j < len(text) and text[j] != q:
            j += 2 if text[j] == '\\' else 1
        return j + 1
    return i


def match_brace(text, open_index):
    """The index of the '}' matching the '{' at open_index."""
    depth, i = 0, open_index
    while i < len(text):
        j = _skip_literal(text, i)
        if j != i:
            i = j
            continue
        if text[i] == '{':
            depth += 1
        elif text[i] == '}':
            depth -= 1
            if depth == 0:
                return i
        i += 1
    raise ValueError('unbalanced braces')


def find_body(text, va):
    """(start, end) such that text[start:end] is the body of the function marked
    "// @retail <va>": the text between its braces."""
    for m in re.finditer(r'^[ \t]*//[ \t]*@retail[ \t]+(0x[0-9a-fA-F]+)[ \t]*\r?$', text, re.M):
        if int(m.group(1), 16) != va:
            continue
        i = m.end()
        while i < len(text):
            j = _skip_literal(text, i)
            if j != i:
                i = j
            elif text[i] == '{':
                return i + 1, match_brace(text, i)
            else:
                i += 1
    raise ValueError(f'no function marked @retail {va:#x}')


# ----------------------------------------------------------------- mutations

OPERAND = (r'(?:\([A-Za-z_][\w \t\*]*\))?~?(?:\([^()\n]*\)|[A-Za-z_]\w*(?:(?:\.|->)[A-Za-z_]\w*)*(?:\[[^\]\n]*\])?'
           r'|\d\w*)')
COMMUTATIVE = ['+', '*', '&', '|', '^', '==', '!=']
FLIPPED = {'<': '>', '>': '<', '<=': '>=', '>=': '<='}
BINARY = re.compile(r'(?P<a>' + OPERAND + r')\s*(?P<op><=|>=|==|!=|[-+*&|^<>])\s*(?P<b>'
                    + OPERAND + r')(?=\s*[;,)])')
GUARD_BEFORE = re.compile(r'(?:^|[(=,;{\n]|\breturn)[ \t]*$')


def binary_spans(text):
    """Matches of a lone 'A op B' with simple operands, delimited so that the
    whole text between the delimiters is the expression."""
    spans = []
    for m in BINARY.finditer(text):
        if GUARD_BEFORE.search(text[:m.start('a')]):
            spans.append(m)
    return spans


def mut_swap_operands(body, rng):
    spans = [m for m in binary_spans(body) if m.group('op') in COMMUTATIVE or m.group('op') in FLIPPED]
    if not spans:
        return None
    m = rng.choice(spans)
    op = m.group('op')
    new = f"{m.group('b')} {FLIPPED.get(op, op)} {m.group('a')}"
    return body[:m.start()] + new + body[m.end('b'):]


def _names(stmt):
    return {n for n in re.findall(r'[A-Za-z_]\w*', stmt) if n not in NOT_NAMES}


def _has_call(stmt):
    return re.search(r'\b(?!sizeof\b)\w+\s*\(', stmt) is not None


def _is_simple_statement(line):
    s = line.strip()
    return (s.endswith(';') and not s.startswith('//') and '{' not in s and '}' not in s
            and not re.match(r'(?:' + '|'.join(STATEMENT_STOP) + r')\b', s))


def mut_reorder(body, rng):
    lines = body.split('\n')
    pairs = []
    for i in range(len(lines) - 1):
        a, b = lines[i], lines[i + 1]
        if not (_is_simple_statement(a) and _is_simple_statement(b)):
            continue
        if len(a) - len(a.lstrip()) != len(b) - len(b.lstrip()):
            continue
        if _names(a) & _names(b) or (_has_call(a) and _has_call(b)):
            continue
        pairs.append(i)
    if not pairs:
        return None
    i = rng.choice(pairs)
    lines[i], lines[i + 1] = lines[i + 1], lines[i]
    return '\n'.join(lines)


DECL = re.compile(r'^(?P<indent>[ \t]*)(?P<type>unsigned long|long|int|short|dword|char|byte)(?P<rest>[ \t]+\w+\s*[;=].*)$')


def mut_retype(body, rng):
    lines = body.split('\n')
    idx = [i for i, l in enumerate(lines) if DECL.match(l)]
    if not idx:
        return None
    i = rng.choice(idx)
    m = DECL.match(lines[i])
    family = CHAR_TYPES if m.group('type') in CHAR_TYPES else INT_TYPES
    choices = [t for t in family if t != m.group('type')]
    lines[i] = m.group('indent') + rng.choice(choices) + m.group('rest')
    return '\n'.join(lines)


def _paren_groups(line):
    """(start, end) of balanced parenthesized groups that look like arithmetic."""
    out, stack = [], []
    for i, ch in enumerate(line):
        if ch == '(':
            stack.append(i)
        elif ch == ')' and stack:
            s = stack.pop()
            inner = line[s + 1:i]
            before = line[:s].rstrip()
            if re.search(r'[-+*&|^/%<>]', inner) and not re.fullmatch(r'[\w \t\*]+', inner) \
                    and not (before and (before[-1].isalnum() or before[-1] == '_')):
                out.append((s, i))
    return out


def _fresh_temp(body):
    n = 0
    while re.search(r'\btmp%d\b' % n, body):
        n += 1
    return f'tmp{n}'


def mut_introduce_temp(body, rng):
    lines = body.split('\n')
    cands = []
    for i, line in enumerate(lines):
        s = line.strip()
        if not _is_simple_statement(line) and not s.startswith('return'):
            continue
        if not s.endswith(';') or s.startswith('//'):
            continue
        for a, b in _paren_groups(line):
            cands.append((i, a, b + 1, line[a + 1:b]))
        for m in binary_spans(line):
            cands.append((i, m.start(), m.end(), m.group(0)))
    if not cands:
        return None
    i, a, b, expr = rng.choice(cands)
    name = _fresh_temp(body)
    line = lines[i]
    indent = line[:len(line) - len(line.lstrip())]
    lines[i:i + 1] = [f'{indent}{rng.choice(["long", "dword", "int"])} {name} = {expr};',
                      line[:a] + name + line[b:]]
    return '\n'.join(lines)


TEMP_DECL = re.compile(r'^[ \t]*(?P<type>long|int|dword|unsigned long|[\w ]+\*)[ \t]*(?P<name>\w+)\s*=\s*(?P<expr>[^;]+);\s*$')


def mut_inline_temp(body, rng):
    lines = body.split('\n')
    cands = []
    for i, line in enumerate(lines):
        m = TEMP_DECL.match(line)
        if not m or m.group('type').split()[0] in ('return',):
            continue
        name, expr = m.group('name'), m.group('expr').strip()
        uses = [j for j, l in enumerate(lines) if j != i and re.search(r'\b%s\b' % name, l)]
        if len(uses) != 1 or uses[0] < i or len(re.findall(r'\b%s\b', body)) > 99:
            continue
        j = uses[0]
        if len(re.findall(r'\b%s\b' % name, body)) != 2 or re.search(r'\b%s\b\s*(?:[-+*&|^]?=[^=]|\+\+|--)' % name, lines[j]):
            continue
        # nothing between the two lines may touch what expr reads
        deps = _names(expr)
        if any(deps & _names(l) for l in lines[i + 1:j]) or any(_has_call(l) for l in lines[i + 1:j]):
            continue
        if re.search(r'\b%s\b' % name, lines[j].split('//')[0]) is None or lines[j].lstrip().startswith(('for', 'while')):
            continue
        cands.append((i, j, name, expr))
    if not cands:
        return None
    i, j, name, expr = rng.choice(cands)
    simple = re.fullmatch(r'[\w.\->]+|\([^()]*\)', expr) is not None
    lines[j] = re.sub(r'\b%s\b' % name, (expr if simple else f'({expr})').replace('\\', '\\\\'), lines[j])
    del lines[i]
    return '\n'.join(lines)


def _split_top(text, start):
    """Index just after the balanced '(...)' that starts at start."""
    depth = 0
    for i in range(start, len(text)):
        if text[i] == '(':
            depth += 1
        elif text[i] == ')':
            depth -= 1
            if depth == 0:
                return i + 1
    return None


def _negate(cond):
    cond = cond.strip()
    m = re.fullmatch(r'(?P<a>[^<>=!&|()]+?)\s*(?P<op>==|!=|<=|>=|<|>)\s*(?P<b>[^<>=!&|()]+)', cond)
    if m:
        inverse = {'==': '!=', '!=': '==', '<': '>=', '>': '<=', '<=': '>', '>=': '<'}[m.group('op')]
        return f"{m.group('a')} {inverse} {m.group('b')}"
    m = re.fullmatch(r'!\s*(\w+)', cond)
    if m:
        return m.group(1)
    m = re.fullmatch(r'!\s*\((.*)\)', cond)
    if m and _split_top(cond, cond.index('(')) == len(cond):
        return m.group(1)
    return f'!({cond})'


def mut_swap_if_else(body, rng):
    cands = []
    for m in re.finditer(r'\bif\s*\(', body):
        end = _split_top(body, m.end() - 1)
        if end is None:
            continue
        k = end
        while k < len(body) and body[k] in ' \t\r\n':
            k += 1
        if k >= len(body) or body[k] != '{':
            continue
        close = match_brace(body, k)
        e = re.compile(r'\s*else\s*\{').match(body, close + 1)
        if not e:
            continue
        eopen = e.end() - 1
        cands.append((m, end, k, close, eopen, match_brace(body, eopen)))
    if not cands:
        return None
    m, end, k, close, eopen, eclose = rng.choice(cands)
    cond = body[m.end():end - 1]
    then, other = body[k + 1:close], body[eopen + 1:eclose]
    gap = body[end:k]
    mid = body[close + 1:eopen]
    return (body[:m.end()] + _negate(cond) + ')' + gap + '{' + other + '}' + mid + '{' + then + '}'
            + body[eclose + 1:])


FOR = re.compile(r'^(?P<indent>[ \t]*)for\s*\((?P<init>[^;{}]*);(?P<cond>[^;{}]*);(?P<inc>[^;{}]*)\)\s*\{\s*$')


def mut_for_while(body, rng):
    lines = body.split('\n')
    cands = []
    for i, line in enumerate(lines):
        m = FOR.match(line)
        if m and m.group('cond').strip() and m.group('inc').strip():
            close = _closing_line(lines, i)
            if close is not None and not any(re.search(r'\bcontinue\b', l) for l in lines[i + 1:close]):
                cands.append(('for', i, close))
        if re.match(r'^[ \t]*while\s*\(.*\)\s*\{\s*$', line) and i > 0 and _is_simple_statement(lines[i - 1]):
            close = _closing_line(lines, i)
            if close is None or close - 1 <= i or not _is_simple_statement(lines[close - 1]):
                continue
            last = lines[close - 1].strip()
            if re.fullmatch(r'(?:\w+(?:\+\+|--)|(?:\+\+|--)\w+|\w+\s*[-+]=\s*[\w.]+);', last) \
                    and not any(re.search(r'\bcontinue\b', l) for l in lines[i + 1:close]):
                cands.append(('while', i, close))
    if not cands:
        return None
    kind, i, close = rng.choice(cands)
    if kind == 'for':
        m = FOR.match(lines[i])
        ind = m.group('indent')
        inc = m.group('inc').strip()
        lines[close:close] = [f'{ind}\t{inc};']
        lines[i:i + 1] = ([f'{ind}{m.group("init").strip()};'] if m.group('init').strip() else []) + \
                         [f'{ind}while ({m.group("cond").strip()}) {{']
        return '\n'.join(lines)
    w = re.match(r'^([ \t]*)while\s*\((.*)\)\s*\{\s*$', lines[i])
    init = lines[i - 1].strip().rstrip(';')
    inc = lines[close - 1].strip().rstrip(';')
    ind = w.group(1)
    new = [f'{ind}for ({init}; {w.group(2)}; {inc}) {{']
    lines[i - 1:close] = new + lines[i + 1:close - 1]
    return '\n'.join(lines)


def _closing_line(lines, i):
    depth = 0
    for k in range(i, len(lines)):
        depth += lines[k].count('{') - lines[k].count('}')
        if depth == 0:
            return k
    return None


COMPOUND = re.compile(r'^(?P<indent>[ \t]*)(?P<lhs>[A-Za-z_][\w.\->\[\]]*)\s*(?P<op>[-+*&|^])=\s*(?P<rhs>[^;=]+);\s*$')
EXPANDED = re.compile(r'^(?P<indent>[ \t]*)(?P<lhs>[A-Za-z_][\w.\->\[\]]*)\s*=\s*(?P=lhs)\s*(?P<op>[-+*&|^])\s*(?P<rhs>[^;=]+);\s*$')


def mut_compound(body, rng):
    lines = body.split('\n')
    cands = []
    for i, line in enumerate(lines):
        for kind, pat in (('expand', COMPOUND), ('fold', EXPANDED)):
            m = pat.match(line)
            if m and '(' not in m.group('lhs') and '++' not in line and '--' not in line:
                rhs = m.group('rhs').strip()
                if kind == 'fold' and not re.fullmatch(r'%s|\([^()]*\)' % OPERAND, rhs):
                    continue
                cands.append((kind, i, m))
    if not cands:
        return None
    kind, i, m = rng.choice(cands)
    ind, lhs, op, rhs = m.group('indent'), m.group('lhs'), m.group('op'), m.group('rhs').strip()
    if kind == 'expand':
        wrapped = rhs if re.fullmatch(OPERAND, rhs) else f'({rhs})'
        lines[i] = f'{ind}{lhs} = {lhs} {op} {wrapped};'
    else:
        if rhs.startswith('(') and rhs.endswith(')') and _split_top(rhs, 0) == len(rhs):
            rhs = rhs[1:-1]
        lines[i] = f'{ind}{lhs} {op}= {rhs};'
    return '\n'.join(lines)


STEP = re.compile(r'(?:(?<=^)|(?<=;)|(?<=\n))(?P<pre>[ \t]*)(?:(?P<a>\+\+|--)(?P<n1>[A-Za-z_]\w*)|(?P<n2>[A-Za-z_]\w*)(?P<b>\+\+|--))(?=[ \t]*[;)])')
FOR_STEP = re.compile(r'(?<=;)(?P<pre>[ \t]*)(?:(?P<a>\+\+|--)(?P<n1>[A-Za-z_]\w*)|(?P<n2>[A-Za-z_]\w*)(?P<b>\+\+|--))(?=[ \t]*\))')


def mut_increment(body, rng):
    spans = list(STEP.finditer(body)) + list(FOR_STEP.finditer(body))
    plus_one = list(re.finditer(r'(?m)^(?P<pre>[ \t]*)(?P<n>[A-Za-z_]\w*)\s*(?P<s>[-+])=\s*1;', body))
    if not spans and not plus_one:
        return None
    if plus_one and (not spans or rng.random() < 0.3):
        m = rng.choice(plus_one)
        return body[:m.start()] + f'{m.group("pre")}{m.group("n")}{m.group("s")}{m.group("s")};' + body[m.end():]
    m = rng.choice(spans)
    if m.group('a'):
        new = f'{m.group("pre")}{m.group("n1")}{m.group("a")}'
    else:
        new = f'{m.group("pre")}{m.group("b")}{m.group("n2")}'
    return body[:m.start()] + new + body[m.end():]


DECL_INIT = re.compile(r'^(?P<indent>[ \t]*)(?P<type>unsigned long|long|int|short|dword|char|byte|[\w ]+\*)[ \t]*(?P<name>\w+)\s*=\s*(?P<expr>[^;]+);\s*$')
DECL_ONLY = re.compile(r'^(?P<indent>[ \t]*)(?P<type>unsigned long|long|int|short|dword|char|byte|[\w ]+\*)[ \t]*(?P<name>\w+);\s*$')


def mut_split_decl(body, rng):
    """'T x = e;' <-> 'T x;' followed by 'x = e;'."""
    lines = body.split('\n')
    cands = []
    for i, l in enumerate(lines):
        m = DECL_INIT.match(l)
        if m and not m.group('type').strip().startswith(('return', 'else')):
            cands.append(('split', i, m))
        m = DECL_ONLY.match(l)
        if m and i + 1 < len(lines):
            n = re.match(r'^[ \t]*%s\s*=\s*([^;]+);\s*$' % m.group('name'), lines[i + 1])
            if n:
                cands.append(('merge', i, m))
    if not cands:
        return None
    kind, i, m = rng.choice(cands)
    ind, name = m.group('indent'), m.group('name')
    if kind == 'split':
        lines[i:i + 1] = [f'{ind}{m.group("type").strip()} {name};', f'{ind}{name} = {m.group("expr").strip()};']
    else:
        n = re.match(r'^[ \t]*%s\s*=\s*([^;]+);\s*$' % name, lines[i + 1])
        lines[i:i + 2] = [f'{ind}{m.group("type").strip()} {name} = {n.group(1).strip()};']
    return '\n'.join(lines)


MUTATIONS = [mut_swap_operands, mut_reorder, mut_retype, mut_introduce_temp, mut_inline_temp, mut_swap_if_else,
             mut_for_while, mut_compound, mut_increment, mut_split_decl]


def mutate(body, rng, count=None):
    """Applies 1 or 2 random mutations; None when none applies."""
    count = count or rng.choice([1, 1, 2])
    changed = False
    for _ in range(count):
        order = MUTATIONS[:]
        rng.shuffle(order)
        for mut in order:
            new = mut(body, rng)
            if new is not None and new != body:
                body, changed = new, True
                break
    return body if changed else None


# ------------------------------------------------------------------ scoring

class Scorer:
    """Builds a scratch root and scores variants of one function's source file."""

    def __init__(self, va, root, xdk=None):
        self.va, self.root, self.xdk = va, root, xdk
        retail_path = retail_xbe_path()
        if not os.path.exists(retail_path):
            raise SystemExit(f'retail XBE not found at {retail_path}')
        self.retail = Xbe(retail_path)
        self.rows = read_rows(FUNCTIONS_CSV)
        self.row = self.rows.get(va)
        if self.row is None:
            raise SystemExit(f'{va:#x} is not a function start in config/functions.csv')
        self.hi = max(s.va + s.vsize for s in self.retail.sections)
        marked = [m for m in build.marked_sources(root) if m.retail == va]
        if not marked:
            raise SystemExit(f'no @retail {va:#x} marker in src/')
        self.marked = marked[0]
        self.path = os.path.join(root, self.marked.path)

    def score_text(self, text):
        """Writes text as the source file and returns (score, first-difference text)."""
        with open(self.path, 'w', encoding='utf-8', newline='') as f:
            f.write(text)
        try:
            map_path = build.build(self.root, self.xdk)
            linkmap = LinkMap.read(map_path)
            image = Pe(os.path.join(self.root, 'build', build.EXE_NAME))
            marked = build.marked_sources(self.root)
            stubs = build.stub_sources(self.root)
            identity = check.Identity(linkmap, self.rows, marked + stubs)
            m = next(m for m in marked if m.retail == self.va)
            symbol = check.resolve(linkmap, m)
            start, end = linkmap.extent(symbol)
            theirs = self.retail.read(self.va, int(self.row['size']))
            absolute, relative = sorted(image.fixups), sorted(linkmap.rel_fixups)
            window = (start - 3, end)
            status, first, lines, length = check.check_function(
                image.read(start, end - start), start, theirs, self.va,
                check._within(absolute, *window), _within_all(relative, window), self.retail.base, self.hi, identity)
        except (SystemExit, StopIteration, ValueError, OSError, KeyError):
            return WORST, 'build failed'
        if first is None:
            return 0, 'MATCH'
        wrong = [l for l in lines if l.startswith('  !!')]
        score = max(len(wrong), 1)
        if len(wrong) > 0 and length != len(theirs):
            score += abs(length - len(theirs)) // 4
        return score, wrong[0].strip() if wrong else f'difference at +{first:#x}'


def _within_all(fixups, window):
    return check._within(fixups, *window)


def prepare_scratch(scratch):
    for d in ('src', 'include', 'config', 'build'):
        s = os.path.join(ROOT, d)
        if os.path.isdir(s):
            shutil.copytree(s, os.path.join(scratch, d), copy_function=shutil.copy2,
                            ignore=shutil.ignore_patterns('*.exe', '*.map') if d == 'build' else None)


@dataclass
class Result:
    baseline: int
    best: int
    text: str
    original: str
    tries: int
    seconds: float
    note: str = ''


def run(va, tries=200, seed=0, time_limit=None, scratch=None, log=print):
    rng = random.Random(seed)
    own = scratch is None
    scratch = scratch or tempfile.mkdtemp(prefix='permute_')
    try:
        prepare_scratch(scratch)
        scorer = Scorer(va, scratch)
        with open(os.path.join(ROOT, scorer.marked.path), encoding='utf-8', newline='') as f:
            original = f.read()
        start, end = find_body(original, va)
        prefix, body, suffix = original[:start], original[start:end], original[end:]
        t0 = time.time()
        best_score, note = scorer.score_text(original)
        baseline, best_body = best_score, body
        log(f'baseline: {best_score} differing instructions ({note})')
        seen = {body: best_score}
        done = 0
        while done < tries and best_score > 0 and (time_limit is None or time.time() - t0 < time_limit):
            candidate = None
            for _ in range(50):
                candidate = mutate(best_body, rng)
                if candidate is not None and candidate not in seen:
                    break
                candidate = None
            if candidate is None:
                log('no new mutations available')
                break
            done += 1
            score, note = scorer.score_text(prefix + candidate + suffix)
            seen[candidate] = score
            if score <= best_score:
                if score < best_score:
                    log(f'try {done}: {score} ({note})')
                best_score, best_body = score, candidate
        seconds = time.time() - t0
        return Result(baseline, best_score, prefix + best_body + suffix, original, done, seconds, note)
    finally:
        if own:
            shutil.rmtree(scratch, ignore_errors=True)


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument('address', help='retail address of a function marked // @retail in src/')
    ap.add_argument('--tries', type=int, default=200)
    ap.add_argument('--seed', type=int, default=0)
    ap.add_argument('--time-limit', type=float, default=None, help='seconds')
    ap.add_argument('--write', action='store_true', help='write a strictly better variant back to the source file')
    args = ap.parse_args()
    va = check.parse_addresses([args.address]).pop()
    result = run(va, args.tries, args.seed, args.time_limit)
    rate = result.tries / result.seconds if result.seconds else 0
    print(f'{result.tries} tries in {result.seconds:.1f}s ({rate:.2f} tries/s); '
          f'best score {result.best} (baseline {result.baseline})' + (' MATCH' if result.best == 0 else ''))
    if result.best < result.baseline:
        diff = ''.join(difflib.unified_diff(result.original.splitlines(True), result.text.splitlines(True),
                                            'a/src', 'b/src'))
        print(diff)
        if args.write:
            marked = next(m for m in build.marked_sources() if m.retail == va)
            with open(os.path.join(ROOT, marked.path), 'w', encoding='utf-8', newline='') as f:
                f.write(result.text)
            print(f'wrote {marked.path}')
    sys.exit(0)


if __name__ == '__main__':
    main()
