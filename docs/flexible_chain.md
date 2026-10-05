# Flexible chain callbacks

**Retail range claimed: `0x115d10`–`0x11689f`**

This pass covers all 11 missing entries in the range (2,875 retail bytes).
The callback table at `0x4674a0` contains nine of the entries. Its creation,
update and attachment callbacks reach the two remaining helpers at
`0x1161f0` and `0x116750`. They share the record pool at `0x4e0334`;
its initialization uses the retail string `antenna` at `0x4532f0`.
The pool holds 12 records of 0x2bc bytes, with a chain of 0x20-byte nodes.

The preceding device callback group ends before `0x115d10`. The next
callback group starts at `0x1168a0` and uses a separate pool and table.
Issue #9 and open pull requests were checked before this claim; no active
claim overlaps this range. The adjacent cloth pass is PR #48.

Implementation and validation results will be recorded here as the pass
progresses. This initial document reserves the range before source work.
Names will describe retail behavior or use address placeholders; evidence
comes from the retail executable and existing project declarations.
