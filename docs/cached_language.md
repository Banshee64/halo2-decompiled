# Cached language getter

## Claim

Retail range: **`0x11cae0`–`0x11caff`**, one 26-byte function.
Issue #9 and open PR descriptions were checked before claiming; this
range has no active overlap.

## Retail behavior

`0x11cae0` loads the cached language at `0x47ff38`. If it is `-1`, the
function calls the Xbox API `XGetLanguage`, converts the result through
the existing helper at `0x11ca80`, and stores the converted value in the
cache. It returns the cached value. Any value other than `-1` bypasses
both calls.

The existing `include/language.h` already expresses this behavior in its
inline `get_current_language` helper and declares the shared cache and
converter. The new definition reuses it and retains the upstream
`long function_11cae0(void)` declaration used by an existing caller.

## Integration

`src/cached_language.cpp` replaces the `0x11cae0` stub in
`src/stubs/lane_g.cpp`. No new global, dependency stub, shared
header edit, or change to another file's compiler flags is included.
Existing converter and inline callers remain unchanged.

## Results and validation

`function_11cae0` matches all **26 retail bytes** on the first
implementation/build, using `/O2 /Gr` and the existing inline helper.
The global operand resolves to the existing cache, and the call targets
resolve to the Xbox API and existing converter.

Validation base: upstream `9189415`, including its identifier and file
renames. Upstream records 5,062 game/total matches; the initial full baseline
on `9816773` reproduced that count. The rebased full implementation check
reports **5,063 game/total matches**,
with all upstream matches preserved. Draft PR #42 published the range
claim before implementation. The policy documents and PR checklist were
read before publication; evidence comes from retail code and existing
project source.

All **48 retail-versus-linked comparisons** pass:

- 17 uncached cases cover platform values 0 through 12, negative one,
  both signed integer extremes, and 65,536. The actual converter runs;
  only the platform API result is supplied by a test hook.
- 17 second calls retain the cache from the preceding call and change
  the platform value. No further platform query occurs.
- 14 additional cached cases verify that every non-sentinel cache value
  is returned unchanged, including negative and out-of-range values.
- Every case compares the return value, cache, adjacent guard words,
  platform call count, and stack cleanup. No in-game tests.
