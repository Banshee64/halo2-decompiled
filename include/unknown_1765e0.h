#pragma once

/* 0x1765e0 takes three of its arguments in registers in retail (esi, ebx and
   eax) and is not decompiled yet, so its callers can't match and these
   parameter types are provisional: every argument is a 4-byte value. */
void function_1765e0(void const *a, void const *b, long c, long d, long e, long f);
