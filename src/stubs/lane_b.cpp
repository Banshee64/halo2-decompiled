// stubs for the callees of lane B (0x1b0000..0x1bffff) not decompiled yet
#include "cseries.h"
#include "slot_handler.h"

/* outside the region: functions the region's functions call */

struct s_object_seat;
struct s_object_child_iterator;

struct s_262b40_result;

struct s_261d20_entry;
struct s_2605d0_request;

// @stub 0x261d20
short __stdcall function_261d20(long actor_index, s_261d20_entry *entries, long maximum_count, s_2605d0_request const *request) { return 0; }

// @stub 0x261510
void __stdcall function_261510(long actor_index, s_2605d0_request const *request) { }

// @stub 0x260670
s_reference __stdcall function_260670(long actor_index, s_2605d0_request const *request, s_261d20_entry *entries, short count, long unknown, long unknown2, byte *scratch, bool *unknown3) { s_reference r = {0, 0}; return r; }

// @stub 0x1f4280
void __stdcall function_1f4280(long actor_index) { }

// @stub 0x25ab50
bool function_25ab50(long reference) { return 0; }

// @stub 0x110ab0
bool __stdcall function_110ab0(long unit_index) { return 0; }

// @stub 0x2628f0
void __stdcall function_2628f0(long actor_index, s_reference reference) { }

struct s_squad_iterator;

struct s_location_view;

// @stub 0x258b20
void function_258b20(long index, long actor_index) { }

// @stub 0x267770
void function_267770(long prop_index, long actor_index) { }

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

// @stub 0x26fc80
bool function_26fc80(long actor_index, long object_index, real distance, void *path) { return 0; }

// @stub 0x26c180
void function_26c180(long actor_index) { }

// @stub 0xe6900
bool function_e6900(long unit_index, s_unit_request *request) { return 0; }

/* outside the region: callbacks */

// @stub 0x1a79e0
short __stdcall function_1a79e0(long actor_index, short level, bool active) { return 0; }

// @stub 0x1afde0
short __stdcall function_1afde0(long actor_index) { return 0; }

// @stub 0x1afe50
bool __stdcall function_1afe50(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1aff10
void __stdcall function_1aff10(long actor_index, s_slot *slot) { }





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


// @stub 0x1b47b0
void __stdcall function_1b47b0(long actor_index, s_slot *slot) { }

// @stub 0x1b4bd0
short __stdcall function_1b4bd0(long actor_index, s_slot *slot, bool active) { return 0; }

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

// @stub 0x1b6120
void __stdcall function_1b6120(long actor_index, s_slot *slot) { }

// @stub 0x1b6450
short __stdcall function_1b6450(long actor_index, s_slot *slot, bool active) { return 0; }


// @stub 0x1b6c90
short __stdcall function_1b6c90(long actor_index, s_slot *slot, bool active) { return 0; }

// @stub 0x1b7210
short __stdcall function_1b7210(long actor_index, s_slot *slot) { return 0; }


// @stub 0x1b7e40
short __stdcall function_1b7e40(long actor_index) { return 0; }

// @stub 0x1b8070
void __stdcall function_1b8070(long actor_index, s_slot *slot) { }

// @stub 0x1b81c0
short __stdcall function_1b81c0(long actor_index, s_slot *slot, bool active) { return 0; }

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

// @stub 0x1ba5c0
void __stdcall function_1ba5c0(long actor_index, s_slot *slot, long index) { }

// @stub 0x1bb3a0
void __stdcall function_1bb3a0(long actor_index, s_slot *slot, long a, long b) { }

// @stub 0x1bbf40
short __stdcall function_1bbf40(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1bc2a0
short __stdcall function_1bc2a0(long actor_index, s_slot *slot) { return 0; }


// @stub 0x1bc850
short __stdcall function_1bc850(long actor_index, s_slot *slot, bool active) { return 0; }

// @stub 0x1bcab0
void __stdcall function_1bcab0(long actor_index, s_slot *slot) { }

// @stub 0x1bcd00
void __stdcall function_1bcd00(long actor_index, s_slot *slot) { }

// @stub 0x1bcfd0
void __stdcall function_1bcfd0(long actor_index, s_slot *slot) { }


// @stub 0x1bdad0
void __stdcall function_1bdad0(long actor_index, s_slot *slot, long index) { }


// @stub 0x1be120
void __stdcall function_1be120(long actor_index, s_slot *slot) { }

// @stub 0x1be4b0
short __stdcall function_1be4b0(long actor_index) { return 0; }

// @stub 0x1be6e0
short __stdcall function_1be6e0(long actor_index, s_slot *slot, bool active) { return 0; }

// @stub 0x1be8f0
void __stdcall function_1be8f0(long actor_index, s_slot *slot) { }

// @stub 0x1beb70
short __stdcall function_1beb70(long actor_index, s_slot *slot, bool active) { return 0; }


// @stub 0x1bf0f0
short __stdcall function_1bf0f0(long actor_index, s_slot *slot, bool active) { return 0; }

// @stub 0x1bf230
void __stdcall function_1bf230(long actor_index, s_slot *slot) { }

// @stub 0x1bf5c0
void __stdcall function_1bf5c0(long actor_index, s_slot *slot) { }

struct s_1fb7e0_data;
struct s_1fbac0_event;

// @stub 0x20ba60
bool __stdcall function_20ba60(short type, long unit_index, long target_index, long unknown, long unknown2, s_1fb7e0_data const *data) { return 0; }

// @stub 0x1fbac0
void __stdcall function_1fbac0(long unknown, long unit_index, bool unknown2, long unknown3, s_1fbac0_event *event) { }

// @stub 0x1f46f0
bool __stdcall function_1f46f0(long actor_index, short type, s_reference reference, byte *scratch, bool unknown2) { return 0; }

// @stub 0x1cb920
bool function_1cb920(void *data, long label) { return 0; }

// @stub 0x1f8a70
bool __stdcall function_1f8a70(long actor_index, long unknown) { return 0; }

// @stub 0x26e030
s_262b40_result *__stdcall function_26e030(s_reference reference) { return 0; }

// @stub 0x1caa40
void function_1caa40(long object_index, real_point3d *position) { }

// @stub 0x262590
bool function_262590(long actor_index, s_reference reference, bool unknown) { return 0; }

// @stub 0x29d6c0
bool function_29d6c0(real_vector3d *vector, s_reference reference) { return 0; }

// @stub 0x1b79d0
bool __stdcall function_1b79d0(long actor_index, s_slot *slot) { return 0; }

// @stub 0x1b7cc0
void __stdcall function_1b7cc0(long actor_index, s_slot *slot) { }