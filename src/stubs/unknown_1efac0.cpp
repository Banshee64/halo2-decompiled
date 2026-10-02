// stub for the c_a virtual in the library range
#include "unknown_1efac0.h"

/* a distinct body, so that the linker does not fold this stub into another empty virtual */
static volatile long g_stub_2d7240;

// @stub 0x2d7240
void c_a::v2(bool enable)
{
	g_stub_2d7240 = enable;
}
