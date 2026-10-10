"""Source permuter: rewrites the body of a function marked "// @retail 0x<va>"
many ways, rebuilds, and keeps the variant with the fewest differing
instructions (0 = MATCH).

It works on a copy of src/, include/ and config/ in a scratch build root, so
the repository is never modified unless --write is given, and then only with a
strictly better variant of the one function.

--fast scores each variant on a reduced image instead of the whole program
(tools/build.py's build(units=...)): the function's own source, and the
sources that define its callees in retail, their callees in turn, and so on
through every decompiled callee (--depth N stops after N levels). The search
starts with a full build of the original, which adds what pins calling
conventions there: the sources that take the address of the function or of a
function it calls, the stubs that call one, and the library objects the full
image takes. The reduced image is linked with those, the ballast, and
unresolved externals allowed. A try recompiles only the function's source and
links a small part of the program. A variant that scores
better on the reduced image is scored again by a full build, and becomes the
best only if that build agrees it is better, so the best variant, the only
one --write writes, always has a full-build score. Both scores are printed.
Without --tries, --time-limit alone bounds a fast search, from the end of that
first build. --jobs N runs N fast scorers at once on one search, each in its
own scratch root with its own seed (S, S + 1, ...). A fast search links its
images, reduced and full, as .img files in build/permute/<va>/, one folder per
scratch root, never as .exe (antivirus software flags the reduced ones), and
removes them when it ends.

    python tools/permute.py <va> [--tries N] [--seed S] [--time-limit SECONDS] [--write]
                            [--fast [--jobs N] [--depth N]]
"""
import argparse
import difflib
import os
import random
import re
import shutil
import sys
import tempfile
import threading
import time
from concurrent.futures import ThreadPoolExecutor
from dataclasses import dataclass

import build
import check
from inventory import read_rows
from linkmap import LinkMap
from pe import Pe
from xbe import FUNCTIONS_CSV, ROOT, Xbe, retail_xbe_path

