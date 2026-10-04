// @flags /O2 /Ob1 /arch:SSE /Gr
/* SOUND_GET_CLASS.CPP: the sound class of a sound tag (part of retail's
   sound tag lookups, src/unknown_18d090.cpp, which inlines its own copy).
   Retail calls it out of line everywhere else, which this file's /Ob1
   reproduces. */

#include "cseries.h"
#include "globals.h"

#define SOUND_TAG 0x736e6421

struct s_tag_instance_view
{
	dword group_tag;
	byte unknown04[4];
	byte *data;
	byte unknown0c[4];
};

struct s_sound_class_definition
{
	byte unknown00[0x34];
};

struct s_sound_definition
{
	byte unknown00[0xd];
	char class_index;
};

static inline s_tag_instance_view *tag_instances(void)
{
	return (s_tag_instance_view *)g_4e3b44;
}

static inline byte *tag_get_data(long tag_index)
{
	return tag_instances()[tag_index & 0xffff].data;
}

// @retail 0x18d170
s_sound_class_definition *sound_get_class(long tag_index)
{
	s_sound_class_definition *result = NULL;

	if (tag_index != NONE && tag_instances()[(short)tag_index].group_tag == SOUND_TAG)
	{
		s_sound_definition *definition = (s_sound_definition *)tag_get_data(tag_index);

		if (definition->class_index == NONE)
		{
			result = NULL;
		}
		else
		{
			result = (s_sound_class_definition *)g_51ebd4->entries34 + definition->class_index;
		}
	}
	return result;
}
