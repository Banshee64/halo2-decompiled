// @flags /O1 /Oi /arch:SSE /Gr
/* INTERFACE.CPP: the interface game system (its entries sit in the game
   system table at 0x441500) and the interface tags of the globals tag */

#include "cseries.h"
#include "globals.h"
#include <string.h>

void hud_initialize(void);
void game_state_initialize_165cc3(void);
void function_22c033();
void function_19173e(void);
void function_13e8a0();
void first_person_weapons_initialize_for_new_map(void);
void function_165db0(void);
void function_13edb0(long font, long style, long justification, dword flags, real_argb_color const *color, real_argb_color const *shadow_color);

/* hs_library_external.cpp */
extern long *g_502248;

/* unknown_033a0b.cpp: the second global colour pointer */
struct s_33a0b_default;
extern s_33a0b_default *g_4686d4;

// @retail 0x13e5c7
void interface_initialize(void)
{
	hud_initialize();
	game_state_initialize_165cc3();
	function_22c033();
}

// @retail 0x13e5d6
void interface_initialize_for_new_map(void)
{
	function_19173e();
	function_13e8a0();
	first_person_weapons_initialize_for_new_map();
	memset(g_502248, 0, 5 * sizeof(long));
	function_13edb0(0, NONE, 0, 0, g_4686cc, (real_argb_color const *)g_4686d4);
}

// @retail 0x13e610
void interface_dispose_from_old_map(void)
{
	function_165db0();
}

struct s_interface_tag_reference
{
	dword group_tag;
	long index;
};

struct s_interface_globals_view
{
	byte unknown00[0x110];
	long interface_tag_count;
	s_interface_tag_reference *interface_tags;
};

// @retail 0x13e615
long interface_get_tag_index(short interface_tag_index)
{
	s_interface_globals_view *globals = (s_interface_globals_view *)g_4e034c;
	s_interface_tag_reference *references = globals->interface_tag_count ? globals->interface_tags : NULL;

	return references[interface_tag_index].index;
}