WORST = 10 ** 6
DEFAULT_TRIES = 200
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
    "// @retail <va>" (or "// @retail <va> standard"): the text between its braces."""
    for m in re.finditer(r'^[ \t]*//[ \t]*@retail[ \t]+(0x[0-9a-fA-F]+)(?:[ \t]+standard)?[ \t]*\r?$', text, re.M):
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


# Idiom mutations: the same behaviour spelled the way the original source may have spelled it.

LOOSER = {'&&': {'||', '?', ',', '='}, '||': {'?', ',', '='}}   # operators that bind more loosely than the key


def _top_level(text):
    """The operators of text outside brackets, strings and comments that matter for precedence:
    '&&', '||', '?', ',' and assignments (plain or compound), as (operator, index) pairs."""
    found, depth, i = [], 0, 0
    while i < len(text):
        j = _skip_literal(text, i)
        if j != i:
            i = j
            continue
        c = text[i]
        if c in '([{':
            depth += 1
        elif c in ')]}':
            depth -= 1
        elif depth == 0:
            if text.startswith(('&&', '||'), i):
                found.append((text[i:i + 2], i))
                i += 2
                continue
            if c in '?,':
                found.append((c, i))
            elif c == '=':
                if text.startswith('==', i):
                    i += 2
                    continue
                before = text[max(i - 2, 0):i]
                if before[-1:] not in ('!', '<', '>') or before in ('<<', '>>'):
                    found.append(('=', i))
        i += 1
    return found


def _split_logical(cond, op):
    """The operands of a top-level 'a op b op c' (op is '&&' or '||'), else None: also None
    when cond has a looser operator at the top level (a conditional, an assignment, a comma)."""
    ops = _top_level(cond)
    if any(o in LOOSER[op] for o, _ in ops):
        return None
    cuts = [i for o, i in ops if o == op]
    if not cuts:
        return None
    parts = [cond[a:b].strip() for a, b in zip([0] + [i + 2 for i in cuts], cuts + [len(cond)])]
    return parts if all(parts) else None


def _wrap(text, op):
    """text as an operand of op, in parentheses when it holds a looser operator."""
    return f'({text})' if any(o in LOOSER[op] for o, _ in _top_level(text)) else text


def _plain_literal(text):
    """True for a literal that every scalar type, bool included, holds unchanged."""
    return text in ('0', '1', 'true', 'false')


PLACE = r'[A-Za-z_]\w*(?:(?:\.|->)[A-Za-z_]\w*|\[\w+\])?'


def _independent(x, y):
    """True when storing to the place x cannot change where the place y is, nor the reverse."""
    if not (re.fullmatch(PLACE, x) and re.fullmatch(PLACE, y)):
        return False
    return not any(re.fullmatch(r'[A-Za-z_]\w*', p) and p in _names(q) for p, q in ((x, y), (y, x)))


def _starts_statement(lines, i, labels=True):
    """True when line i begins a statement, not the one-statement body of an if, else, for or while;
    a label before it counts as a start only with labels=True (a declaration cannot follow one)."""
    before = next((l.strip() for l in reversed(lines[:i]) if l.strip() and not l.strip().startswith('//')), ';')
    if before.endswith(':') and labels:
        return re.fullmatch(r'(?:case\b[^:]*|default|\w+)\s*:', before) is not None
    return before.endswith((';', '{', '}'))


CHAIN = re.compile(r'^(?P<indent>[ \t]*)(?P<a>[A-Za-z_][\w.\->\[\]]*)\s*=\s*(?P<b>[A-Za-z_][\w.\->\[\]]*)\s*=\s*'
                   r'(?P<v>\w+)\s*;\s*$')
PLAIN = re.compile(r'^(?P<indent>[ \t]*)(?P<a>[A-Za-z_][\w.\->\[\]]*)\s*=\s*(?P<v>\w+)\s*;\s*$')


def mut_chain_assign(body, rng):
    """'a = b = 0;' <-> 'b = 0; a = 0;': the chain stores to b first, the pair in either order.
    Only 0, 1, true and false, so that a bool or narrow b cannot change what a receives."""
    lines = body.split('\n')
    cands = []
    for i, line in enumerate(lines):
        if not _starts_statement(lines, i):
            continue
        m = CHAIN.match(line)
        if m and _plain_literal(m.group('v')) and _independent(m.group('a'), m.group('b')):
            cands.append(('split', i))
        m = PLAIN.match(line)
        if m and _plain_literal(m.group('v')) and i + 1 < len(lines):
            n = PLAIN.match(lines[i + 1])
            if n and n.group('indent') == m.group('indent') and n.group('v') == m.group('v') \
                    and _independent(m.group('a'), n.group('a')):
                cands.append(('merge', i))
    if not cands:
        return None
    kind, i = rng.choice(cands)
    if kind == 'split':
        m = CHAIN.match(lines[i])
        order = rng.choice([(m.group('b'), m.group('a')), (m.group('a'), m.group('b'))])
        lines[i:i + 1] = [f'{m.group("indent")}{x} = {m.group("v")};' for x in order]
    else:
        m, n = PLAIN.match(lines[i]), PLAIN.match(lines[i + 1])
        first, second = rng.choice([(m, n), (n, m)])
        lines[i:i + 2] = [f'{m.group("indent")}{first.group("a")} = {second.group("a")} = {m.group("v")};']
    return '\n'.join(lines)


def _if_head(line):
    """(indent, condition, rest of the line) of a line that starts with 'if (', else None."""
    m = re.match(r'^([ \t]*)if\s*\(', line)
    end = _split_top(line, m.end() - 1) if m else None
    return (m.group(1), line[m.end():end - 1], line[end:]) if end else None


def mut_bool_flag(body, rng):
    """'if (a && b) S' <-> 'bool flag0 = false; if (a) flag0 = b; if (flag0) S': a named bool gets a home."""
    lines = body.split('\n')
    cands = []
    for i, line in enumerate(lines):
        h = _if_head(line)
        if h and h[2].strip() in ('', '{') and _split_logical(h[1], '&&'):
            if _starts_statement(lines, i, labels=False):
                cands.append(('name', i))
        m = re.match(r'^([ \t]*)bool\s+(flag\d+)\s*=\s*false;\s*$', line)
        if m and i + 3 < len(lines) and len(re.findall(r'\b%s\b' % m.group(2), body)) == 3:
            a = _if_head(lines[i + 1])
            b = re.match(r'^[ \t]*%s\s*=\s*([^;]+);\s*$' % m.group(2), lines[i + 2])
            c = _if_head(lines[i + 3])
            if a and b and c and a[2].strip() == '' and c[1].strip() == m.group(2):
                cands.append(('inline', i))
    if not cands:
        return None
    kind, i = rng.choice(cands)
    if kind == 'name':
        ind, cond, rest = _if_head(lines[i])
        parts = _split_logical(cond, '&&')
        n = 0
        while re.search(r'\bflag%d\b' % n, body):
            n += 1
        lines[i:i + 1] = [f'{ind}bool flag{n} = false;', f'{ind}if ({parts[0]})',
                          f'{ind}\tflag{n} = {" && ".join(parts[1:])};', f'{ind}if (flag{n}){rest}']
    else:
        ind = re.match(r'^[ \t]*', lines[i]).group(0)
        first = _if_head(lines[i + 1])[1].strip()
        second = re.match(r'^[ \t]*\w+\s*=\s*([^;]+);', lines[i + 2]).group(1).strip()
        rest = _if_head(lines[i + 3])[2]
        lines[i:i + 4] = [f'{ind}if ({_wrap(first, "&&")} && {_wrap(second, "&&")}){rest}']
    return '\n'.join(lines)


def _braced_if(body, m):
    """For the match m of 'if (': (condition, end of the condition, index of '{', index of '}'), else None."""
    end = _split_top(body, m.end() - 1)
    if end is None:
        return None
    k = end
    while k < len(body) and body[k] in ' \t\r\n':
        k += 1
    if k >= len(body) or body[k] != '{':
        return None
    return body[m.end():end - 1], end, k, match_brace(body, k)


def _flat(text):
    return re.sub(r'\s+', ' ', text).strip()


def _line_indent(body, pos):
    return re.match(r'[ \t]*', body[body.rfind('\n', 0, pos) + 1:]).group(0)


def _deeper(block):
    return re.sub(r'\n(?=[^\n])', '\n\t', block)


def _shallower(block):
    return re.sub(r'(?<=\n)\t', '', block)


def mut_or_split(body, rng):
    """'if (a || b) {S}' <-> 'if (a) {S} else if (b) {S}' and
    'if (a && b) {S}' (without an else) <-> 'if (a) { if (b) {S} }'."""
    cands = []
    for m in re.finditer(r'(?:\belse\s+)?\bif\s*\(', body):
        info = _braced_if(body, m)
        if not info:
            continue
        cond, end, k, close = info
        block = body[k:close + 1]
        chain = re.compile(r'\s*else\s+if\s*\(').match(body, close + 1)
        has_else = re.compile(r'\s*else\b').match(body, close + 1) is not None
        ors = _split_logical(cond, '||')
        if ors and len(ors) == 2 and not re.search(r'^[ \t]*\w+:[ \t]*$|\bstatic\b', block, re.M):
            cands.append(('split_or', m, end, k, close, ors))
        if chain:
            other = _braced_if(body, chain)
            if other and _flat(body[other[2]:other[3] + 1]) == _flat(block):
                cands.append(('join_or', m, end, k, close, (cond, other[0], other[3])))
        if m.group(0).startswith('else') or has_else:
            continue
        ands = _split_logical(cond, '&&')
        if ands and len(ands) == 2:
            cands.append(('split_and', m, end, k, close, ands))
        inner = body[k + 1:close].strip()
        im = re.match(r'if\s*\(', inner)
        sub = _braced_if(inner, im) if im else None
        if sub and sub[3] == len(inner) - 1 and not re.compile(r'\s*else\b').match(inner, sub[3] + 1):
            cands.append(('join_and', m, end, k, close, (cond, sub[0], inner[sub[1]:])))
    if not cands:
        return None
    kind, m, end, k, close, ops = rng.choice(cands)
    head, gap, block = body[m.start():m.end()], body[end:k], body[k:close + 1]
    stop = close + 1
    if kind == 'split_or':
        new = f'{head}{ops[0]}){gap}{block} else if ({ops[1]}){gap}{block}'
    elif kind == 'split_and':
        indent = _line_indent(body, m.start())
        inner_gap = gap if '\n' not in gap else f'\n{indent}\t'
        new = (f'{head}{ops[0]}){gap}{{\n{indent}\tif ({ops[1]}){inner_gap}{_deeper(block)}\n{indent}}}')
    elif kind == 'join_or':
        new = f'{head}{_wrap(ops[0], "||")} || {_wrap(ops[1], "||")}){gap}{block}'
        stop = ops[2] + 1
    else:
        new = f'{head}{_wrap(ops[0], "&&")} && {_wrap(ops[1], "&&")}){gap}{_shallower(ops[2].strip())}'
    return body[:m.start()] + new + body[stop:]


ARM = r'(?:-?\d\w*|NONE|true|false)'
TERNARY = re.compile(r'^(?P<indent>[ \t]*)(?P<lhs>[A-Za-z_][\w.\->\[\]]*)\s*=(?!=)\s*(?P<cond>[^?;:]+?)\s*\?\s*(?P<a>'
                     + ARM + r')\s*:\s*(?P<b>' + ARM + r')\s*;\s*$')
ARM_STORE = re.compile(r'\s*(?P<open>\{\s*)?(?P<lhs>[A-Za-z_][\w.\->\[\]]*)\s*=(?!=)\s*(?P<v>' + ARM + r')\s*;')


def _value(literal):
    if literal in ('true', 'false'):
        return int(literal == 'true')
    try:
        return -1 if literal == 'NONE' else int(literal.rstrip('uUlL'), 0)
    except ValueError:
        return None


def _arms_ok(a, b):
    """Both arms are literals that give a ?: the same value as each store: a negative next to an
    unsigned or 32-bit-wide literal would put them in unsigned arithmetic."""
    va, vb = _value(a), _value(b)
    if va is None or vb is None:
        return False
    wide = any(re.search(r'[uU][lL]*$', x) or v >= 2 ** 31 for x, v in ((a, va), (b, vb)))
    return not (wide and (va < 0 or vb < 0))


def _arm(body, pos):
    """(place, literal, end) of a branch at pos that only stores a literal: 'x = 1;' or '{ x = 1; }'."""
    m = ARM_STORE.match(body, pos)
    if not m:
        return None
    end = m.end()
    if m.group('open'):
        close = re.compile(r'\s*\}').match(body, end)
        if not close:
            return None
        end = close.end()
    return m.group('lhs'), m.group('v'), end


def _if_else_stores(body):
    """(start, end, place, condition, a, b) of each 'if (c) x = A; else x = B;' (braces optional)."""
    found = []
    for m in re.finditer(r'\bif\s*\(', body):
        end = _split_top(body, m.end() - 1)
        first = _arm(body, end) if end else None
        e = re.compile(r'\s*else\b').match(body, first[2]) if first else None
        second = _arm(body, e.end()) if e else None
        if second and second[0] == first[0] and _arms_ok(first[1], second[1]):
            found.append((m.start(), second[2], first[0], body[m.end():end - 1].strip(), first[1], second[1]))
    return found


def mut_ternary(body, rng):
    """'x = c ? 1 : 0;' <-> 'if (c) x = 1; else x = 0;' (literal arms only)."""
    lines = body.split('\n')
    cands = []
    for i, line in enumerate(lines):
        m = TERNARY.match(line)
        if m and not {'?', ',', '='} & {o for o, _ in _top_level(m.group('cond'))} \
                and _arms_ok(m.group('a'), m.group('b')):
            cands.append(('expand', i))
    cands += [('fold', f) for f in _if_else_stores(body)]
    if not cands:
        return None
    kind, where = rng.choice(cands)
    if kind == 'expand':
        m = TERNARY.match(lines[where])
        ind, lhs = m.group('indent'), m.group('lhs')
        lines[where] = (f'{ind}if ({m.group("cond").strip()})\n{ind}\t{lhs} = {m.group("a")};\n'
                        f'{ind}else\n{ind}\t{lhs} = {m.group("b")};')
        return '\n'.join(lines)
    start, end, lhs, cond, a, b = where
    cond = f'({cond})' if {'?', ',', '='} & {o for o, _ in _top_level(cond)} else cond
    return body[:start] + f'{lhs} = {cond} ? {a} : {b};' + body[end:]


TERNARY_ANY = re.compile(r'^(?P<head>[ \t]*(?:return|[A-Za-z_][\w.\->\[\]]*\s*=(?!=)))\s*(?P<cond>[^?;:]+?)\s*\?\s*'
                         r'(?P<a>' + OPERAND + r')\s*:\s*(?P<b>' + OPERAND + r')\s*;\s*$')


def mut_swap_ternary(body, rng):
    """'c ? a : b' -> '!c ? b : a' (the condition negated the way mut_swap_if_else negates one)."""
    lines = body.split('\n')
    idx = []
    for i, line in enumerate(lines):
        m = TERNARY_ANY.match(line)
        if m and not {'?', ',', '='} & {o for o, _ in _top_level(m.group('cond'))}:
            idx.append(i)
    if not idx:
        return None
    i = rng.choice(idx)
    m = TERNARY_ANY.match(lines[i])
    lines[i] = f'{m.group("head")} {_negate(m.group("cond"))} ? {m.group("b")} : {m.group("a")};'
    return '\n'.join(lines)


# What the edits that turned near functions into matches did (the last change to the body, from the history of
# config/functions.csv): in the largest group two arguments of a call traded places; next came volatile reads and
# qualifiers, then locals that take the address of a field or hoist a loop's declaration.

CALLEE = re.compile(r'(?<![\w$])(?!(?:if|while|for|switch|return|sizeof)\b)[A-Za-z_]\w*[ \t]*\(')


def _call_args(text, open_index):
    """([(start, end) of each argument], index after the call) for the call whose '(' is at open_index."""
    end = _split_top(text, open_index)
    if end is None:
        return None
    spans, depth, start, i = [], 0, open_index + 1, open_index + 1
    while i < end - 1:
        j = _skip_literal(text, i)
        if j != i:
            i = j
            continue
        if text[i] in '([{':
            depth += 1
        elif text[i] in ')]}':
            depth -= 1
        elif text[i] == ',' and depth == 0:
            spans.append((start, i))
            start = i + 1
        i += 1
    spans.append((start, end - 1))
    return spans, end


def _arg_kind(arg):
    arg = arg.strip()
    if re.fullmatch(r'-?\d\w*|NONE|NULL|true|false', arg):
        return 'literal'
    if arg.startswith('&'):
        return 'address'
    if re.match(r'\([A-Za-z_][\w \t\*]*\)', arg):
        return 'cast'
    return 'value'


def _literal_spans(text):
    """The (start, end) of each comment, string and character literal in text."""
    spans, i = [], 0
    while i < len(text):
        j = _skip_literal(text, i)
        if j != i:
            spans.append((i, j))
        i = max(j, i + 1)
    return spans


def _arg_orders(texts):
    """[(weight, order)] for each way to rearrange a call's arguments: a swap of two (neighbours and arguments of the
    same kind are likelier) or one argument moved two or more places, which also covers a rotation of three."""
    n, seen, found = len(texts), {tuple(texts)}, []
    for i in range(n):
        for j in range(i + 1, n):
            order = list(range(n))
            order[i], order[j] = j, i
            weight = (3 if j == i + 1 else 1) * (2 if _arg_kind(texts[i]) == _arg_kind(texts[j]) else 1)
            found.append((weight, order))
    for i in range(n):
        for j in range(n):
            if abs(i - j) >= 2:
                order = [k for k in range(n) if k != i]
                order.insert(j, i)
                found.append((1, order))
    unique = []
    for weight, order in found:
        key = tuple(texts[k] for k in order)
        if key not in seen:
            seen.add(key)
            unique.append((weight, order))
    return unique


def _rearranged(body, spans, texts, order):
    """body with the arguments at spans replaced by texts[k] for k in order (spacing around each kept)."""
    out = body
    for (a, b), k in reversed(list(zip(spans, order))):   # right to left, so earlier spans keep their place
        lead = len(out[a:b]) - len(out[a:b].lstrip())
        trail = len(out[a:b]) - len(out[a:b].rstrip())
        out = out[:a + lead] + texts[k] + out[b - trail:]
    return out


def _calls(body):
    """[(spans, texts)] for each call in body with two or more arguments, outside comments and strings."""
    calls, literals = [], _literal_spans(body)
    for m in CALLEE.finditer(body):
        if any(a <= m.start() < b for a, b in literals):
            continue
        got = _call_args(body, m.end() - 1)
        if got and len(got[0]) >= 2:
            calls.append((got[0], [body[a:b].strip() for a, b in got[0]]))
    return calls


def mut_swap_args(body, rng):
    """f(a, b, c) -> f(b, a, c) or f(b, c, a): two arguments of one call trade places, or one moves two or more
    places. What the call means changes, so a variant is kept only when its code gets closer to retail."""
    calls = [(spans, texts, orders) for spans, texts in _calls(body) if (orders := _arg_orders(texts))]
    if not calls:
        return None
    spans, texts, orders = rng.choice(calls)
    order = rng.choices([o for _, o in orders], [w for w, _ in orders])[0]
    return _rearranged(body, spans, texts, order)


SCALARS = r'(?:unsigned long|unsigned short|unsigned char|long|short|char|byte|word|dword|int|real|bool)'
FIELD = r'(?:[A-Za-z_]\w*(?:(?:\.|->)[A-Za-z_]\w*|\[[\w ]+\])+|g_\w+)'     # a field, an element or a global
PLAIN_READ = re.compile(r'^(?P<head>[ \t]*(?P<type>' + SCALARS + r')[ \t]+\w+[ \t]*=[ \t]*)(?P<field>' + FIELD
                        + r')[ \t]*;[ \t]*$')
VOLATILE_READ = re.compile(r'^(?P<head>[ \t]*(?P<type>' + SCALARS + r')[ \t]+\w+[ \t]*=[ \t]*)'
                           r'\*\((?:const )?volatile (?P=type) \*\)&(?P<field>' + FIELD + r')[ \t]*;[ \t]*$')
LITERAL_STORE = re.compile(r'^(?P<indent>[ \t]*)(?P<place>' + FIELD + r')[ \t]*=[ \t]*'
                           r'(?P<value>-?\d\w*|NONE|true|false)[ \t]*;[ \t]*$')
VOLATILE_STORE = re.compile(r'^(?P<indent>[ \t]*)\*\(volatile \w+ \*\)&(?P<place>' + FIELD + r')[ \t]*=[ \t]*'
                            r'(?P<value>[^;]+);[ \t]*$')


def mut_volatile_read(body, rng):
    """'T x = p->f;' <-> 'T x = *(volatile T *)&p->f;' (a global too), and
    'p->f = 0;' <-> '*(volatile T *)&p->f = 0;': the access goes to memory, at T's width."""
    lines = body.split('\n')
    cands = [(i, 'wrap') for i, l in enumerate(lines) if PLAIN_READ.match(l)]
    cands += [(i, 'plain') for i, l in enumerate(lines) if VOLATILE_READ.match(l)]
    cands += [(i, 'store') for i, l in enumerate(lines) if LITERAL_STORE.match(l)]
    cands += [(i, 'unstore') for i, l in enumerate(lines) if VOLATILE_STORE.match(l)]
    if not cands:
        return None
    i, kind = rng.choice(cands)
    if kind == 'wrap':
        m = PLAIN_READ.match(lines[i])
        lines[i] = f'{m.group("head")}*(volatile {m.group("type")} *)&{m.group("field")};'
    elif kind == 'plain':
        m = VOLATILE_READ.match(lines[i])
        lines[i] = f'{m.group("head")}{m.group("field")};'
    elif kind == 'store':
        m = LITERAL_STORE.match(lines[i])
        width = ('bool' if m.group('value') in ('true', 'false')
                 else rng.choice(['long', 'long', 'long', 'short', 'byte']))
        lines[i] = f'{m.group("indent")}*(volatile {width} *)&{m.group("place")} = {m.group("value")};'
    else:
        m = VOLATILE_STORE.match(lines[i])
        lines[i] = f'{m.group("indent")}{m.group("place")} = {m.group("value")};'
    return '\n'.join(lines)


