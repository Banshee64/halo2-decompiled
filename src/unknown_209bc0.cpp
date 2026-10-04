// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_209BC0.CPP: the script globals' runtime values: refreshing an
   external global's value and reading a global (outside functions of lane I;
   retail calls them from the thread code, never inlines them) */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "hs.h"

/* a global's runtime value (g_4f9380, 8 bytes) */
struct s_hs_global_value
{
	byte unknown0[4];
	script_value value;
};

/* an external global's definition (g_473468 points at one per global) */
struct s_hs_external_global
{
	short type;
	byte unknown2[2];
	void *address;
};

extern s_record_pool *g_4f9380;
extern s_hs_external_global *g_473468[1];
extern char const *g_470010;

void function_20a2e0(short global_index);

// @retail 0x209bc0
long function_209bc0(short global_index)
{
	function_20a2e0(global_index);

	long index = global_index;

	return ((s_hs_global_value *)g_4f9380->data)[(index & 0x8000) ? (index & 0x7fff) : (index & 0x7fff) + 0x41d].value.d;
}

// @retail 0x20a2e0
void function_20a2e0(short global_index)
{
	if (global_index & 0x8000)
	{
		long index = global_index;
		long slot = index & 0x7fff;

		if (!(index & 0x8000))
		{
			slot += 0x41d;
		}

		s_hs_global_value *value = &((s_hs_global_value *)g_4f9380->data)[slot];
		s_hs_external_global *global = g_473468[global_index & 0x7fff];

		switch (global->type)
		{
		case _hs_type_boolean:
			value->value.b = global->address ? *(byte *)global->address : (byte)0;
			break;
		case _hs_type_real:
			value->value.r = global->address ? *(real *)global->address : 0.f;
			break;
		case _hs_type_short_integer:
			value->value.s = global->address ? *(short *)global->address : (short)0;
			break;
		case _hs_type_string:
			value->value.d = global->address ? *(dword *)global->address : (dword)g_470010;
			break;
		case _hs_type_long_integer:
			value->value.d = global->address ? *(dword *)global->address : 0;
			break;
		case _hs_type_script:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_string_id:
			value->value.d = global->address ? *(dword *)global->address : 0;
			break;
		case _hs_type_unit_seat_mapping:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_trigger_volume:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_cutscene_flag:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_cutscene_camera_point:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_cutscene_title:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_cutscene_recording:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_device_group:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_ai:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_ai_command_list:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_ai_command_script:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_ai_behavior:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_ai_orders:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_starting_profile:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_conversation:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_structure_bsp:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_navpoint:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_point_reference:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_style:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_hud_message:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_object_list:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_sound:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_effect:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_damage:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_looping_sound:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_animation_graph:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_damage_effect:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_object_definition:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_bitmap:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_shader:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_render_model:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_structure_definition:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_lightmap_definition:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_game_difficulty:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_team:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_actor_type:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_hud_corner:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_model_state:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_network_event:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		case _hs_type_object:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_unit:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_vehicle:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_weapon:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_device:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_scenery:
			value->value.d = global->address ? *(dword *)global->address : NONE;
			break;
		case _hs_type_object_name:
			value->value.s = global->address ? *(short *)global->address : (short)NONE;
			break;
		}
	}
}
