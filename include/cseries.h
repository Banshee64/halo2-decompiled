/* CSERIES.H: basic types and conventions shared by every source file */

#ifndef CSERIES_H
#define CSERIES_H

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;
typedef float real;

#define NONE -1

/* Bungie's static functions. Empty here, so the build's stand-in callers in
   other files can reach them; LTCG sees the whole program either way. */
#define PRIVATE

/* Bit tests. Halo 2 reads a flag as a 1-bit bitfield of a 16-bit flags word
   converted to bool; this is the only form VC7.1 LTCG compiles to
   "mov al,[mem]; shr al,N; test al,1" (plain masks, (x >> n) & 1, inline
   functions and c_flags-style templates all fold to "test byte ptr [mem], mask"). */
#define TEST_FIELD_BIT(field) ((bool)(field))

#endif
