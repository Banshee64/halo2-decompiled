// stubs for lane C (0x1c0000..0x1cffff): callees outside the region that are
// not decompiled yet, and the library (Havok) functions the region calls
#include "cseries.h"
#include "unknown_1cec30.h"
#include "slot_owner.h"
#include "unknown_1c62f0.h"

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
/* game functions outside the region called by the physics lifecycle callbacks */

// @stub 0x2263c0
void function_2263c0(void) { }

// @stub 0x146b30
void function_146b30(void) { }

// @stub 0x146b80
void function_146b80(void) { }

// @stub 0x146de0
void function_146de0(void) { }

// @stub 0x226440
void function_226440(void) { }
/* game functions outside the region called by the ai lifecycle callbacks */

// @stub 0x1dfae0
void function_1dfae0(void) { }

// @stub 0x28d930
void function_28d930(void) { }

// @stub 0x25c170
void function_25c170(void) { }

// @stub 0x200930
void function_200930(void) { }

// @stub 0x257d00
void function_257d00(void) { }

// @stub 0x20b930
void function_20b930(void) { }

// @stub 0x292130
void function_292130(void) { }

// @stub 0x1a6d80
void function_1a6d80(void) { }

// @stub 0x28d9d0
void function_28d9d0(void) { }

// @stub 0x292e00
void function_292e00(void) { }

// @stub 0x292f60
void function_292f60(void) { }
/* the animation graph lookups (0x1d9000..0x1de000) */

// @stub 0x1ddb40
void *__stdcall function_1ddb40(void *graph, c_animation_id animation_id) { return 0; }

/* the physics callees of the havok components */
struct s_havok_component;

// @stub 0x1d1260
void function_1d1260(s_havok_component *component) { }

// @stub 0x1d01c0
void __stdcall function_1d01c0(s_havok_component *component) { }