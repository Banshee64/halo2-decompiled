#include "unknown_11c920.h"
#include "effects.h"

// @flags /O2 /Ob1 /arch:SSE /Gr

struct s_object_2470e0;
struct s_frame_2477b0;

void function_2477b0(s_object_2470e0 *object, s_frame_2477b0 *frame, transform4x3f const *source);

struct s_location_248c60
{
	short salt;
	byte flags;
	byte random;
	long first_emitter_index;
};

struct s_definition_248c60
{
	byte unknown00[0x30];
	long count;
	byte *objects;
};

struct s_emitter_248c60
{
	byte unknown00[8];
	long next_index;
	byte unknown0c[0x4c - 0xc];
};

/* Updates the transforms of a location's chained particle emitters. */
// @retail 0x248c60
void function_248c60(s_particle_location_datum *particle_location, s_particle_system_datum *particle_system,
	transform4x3f const *matrix, bool field_b4)
{
	s_location_248c60 *location = (s_location_248c60 *)particle_location;
	long emitter_index = location->first_emitter_index;
	s_definition_248c60 *definition = (s_definition_248c60 *)particle_system->function_1751d0();
	long index = 0;
	location->flags = field_b4;
	for (; emitter_index != NONE && index < definition->count; index++)
	{
		s_emitter_248c60 *emitter = &((s_emitter_248c60 *)g_51ec88->data)[emitter_index & 0xffff];
		function_2477b0((s_object_2470e0 *)(definition->objects + index * 0xb8),
			(s_frame_2477b0 *)emitter, matrix);
		emitter_index = emitter->next_index;
	}
}