POINTER_LOCAL = re.compile(r'^(?P<indent>[ \t]*)(?P<type>(?:const )?[A-Za-z_]\w*)(?P<volatile> volatile)?'
                           r'(?P<const> const)?[ \t]*\*[ \t]*(?P<name>\w+)[ \t]*=[ \t]*(?P<rest>[^;]+;[ \t]*)$')


def mut_volatile_pointer(body, rng):
    """'T *p = e;' <-> 'T volatile *p = e;': every read through p goes to memory."""
    lines = body.split('\n')
    cands = []
    for i, l in enumerate(lines):
        m = POINTER_LOCAL.match(l)
        if m and m.group('type') not in ('return', 'else', 'case', 'goto', 'delete'):
            cands.append(i)
    if not cands:
        return None
    i = rng.choice(cands)
    m = POINTER_LOCAL.match(lines[i])
    volatile = '' if m.group('volatile') else ' volatile'
    lines[i] = (f'{m.group("indent")}{m.group("type")}{volatile}{m.group("const") or ""} *{m.group("name")} = '
                f'{m.group("rest")}')
    return '\n'.join(lines)


MUTATIONS = [mut_swap_operands, mut_reorder, mut_retype, mut_introduce_temp, mut_inline_temp, mut_swap_if_else,
             mut_for_while, mut_compound, mut_increment, mut_split_decl, mut_chain_assign, mut_bool_flag,
             mut_or_split, mut_ternary, mut_swap_ternary, mut_swap_args,
             mut_volatile_read, mut_volatile_pointer]


