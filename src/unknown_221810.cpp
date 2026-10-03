// @flags /O2 /Ob1 /arch:SSE /Gr
/* a sound class of the sound classes tag (0x5c bytes). Retail calls this out
   of line everywhere, so it has a file of its own built /Ob1, where LTCG
   doesn't inline it (it was in unknown_221490.cpp). */

#include "cseries.h"
#include "globals.h"
#include "sound_classes.h"

// @retail 0x221810
s_unknown_5c *function_221810(
	short index)
{
	return sound_class_definition_get(index);
}
