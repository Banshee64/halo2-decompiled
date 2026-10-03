// stubs for the callees of lane B (0x1b0000..0x1bffff) not decompiled yet
#include "cseries.h"
#include "slot_handler.h"

/* outside the region: functions the region's functions call */

struct s_object_seat;
struct s_object_child_iterator;

// @stub 0xc8a40
void __stdcall function_c8a40(long object_index, s_object_seat *seats, short *count, short maximum_count) { }

// @stub 0xc8f60
long unit_seat_get_occupant(long unit_index, short seat_index) { return 0; }

// @stub 0xc8200
bool function_c8200(long object_index, long unit_index, short seat_index) { return 0; }

// @stub 0xd0620
void function_d0620(long object_index, s_object_child_iterator *iterator) { }

struct s_262b40_result;

struct s_261d20_entry;

// @stub 0x262b40
s_262b40_result *__stdcall function_262b40(s_reference reference) { return 0; }

// @stub 0x210b60
real function_210b60(s_262b40_result *path) { return 0; }

// @stub 0x261d20
short __stdcall function_261d20(long actor_index, s_261d20_entry *entries, long maximum_count, long unknown, real_point3d const *point) { return 0; }

struct s_slot_entry_iterator;

struct s_slot_memory_entry;

struct s_actor_group_iterator;

struct s_squad_actor_iterator;

// @stub 0x26f0c0
s_slot_memory_entry *function_26f0c0(s_slot_entry_iterator *iterator) { return 0; }

// @stub 0x26ecc0
bool function_26ecc0(long actor_index, s_slot *slot, s_reference reference) { return 0; }

// @stub 0x26e940
long function_26e940(long actor_index) { return 0; }

// @stub 0x1f4280
void __stdcall function_1f4280(long actor_index) { }

// @stub 0x25ab50
bool function_25ab50(long point_reference) { return 0; }

// @stub 0x26eae0
bool invite_actor(long actor_index, long other_index, long a, short type, real weight) { return 0; }

// @stub 0x272d90
void function_272d90(long group_index, s_actor_group_iterator *iterator) { }

// @stub 0x272e20
s_actor_view *function_272e20(s_actor_group_iterator *iterator) { return 0; }

// @stub 0x204d30
void function_204d30(long squad_index, s_squad_actor_iterator *iterator) { }

// @stub 0x20ba60
bool __stdcall function_20ba60(short type, long object_index, long a, long b, long c, long d) { return 0; }

// @stub 0x110ab0
bool __stdcall function_110ab0(long unit_index) { return 0; }

// @stub 0x2628f0
void __stdcall function_2628f0(long actor_index, s_reference reference) { }

struct s_squad_iterator;

struct s_location_view;

// @stub 0x262800
void function_262800(long actor_index, s_reference reference, bool unknown) { }

// @stub 0x258b20
void function_258b20(long index, long actor_index) { }

// @stub 0x1f4460
bool function_1f4460(long actor_index, void *data, long a, long b, long c) { return 0; }

// @stub 0x267770
void function_267770(long prop_index, long actor_index) { }

// @stub 0x1fb7e0
bool function_1fb7e0(long actor_index, short type, void *data, long target_index, long unknown) { return 0; }

// @stub 0x1e4e50
void *function_1e4e50(long actor_index) { return 0; }

// @stub 0x1e4ef0
void *function_1e4ef0(long actor_index) { return 0; }

// @stub 0x1e4d10
void *function_1e4d10(long actor_index) { return 0; }

// @stub 0x1e4db0
void *function_1e4db0(long actor_index) { return 0; }

// @stub 0x204ec0
void function_204ec0(s_squad_iterator *iterator, short encounter_index, short a, bool b) { }

// @stub 0x205010
short function_205010(s_squad_iterator *iterator) { return 0; }

// @stub 0xf5dc0
bool function_f5dc0(long object_index) { return 0; }

// @stub 0x1a77a0
short function_1a77a0(long actor_index, long a, short level) { return 0; }

// @stub 0x26bfa0
void function_26bfa0(long object_index, long *location_index, s_location_view *location) { }

// @stub 0x25d9b0
bool function_25d9b0(long prop_index) { return 0; }

struct s_prop_node_view;

// @stub 0x1f4810
bool __stdcall function_1f4810(long actor_index, long prop_index, real distance, long unknown) { return 0; }

// @stub 0x265c30
void function_265c30(long prop_index, long actor_index, bool unknown) { }

// @stub 0x25da00
bool function_25da00(s_prop_node_view *node) { return 0; }

// @stub 0x26ed40
void function_26ed40(long actor_index, s_reference reference) { }

// @stub 0x26fc80
bool function_26fc80(long actor_index, long object_index, real distance, void *path) { return 0; }

