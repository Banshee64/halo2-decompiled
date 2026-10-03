/* HS_LIBRARY_EXTERNAL.H: what the script functions' evaluators share */

#ifndef HS_LIBRARY_EXTERNAL_H
#define HS_LIBRARY_EXTERNAL_H

#include "cseries.h"
#include "hs.h"

/* hs_return: stores a script function's value in the calling frame */
void function_209ae0(long thread_index, long value);

/* evaluates a script function's arguments one per call; returns the
   arguments once they are all evaluated, NULL until then */
long *__stdcall hs_macro_function_evaluate(long thread_index, short parameter_count, short const *parameter_types, bool initialize);

/* callees not decompiled yet (stubs in src/stubs/lane_a.cpp) */
void __stdcall function_29fe10(long index);
void function_159ac0(void);
long function_15e730(void);
void function_11b350(void);
void function_277380(void);
void function_13bff0(void);
void function_13ca80(void);
void function_1388e0(void);
bool function_226190(void);
void scripted_hud_messages_clear(void);
bool function_187ec0(void);
long function_1564e0(void);
void function_135750(void);
void function_135790(void);
void function_1915f0(void);

#endif