# Tried more often: call arguments traded places in a fifth of the edits that made a near function match, and
# statement order or operand order in a few more (see the comment above mut_swap_args). The rest weigh 1.
WEIGHTS = {mut_swap_args: 6, mut_reorder: 2, mut_swap_operands: 2}


def mutate(body, rng, count=None):
    """Applies 1 or 2 random mutations; None when none applies."""
    count = count or rng.choice([1, 1, 2])
    changed = False
    for _ in range(count):
        order = MUTATIONS[:]
        while order:
            mut = rng.choices(order, [WEIGHTS.get(m, 1) for m in order])[0]
            order.remove(mut)
            new = mut(body, rng)
            if new is not None and new != body:
                body, changed = new, True
                break
    return body if changed else None


# ------------------------------------------------------------------ scoring

class Scorer:
    """Builds a scratch root and scores variants of one function's source file.
    image, if given, is where the build links the image (see build.build)."""

    def __init__(self, va, root, xdk=None, image=None):
        self.va, self.root, self.xdk, self.image = va, root, xdk, image
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

    def build(self):
        """Builds the scratch root: (map path, image path)."""
        if self.image is None:
            return build.build(self.root, self.xdk), os.path.join(self.root, 'build', build.EXE_NAME)
        return build.build(self.root, self.xdk, image=self.image), self.image

    def score_text(self, text):
        """Writes text as the source file and returns (score, first-difference text)."""
        with open(self.path, 'w', encoding='utf-8', newline='') as f:
            f.write(text)
        try:
            return self.score_image(*self.build())
        except (SystemExit, StopIteration, ValueError, OSError, KeyError):
            return WORST, 'build failed'

    def score_image(self, map_path, exe_path):
        """(score, first-difference text) of the function in a built image."""
        try:
            linkmap = LinkMap.read(map_path)
            image = Pe(exe_path)
            marked = build.marked_sources(self.root)
            stubs = build.stub_sources(self.root)
            standin_calls = check.StandinCalls(linkmap, image, marked)
            identity = check.Identity(linkmap, self.rows, marked + stubs, standin_calls)
            m = next(m for m in marked if m.retail == self.va)
            symbol = check.resolve(linkmap, m, standin_calls)
            # resolve returns None when the linker folded this variant into an
            # identical function, or nothing references it. That is a failed
            # candidate, not a crash: aborting here would throw away the search.
            if symbol is None:
                return WORST, 'not in the image'
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


