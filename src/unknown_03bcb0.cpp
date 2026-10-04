// @flags /O2 /arch:SSE /Ob1 /Gr
/* UNKNOWN_03BCB0.CPP: predicting one bitmap's texture (unknown_03bcb0.h);
   its own /Ob1 file because retail calls it out of line from 0x16e5e0 */

#include "unknown_11c920.h"
#include "unknown_03bcb0.h"

// @retail 0x3bcb0
void function_3bcb0(s_bitmap_data *bitmap)
{
	bitmap_predict_inline((s_bitmap_predict_view *)bitmap, 0xe);
}
