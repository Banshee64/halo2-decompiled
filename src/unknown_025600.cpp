// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_025600.CPP: showing an object's name with a caption (an outside
   function lane A's script evaluator 0x2a2f00 needs) */

#include "cseries.h"
#include "globals.h"
#include <string.h>

/* the object name caption (g_4b9970, defined in hs_library_external.cpp):
   the object, its name, a string id and how long to show it */
struct s_object_name_caption
{
	long object_index;
	char name[0x20];
	long string_handle;
	real seconds;
	byte unknown2c[0x30 - 0x2c];
};

extern long g_4b9970[12];
extern byte g_5093fc;

/* the scenario's object names (g_4e0350 +0x4c), 0x24 bytes each */
struct s_scenario_object_name
{
	char name[0x20];
	byte unknown20[4];
};

struct s_scenario_object_names_view
{
	byte unknown00[0x4c];
	s_scenario_object_name *object_names;
};

struct s_25600_object
{
	byte unknown00[0xac];
	short name_index;
};

struct s_25600_object_header
{
	byte unknown00[8];
	s_25600_object *object;
};

inline void object_name_caption_reset(void)
{
	memset(g_4b9970, 0, sizeof(g_4b9970));
	g_4b9970[0] = NONE;
}

// @retail 0x25600
void function_25600(long object_index, long string_handle, real seconds)
{
	s_scenario_object_names_view *scenario = (s_scenario_object_names_view *)g_4e0350;

	object_name_caption_reset();
	g_5093fc = false;

	if (scenario)
	{
		if (object_index != NONE)
		{
			s_object_name_caption *caption = (s_object_name_caption *)g_4b9970;
			s_25600_object *object = ((s_25600_object_header *)g_4e0300->data)[object_index & 0xffff].object;

			strncpy(caption->name, scenario->object_names[object->name_index].name, sizeof(caption->name));
			caption->name[sizeof(caption->name) - 1] = 0;
			caption->string_handle = string_handle;
			caption->object_index = object_index;
			if (seconds == 0.0f)
				caption->seconds = 0.0f;
			else
				caption->seconds = seconds < 10.0f ? 10.0f : (seconds > 160.0f ? 160.0f : seconds);
		}
		else
		{
			object_name_caption_reset();
		}
	}
}