# ------------------------------------------------------------ reduced image

def reduced_units(va, rows, markers, depth=None):
    """The sources of the reduced image that scores va (tools/build.py's
    build(units=...)): the one that marks va and those that mark its callees in
    retail (the calls column of config/functions.csv, with @retail or @stub
    markers), followed through callees that are themselves marked @retail, to
    depth levels (None: all the way). Link-time code generation fits a call to
    its callee's code, and that code to the callee's own callees, so leaving a
    callee out changes how the target calls it. A stub is a leaf: it is built
    without LTCG. A callee that no source marks comes from the libraries."""
    where = {m.retail: m.path for m in markers}
    decompiled = {m.retail for m in markers if not m.stub}
    seen, frontier, level = {va}, [va], 0
    while frontier and (depth is None or level < depth):
        level += 1
        found = [int(t, 16) for f in frontier if f in decompiled for t in rows.get(f, {}).get('calls', '').split()]
        frontier = [c for c in dict.fromkeys(found) if c not in seen]
        seen.update(frontier)
    return {where[f] for f in seen if f in where}


LTCG_LIBRARIES = {name[:-len('.lib')] for name in build.LIBRARIES if name.endswith('ltcg.lib')}  # built with /GL


def library_symbols(linkmap):
    """One public symbol of each object an image (given its LinkMap) took from
    a library built without LTCG. A reduced image that takes the same objects
    (/INCLUDE) keeps the standard conventions their calls force on the
    functions they call, such as the compiler's vector iterators."""
    found = {}
    for s in linkmap.symbols:
        library, colon, _ = s.object.partition(':')
        if colon and not s.static and library not in LTCG_LIBRARIES:
            found.setdefault(s.object, s.name)
    return sorted(found.values())


