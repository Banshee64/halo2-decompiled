// @flags /O2 /Gr
/* DEVICE_LIGHT_FIXTURES.CPP: scenario placement for light fixtures.
   See docs/device_light_fixtures.md for the original-object mapping. */

#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include "real_math.h"

struct s_light_fixture_object_view
{
	byte unknown000[0x1cc];
	real_rgb_color color;
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
	real_rgb_color color;
	real intensity;
	real falloff_angle;
	real cutoff_angle;
};

// @retail 0x11bdc0
void __stdcall light_fixture_place(long object_index, s_scenario_light_fixture_view *placement)
{
	s_light_fixture_object_view *light_fixture =
		((s_light_fixture_header_view *)g_4e0300->data)[object_index & 0xffff].object;
	light_fixture->color = placement->color;
	light_fixture->intensity = placement->intensity;
	light_fixture->falloff_angle = placement->falloff_angle;
	light_fixture->cutoff_angle = placement->cutoff_angle;
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
	light_fixture_place
};
