// @flags /O2 /Gr
/* UNKNOWN_11BDC0.CPP: scenario placement for light fixtures.
   See docs/unknown_11bdc0.md for the original-object mapping. */

#include "unknown_11c920.h"
#include "globals.h"
#include "data_array.h"
#include "unknown_0259d0.h"

struct s_light_fixture_object_view
{
	byte unknown000[0x1cc];
	color3f color;
	real intensity;
	real falloff_angle;
	real cutoff_angle;
};

struct s_light_fixture_header_view
{
	byte unknown00[8];
	s_light_fixture_object_view *object;
};

struct s_scenario_light_fixture_view
{
	byte unknown00[0x3c];
	color3f color;
	real intensity;
	real falloff_angle;
	real cutoff_angle;
};

// @retail 0x11bdc0
void __stdcall function_11bdc0(long object_index, s_scenario_light_fixture_view *placement)
{
	s_light_fixture_object_view *local_d958a2 =
		((s_light_fixture_header_view *)g_4e0300->data)[object_index & 0xffff].object;
	local_d958a2->color = placement->color;
	local_d958a2->intensity = placement->intensity;
	local_d958a2->falloff_angle = placement->falloff_angle;
	local_d958a2->cutoff_angle = placement->cutoff_angle;
}

/* Actual light fixture type-definition prefix at 0x4684a0, through
   placement. Later fields and parent types are outside this view. */
struct s_light_fixture_type_definition_view
{
	char const *name;
	dword group_tag;
	short datum_size;
	short unknown0a;
	short unknown0c;
	short unknown0e;
	void *unknown10[7];
	void *create;
	void (__stdcall *place)(long, s_scenario_light_fixture_view *);
};

s_light_fixture_type_definition_view g_4684a0 =
{
	"light_fixture", 'lifi', 0x1e4, 0xc8, 0xd0, 0x54,
	{ NULL, NULL, NULL, NULL, NULL, NULL, NULL },
	NULL,
	function_11bdc0
};