def _rel_target(image, field):
    return (field + 4 + int.from_bytes(image.read(field, 4), 'little', signed=True)) & 0xFFFFFFFF


def pinning_units(linkmap, image, symbol, sources):
    """The sources that, in a full image (its LinkMap and Pe), pin the calling
    convention of the function at symbol or of a function it calls directly:
    those that take such a function's address (link-time code generation keeps
    the standard convention of a function whose address escapes, such as a
    callback) and stubs, built without LTCG, that call one. sources maps the
    map's object names to their sources (build.source_objects)."""
    start, end = linkmap.extent(symbol)
    pinned = {symbol.va}
    for field in linkmap.rel_fixups:
        if start <= field < end:
            callee = linkmap.symbol_at(_rel_target(image, field))
            if callee is not None and callee.va != symbol.va:
                pinned.add(callee.va)
    objects = set()
    for field in image.fixups:
        if int.from_bytes(image.read(field, 4), 'little') in pinned and linkmap.symbol_at(field):
            objects.add(linkmap.symbol_at(field).object)
    for field in linkmap.rel_fixups:
        caller = linkmap.symbol_at(field)
        if (caller is not None and sources.get(caller.object, '').startswith('src/stubs/')
                and _rel_target(image, field) in pinned):
            objects.add(caller.object)
    return {sources[o] for o in objects if o in sources}


