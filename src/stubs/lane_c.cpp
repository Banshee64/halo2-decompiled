// stubs for lane C (0x1c0000..0x1cffff): callees outside the region that are
// not decompiled yet, and the library (Havok) functions the region calls
#include "cseries.h"
#include "unknown_1cec30.h"
#include "slot_owner.h"

// @stub 0x3123a0
hkPropertyValue hkEntity::removeProperty(dword key) { return hkPropertyValue(0); }

// @stub 0x312530
void hkEntity::addProperty(dword key, hkPropertyValue value) { }

/* slot handler callbacks of the region not decompiled yet (their handler
   structs hold their addresses) */

// @stub 0x1c0300
short __stdcall function_1c0300(long actor_index) { return 0; }

// @stub 0x1c04f0
short __stdcall function_1c04f0(long actor_index, s_slot *slot, bool active) { return 0; }

// @stub 0x1c0670
void __stdcall function_1c0670(long actor_index, s_slot *slot) { }
// @stub 0x1c1160
short __stdcall function_1c1160(long actor_index, s_slot *slot, bool active) { return 0; }

// @stub 0x1c1320
void __stdcall function_1c1320(long actor_index, s_slot *slot) { }

// @stub 0x1c15c0
bool __stdcall function_1c15c0(long actor_index, s_slot *slot) { return false; }

// @stub 0x1c1730
void __stdcall function_1c1730(long actor_index, s_slot *slot) { }

// @stub 0x1c2130
void __stdcall function_1c2130(long actor_index, s_slot *slot) { }