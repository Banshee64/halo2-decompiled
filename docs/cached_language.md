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
converter. The new definition will reuse it and retain the upstream
`long function_11cae0(void)` declaration used by an existing caller.

## Planned integration

Add `src/cached_language.cpp` and remove only the replaced `0x11cae0`
stub from `src/stubs/lane_g.cpp`. No new global, dependency stub, shared
header edit, or change to another file's compiler flags is planned.
Existing converter and inline callers remain unchanged.

The draft range claim precedes implementation. Validation will include a
full match-preservation check and retail-versus-linked comparisons of
cached and uncached calls, language conversion, and reuse of the cache.
