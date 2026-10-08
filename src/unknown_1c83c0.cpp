#include "unknown_11c920.h"
#include "globals.h"
// @flags /O2 /Gr

void function_1c81c0();
void function_201c80();
void function_268510();
void function_1e2f50();
void function_20fb30();
void function_293830();
void function_1e3240();

// @retail 0x1c83c0
void function_1c83c0()
{
 if (g_4f55d0->active && g_4e6948->mode != 4)
 {
  if (g_4f55d0->enabled)
  {
   function_1c81c0();
   short local_0 = g_4f55d0->unknown22;
   if (local_0 > 0)
    g_4f55d0->unknown22 = local_0 - 1;
   function_201c80();
   function_268510();
   function_1e2f50();
   function_20fb30();
   function_293830();
   g_4f55d0->unknown02 = true;
  }
  else if (g_4f55d0->unknown02)
  {
   function_1e3240();
   g_4f55d0->unknown02 = false;
  }
 }
}