// @stub 0x26c180
void function_26c180(long actor_index) { }

/* outside the region: callbacks */

// @stub 0x1a79e0
short __stdcall function_1a79e0(long actor_index, short level, bool active) { return 0; }

// @stub 0x1ad550
short __stdcall function_1ad550(long actor_index) { return 0; }

// @stub 0x1adcd0
short __stdcall function_1adcd0(long actor_index) { return 0; }

// @stub 0x1afde0
short __stdcall function_1afde0(long actor_index) { return 0; }

// @stub 0x1afe50
bool __stdcall function_1afe50(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1aff10
void __stdcall function_1aff10(long actor_index, s_slot *slot) { }

// @stub 0x1c0b60
void __stdcall function_1c0b60(long actor_index, s_slot *slot, long index) { }

// @stub 0x1c1520
void __stdcall function_1c1520(long actor_index, s_slot *slot, long index) { }

// @stub 0x1c1990
void __stdcall function_1c1990(long actor_index, s_slot *slot, long index) { }

// @stub 0x26e4b0
bool __stdcall function_26e4b0(long actor_index, s_slot *slot) { return 0; }

// @stub 0x26e600
void __stdcall function_26e600(long actor_index, s_slot *slot) { }

// @stub 0x26e650
void __stdcall function_26e650(long actor_index, s_slot *slot) { }

// @stub 0x26e6d0
void __stdcall function_26e6d0(long actor_index, s_slot *slot) { }

// @stub 0x26e710
void __stdcall function_26e710(long actor_index, s_slot *slot) { }

// @stub 0x26e750
bool __stdcall function_26e750(long actor_index, s_slot *slot) { return 0; }

// @stub 0x26e8a0
short __stdcall function_26e8a0(long actor_index, short level, bool active) { return 0; }


/* callbacks of the region, referenced by the handlers */

// @stub 0x1b0020
void __stdcall function_1b0020(long actor_index, s_slot *slot) { }

// @stub 0x1b0110
void __stdcall function_1b0110(long actor_index, s_slot *slot) { }

// @stub 0x1b0ab0
void __stdcall function_1b0ab0(long actor_index, s_slot *slot) { }

// @stub 0x1b13b0
void __stdcall function_1b13b0(long actor_index, s_slot *slot, long index) { }

// @stub 0x1b1f70
void __stdcall function_1b1f70(long actor_index, s_slot *slot) { }

// @stub 0x1b23c0
bool __stdcall function_1b23c0(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1b2630
short __stdcall function_1b2630(long actor_index, s_slot *slot, bool active) { return 0; }

// @stub 0x1b2770
void __stdcall function_1b2770(long actor_index, s_slot *slot) { }

// @stub 0x1b2bb0
void __stdcall function_1b2bb0(long actor_index, s_slot *slot) { }

// @stub 0x1b2e80
short __stdcall function_1b2e80(long actor_index, short level, bool active) { return 0; }

// @stub 0x1b3360
bool __stdcall function_1b3360(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1b3380
void __stdcall function_1b3380(long actor_index, s_slot *slot) { }

// @stub 0x1b36e0
bool __stdcall function_1b36e0(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1b3880
void __stdcall function_1b3880(long actor_index, s_slot *slot) { }

// @stub 0x1b3a80
bool __stdcall function_1b3a80(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1b3c60
void __stdcall function_1b3c60(long actor_index, s_slot *slot) { }

// @stub 0x1b3fd0
void __stdcall function_1b3fd0(long actor_index, s_slot *slot) { }

// @stub 0x1b4390
short __stdcall function_1b4390(long actor_index, short level, bool active) { return 0; }

// @stub 0x1b4560
short __stdcall function_1b4560(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1b4680
short __stdcall function_1b4680(long actor_index) { return 0; }

// @stub 0x1b47b0
void __stdcall function_1b47b0(long actor_index, s_slot *slot) { }

// @stub 0x1b4bd0
short __stdcall function_1b4bd0(long actor_index, s_slot *slot, bool active) { return 0; }

// @stub 0x1b4d90
void __stdcall function_1b4d90(long actor_index, s_slot *slot, long index) { }

// @stub 0x1b4fe0
short __stdcall function_1b4fe0(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1b51f0
short __stdcall function_1b51f0(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1b52a0
short __stdcall function_1b52a0(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1b53a0
short __stdcall function_1b53a0(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1b54d0
short __stdcall function_1b54d0(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1b5aa0
short __stdcall function_1b5aa0(long actor_index, s_slot *slot, bool active) { return 0; }

// @stub 0x1b5c00
bool __stdcall function_1b5c00(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1b5e00
short __stdcall function_1b5e00(long actor_index, short level, bool active) { return 0; }

// @stub 0x1b5f30
void __stdcall function_1b5f30(long actor_index, s_slot *slot) { }

// @stub 0x1b6120
void __stdcall function_1b6120(long actor_index, s_slot *slot) { }

// @stub 0x1b6450
short __stdcall function_1b6450(long actor_index, s_slot *slot, bool active) { return 0; }

// @stub 0x1b6740
void __stdcall function_1b6740(long actor_index, s_slot *slot, long a, long b) { }

// @stub 0x1b6930
bool __stdcall function_1b6930(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1b6c90
short __stdcall function_1b6c90(long actor_index, s_slot *slot, bool active) { return 0; }

// @stub 0x1b6d40
void __stdcall function_1b6d40(long actor_index, s_slot *slot) { }

// @stub 0x1b7000
short __stdcall function_1b7000(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1b7210
short __stdcall function_1b7210(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1b73b0
short __stdcall function_1b73b0(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1b7920
short __stdcall function_1b7920(long actor_index) { return 0; }

// @stub 0x1b7e40
short __stdcall function_1b7e40(long actor_index) { return 0; }

// @stub 0x1b8070
void __stdcall function_1b8070(long actor_index, s_slot *slot) { }

// @stub 0x1b81c0
short __stdcall function_1b81c0(long actor_index, s_slot *slot, bool active) { return 0; }

// @stub 0x1b83b0
void __stdcall function_1b83b0(long actor_index, s_slot *slot, long a, long b) { }

// @stub 0x1b85a0
void __stdcall function_1b85a0(long actor_index, s_slot *slot) { }

// @stub 0x1b89d0
void __stdcall function_1b89d0(long actor_index, s_slot *slot) { }

// @stub 0x1b8ae0
void __stdcall function_1b8ae0(long actor_index, s_slot *slot) { }

// @stub 0x1b9540
short __stdcall function_1b9540(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1b9640
short __stdcall function_1b9640(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1b9890
short __stdcall function_1b9890(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1b99d0
short __stdcall function_1b99d0(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1ba090
void __stdcall function_1ba090(long actor_index, s_slot *slot) { }

// @stub 0x1ba4e0
short __stdcall function_1ba4e0(long actor_index, s_slot *slot, bool active) { return 0; }

// @stub 0x1ba5c0
void __stdcall function_1ba5c0(long actor_index, s_slot *slot, long index) { }

// @stub 0x1bb3a0
void __stdcall function_1bb3a0(long actor_index, s_slot *slot, long a, long b) { }

// @stub 0x1bbf40
short __stdcall function_1bbf40(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1bc2a0
short __stdcall function_1bc2a0(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1bc6d0
short __stdcall function_1bc6d0(long actor_index) { return 0; }

// @stub 0x1bc850
short __stdcall function_1bc850(long actor_index, s_slot *slot, bool active) { return 0; }

// @stub 0x1bcab0
void __stdcall function_1bcab0(long actor_index, s_slot *slot) { }

// @stub 0x1bcd00
void __stdcall function_1bcd00(long actor_index, s_slot *slot) { }

// @stub 0x1bcee0
bool __stdcall function_1bcee0(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1bcfd0
void __stdcall function_1bcfd0(long actor_index, s_slot *slot) { }

// @stub 0x1bd230
bool __stdcall function_1bd230(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1bd890
void __stdcall function_1bd890(long actor_index, s_slot *slot) { }

// @stub 0x1bdad0
void __stdcall function_1bdad0(long actor_index, s_slot *slot, long index) { }

// @stub 0x1bde80
void __stdcall function_1bde80(long actor_index, s_slot *slot, long a, long b) { }

// @stub 0x1be120
void __stdcall function_1be120(long actor_index, s_slot *slot) { }

// @stub 0x1be4b0
short __stdcall function_1be4b0(long actor_index) { return 0; }

// @stub 0x1be6e0
short __stdcall function_1be6e0(long actor_index, s_slot *slot, bool active) { return 0; }

// @stub 0x1be840
void __stdcall function_1be840(long actor_index, s_slot *slot) { }

// @stub 0x1be8f0
void __stdcall function_1be8f0(long actor_index, s_slot *slot) { }

// @stub 0x1beb70
short __stdcall function_1beb70(long actor_index, s_slot *slot, bool active) { return 0; }

// @stub 0x1bee40
void __stdcall function_1bee40(long actor_index, s_slot *slot, long a, long b) { }

// @stub 0x1bf0f0
short __stdcall function_1bf0f0(long actor_index, s_slot *slot, bool active) { return 0; }

// @stub 0x1bf230
void __stdcall function_1bf230(long actor_index, s_slot *slot) { }

// @stub 0x1bf4e0
bool __stdcall function_1bf4e0(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1bf5c0
void __stdcall function_1bf5c0(long actor_index, s_slot *slot) { }
