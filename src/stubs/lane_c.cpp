// stubs for lane C (0x1c0000..0x1cffff): callees outside the region that are
// not decompiled yet, and the library (Havok) functions the region calls
#include "cseries.h"
#include "unknown_1cec30.h"

// @stub 0x3123a0
hkPropertyValue hkEntity::removeProperty(dword key) { return hkPropertyValue(0); }

// @stub 0x312530
void hkEntity::addProperty(dword key, hkPropertyValue value) { }
