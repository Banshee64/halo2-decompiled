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

#endif
