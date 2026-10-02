/* HS_LIBRARY_EXTERNAL.H: what the script functions' evaluators share */

#ifndef HS_LIBRARY_EXTERNAL_H
#define HS_LIBRARY_EXTERNAL_H

#include "cseries.h"
#include "hs.h"

enum
{
	k_maximum_hs_function_parameters = 8
};

enum e_hs_type
{
	_hs_type_unparsed = 0,
	_hs_type_special_form,
	_hs_type_function_name,
	_hs_type_passthrough,
	_hs_type_void,
	_hs_type_boolean,
	_hs_type_real,
	_hs_type_short_integer,
	_hs_type_long_integer,
	_hs_type_string,
	_hs_type_script,
	_hs_type_string_id,
	_hs_type_unit_seat_mapping,
	_hs_type_trigger_volume,
	_hs_type_cutscene_flag,
	_hs_type_cutscene_camera_point,
	_hs_type_cutscene_title,
	_hs_type_cutscene_recording,
	_hs_type_device_group,
	_hs_type_ai,
	_hs_type_ai_command_list,
	_hs_type_ai_command_script,
	_hs_type_ai_behavior,
	_hs_type_ai_orders,
	_hs_type_starting_profile,
	_hs_type_conversation,
	_hs_type_structure_bsp,
	_hs_type_navpoint,
	_hs_type_point_reference,
	_hs_type_style,
	_hs_type_hud_message,
	_hs_type_object_list,
	_hs_type_sound,
	_hs_type_effect,
	_hs_type_damage,
	_hs_type_looping_sound,
	_hs_type_animation_graph,
	_hs_type_damage_effect,
	_hs_type_object_definition,
	_hs_type_bitmap,
	_hs_type_shader,
	_hs_type_render_model,
	_hs_type_structure_definition,
	_hs_type_lightmap_definition,
	_hs_type_game_difficulty,
	_hs_type_team,
	_hs_type_actor_type,
	_hs_type_hud_corner,
	_hs_type_model_state,
	_hs_type_network_event,
	_hs_type_object,
	_hs_type_unit,
	_hs_type_vehicle,
	_hs_type_weapon,
	_hs_type_device,
	_hs_type_scenery,
	_hs_type_object_name,
	_hs_type_unit_name,
	_hs_type_vehicle_name,
	_hs_type_weapon_name,
	_hs_type_device_name,
	_hs_type_scenery_name
};

typedef void (__stdcall *hs_evaluate_proc)(short function_index, long thread_index, bool initialize);

/* a script function's definition (.rdata 0x44b098 onwards; retail sizes
   each one to its parameters); the function table g_4744e0 points at one
   per script function */
struct hs_function_definition
{
	short return_type;
	word flags;
	hs_evaluate_proc evaluate;
	void const *parse;
	short parameter_count;
	short parameter_types[k_maximum_hs_function_parameters];
};

/* the function table, defined (with its own view of the entries) beside
   hs_return in unknown_209ae0.cpp */
struct s_type_header;
extern s_type_header *g_4744e0[];

inline hs_function_definition *hs_function_get(short function_index)
{
	return (hs_function_definition *)g_4744e0[function_index];
}

/* hs_return: stores a script function's value in the calling frame */
void function_209ae0(long thread_index, long value);

/* evaluates a script function's arguments one per call; returns the
   arguments once they are all evaluated, NULL until then */
long *__stdcall hs_macro_function_evaluate(long thread_index, short parameter_count, short const *parameter_types, bool initialize);

#endif