class FastScorer(Scorer):
    """A Scorer that builds a reduced image (see reduced_units): only the
    target's source is recompiled, and the link leaves out most of the program.
    The image is linked to image; include lists symbols the link takes in (see
    library_symbols). Scores come from the same comparison as the full
    image's."""

    def __init__(self, va, root, image, xdk=None, depth=None, include=()):
        super().__init__(va, root, xdk, image)
        self.units = reduced_units(va, self.rows, build.marked_sources(root) + build.stub_sources(root), depth)
        self.include = include

    def build(self):
        return build.build(self.root, self.xdk, units=self.units, image=self.image, include=self.include), self.image


def prepare_scratch(scratch):
    for d in ('src', 'include', 'config', 'build'):
        s = os.path.join(ROOT, d)
        if os.path.isdir(s):
            shutil.copytree(s, os.path.join(scratch, d), copy_function=shutil.copy2,
                            ignore=shutil.ignore_patterns('*.exe', '*.map', 'permute') if d == 'build' else None)


@dataclass
class Result:
    baseline: int
    best: int
    text: str
    original: str
    tries: int
    seconds: float
    note: str = ''
    # --fast: baseline and best are still full-build scores; these are the reduced image's
    fast_baseline: int = None
    fast_best: int = None
    confirmations: int = 0


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


# --------------------------------------------------------------- --fast search

class FastSearch:
    """The state of a --fast search, shared by its workers. The search walks as
    the default one does, but on scores from the reduced image: a variant that
    scores no worse than the walk's position becomes the position. One that
    scores better is first confirmed: confirm (a full build) scores it, and it
    becomes the best, and the position, only when that score beats the best's
    full score. The best is the only variant ever written, so every written
    variant's score comes from a full build: baseline, the original's full
    score, to start with."""

    def __init__(self, prefix, body, suffix, confirm, baseline, log=print):
        self.prefix, self.original, self.suffix = prefix, body, suffix
        self.confirm, self.log = confirm, log
        self.lock = threading.Lock()  # guards the state below
        self.confirming = threading.Lock()  # one full build at a time
        self.walk, self.walk_score = body, None  # the position and its reduced-image score
        self.fast_baseline = None
        self.best, self.best_score = body, baseline  # the best and its full-build score
        self.baseline = baseline
        self.seen = {body}
        self.tries = 0
        self.confirmations = 0
        self.done = False

    def text(self, body):
        return self.prefix + body + self.suffix

    def start(self, score, note):
        """Records the original's reduced-image score (each worker scores it once)."""
        with self.lock:
            if self.fast_baseline is None:
                self.fast_baseline = self.walk_score = score
                self.log(f'baseline: {score} differing instructions on the reduced image ({note})')
                if score != self.baseline:
                    self.log(f'warning: the reduced image scores the original {score}, the full image '
                             f'{self.baseline}; reduced-image scores may not follow full ones for this function')
            elif score != self.fast_baseline:
                self.log(f'warning: a worker scored the original {score}, not {self.fast_baseline}')

    def propose(self, rng, tries):
        """(a new variant of the position, its try number), or (None, None) when
        the search is over or no new variant turns up."""
        with self.lock:
            if self.done or self.tries >= tries:
                return None, None
            for _ in range(50):
                candidate = mutate(self.walk, rng)
                if candidate is not None and candidate not in self.seen:
                    self.seen.add(candidate)
                    self.tries += 1
                    return candidate, self.tries
            self.log('no new mutations available')
            self.done = True
            return None, None

    def offer(self, candidate, score, note, n):
        """Takes a variant's reduced-image score; confirms it if it is better."""
        with self.lock:
            if score > self.walk_score:
                return
            if score == self.walk_score:
                self.walk = candidate
                return
        with self.confirming:
            with self.lock:
                if self.done or score >= self.walk_score:  # another worker moved on meanwhile
                    return
            full, full_note = self.confirm(self.text(candidate))
            with self.lock:
                self.confirmations += 1
                better = full < self.best_score
                self.log(f'try {n}: {score} on the reduced image ({note}), {full} in the full image ({full_note}): '
                         + ('new best' if better else 'rejected'))
                if better:
                    self.best, self.best_score = candidate, full
                    self.walk, self.walk_score = candidate, score
                    self.done = full == 0


def _work(search, scorer, rng, tries, deadline):
    """One worker of a --fast search."""
    search.start(*scorer.score_text(search.text(search.original)))
    while deadline is None or time.time() < deadline:
        candidate, n = search.propose(rng, tries)
        if candidate is None:
            break
        score, note = scorer.score_text(search.text(candidate))
        search.offer(candidate, score, note, n)


