// stubs for game functions not decompiled yet, called by damage.cpp
#include "cseries.h"

/* compares two string ids for bsearch_elements */
// @stub 0x122cf0
long __stdcall function_122cf0(const void *a, const void *b, const void *context) { return *(long *)a - *(long *)b; }
/* the difficulty multiplier of a team (kind 1 body, 2 shield); retail passes
   both arguments in registers and returns in xmm0 */
// @stub 0x1e9720
real function_1e9720(long kind, short team) { return 1.0f; }
struct s_object_child_iterator;
struct s_damage_owner;
struct s_damage_info;
struct s_damage_region_accumulator;
struct s_damage_object;

/* starts an object child iterator; retail passes both in registers */
// @stub 0xd0620
void function_d0620(s_object_child_iterator *iterator, long object_index) { }
// @stub 0xb8b70
void function_b8b70(long object_index) { }
/* destroys one damage info region */
// @stub 0xdae60
void __stdcall function_dae60(s_damage_info *info, long object_index, s_damage_owner const *owner, long region_index, s_damage_region_accumulator *accumulator) { }
// @stub 0xdbfb0
void __stdcall function_dbfb0(s_damage_object *object, s_damage_owner const *owner, long a, long b, long c) { }
// @stub 0xdbc80
void __stdcall function_dbc80(long object_index, short a, short b) { }
// @stub 0xe6460
void __stdcall function_e6460(long object_index) { }
/* creates an effect on an object */
// @stub 0x176780
void function_176780(long effect_index, long object_index, s_damage_owner const *owner, long a, long b, long c) { }
// @stub 0xba7f0
void __stdcall function_ba7f0(long object_index, long a, long b, long c) { }