def run_fast(va, tries=None, seed=0, time_limit=None, scratch=None, log=print, jobs=1, depth=None):
    """The --fast search with jobs workers, each scoring on its own scratch root
    with its own seed (seed, seed + 1, ...), and one more root for the full
    builds: first the original's, whose map gives the library objects the
    reduced images take in, then those that confirm. tries None: 200, or no
    limit with a time limit, which starts after that first build. The images
    are linked in build/permute/<va>/ of the repository, one folder per root,
    as .img files (see build.build)."""
    if tries is None:
        tries = DEFAULT_TRIES if time_limit is None else float('inf')
    own = scratch is None
    scratch = scratch or tempfile.mkdtemp(prefix='permute_')
    images = os.path.join(ROOT, 'build', 'permute', f'{va:08x}')
    try:
        names = ['full', *(f'fast{k}' for k in range(jobs))]
        roots = [os.path.join(scratch, name) for name in names]
        for root in roots:
            os.makedirs(root, exist_ok=True)
            prepare_scratch(root)
        full = Scorer(va, roots[0], image=os.path.join(images, 'full', 'halo2.img'))
        scorers = [FastScorer(va, root, os.path.join(images, name, 'reduced.img'), depth=depth)
                   for name, root in zip(names[1:], roots[1:])]
        with open(os.path.join(ROOT, full.marked.path), encoding='utf-8', newline='') as f:
            original = f.read()
        start, end = find_body(original, va)
        t0 = time.time()
        baseline, note = full.score_text(original)
        log(f'baseline: {baseline} differing instructions in the full image ({note}; {time.time() - t0:.0f}s)')
        map_path = os.path.splitext(full.image)[0] + '.map'
        if baseline < WORST and os.path.exists(map_path):
            linkmap, image = LinkMap.read(map_path), Pe(full.image)
            symbol = check.resolve(linkmap, full.marked,
                                   check.StandinCalls(linkmap, image, build.marked_sources(full.root)))
            include = library_symbols(linkmap)
            pinning = pinning_units(linkmap, image, symbol, build.source_objects(full.root))
            for scorer in scorers:
                scorer.include = include
                scorer.units |= pinning
        log(f'reduced image: {len(scorers[0].units)} sources: ' + ', '.join(sorted(scorers[0].units)))
        search = FastSearch(original[:start], original[start:end], original[end:], full.score_text, baseline, log)
        t0 = time.time()
        deadline = None if time_limit is None else t0 + time_limit
        with ThreadPoolExecutor(jobs) as pool:
            work = [pool.submit(_work, search, s, random.Random(seed + k), tries, deadline)
                    for k, s in enumerate(scorers)]
            try:
                for w in work:
                    w.result()  # a worker's exception, or Ctrl-C, stops the others after their current try
            finally:
                search.done = True
        return Result(search.baseline, search.best_score, search.text(search.best), original, search.tries,
                      time.time() - t0, fast_baseline=search.fast_baseline, fast_best=search.walk_score,
                      confirmations=search.confirmations)
    finally:
        shutil.rmtree(images, ignore_errors=True)
        if own:
            shutil.rmtree(scratch, ignore_errors=True)


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument('address', help='retail address of a function marked // @retail in src/')
    ap.add_argument('--tries', type=int, default=None,
                    help='default 200; with --fast and --time-limit, as many as the time allows')
    ap.add_argument('--seed', type=int, default=0)
    ap.add_argument('--time-limit', type=float, default=None, help='seconds')
    ap.add_argument('--write', action='store_true', help='write a strictly better variant back to the source file')
    ap.add_argument('--fast', action='store_true',
                    help='score on a reduced image; a full build confirms each improvement')
    ap.add_argument('--jobs', type=int, default=1, help='with --fast: scorers working in parallel')
    ap.add_argument('--depth', type=int, default=None,
                    help='with --fast: follow callees only this many levels (default: all)')
    args = ap.parse_args()
    if args.jobs < 1 or (args.jobs > 1 and not args.fast):
        ap.error('--jobs takes a positive count, and needs --fast')
    if args.depth is not None and (args.depth < 0 or not args.fast):
        ap.error('--depth takes a count of levels, and needs --fast')
    va = check.parse_addresses([args.address]).pop()
    if args.fast:
        result = run_fast(va, args.tries, args.seed, args.time_limit, jobs=args.jobs, depth=args.depth)
    else:
        result = run(va, DEFAULT_TRIES if args.tries is None else args.tries, args.seed, args.time_limit)
    rate = result.tries / result.seconds if result.seconds else 0
    if args.fast:
        print(f'{result.tries} tries in {result.seconds:.1f}s ({rate:.2f} tries/s); reduced image: best '
              f'{result.fast_best} (baseline {result.fast_baseline}); full image: best {result.best} '
              f'(baseline {result.baseline}), {result.confirmations} confirming builds'
              + (' MATCH' if result.best == 0 else ''))
    else:
        print(f'{result.tries} tries in {result.seconds:.1f}s ({rate:.2f} tries/s); '
              f'best score {result.best} (baseline {result.baseline})' + (' MATCH' if result.best == 0 else ''))
    # with --fast too, both scores come from full builds
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
