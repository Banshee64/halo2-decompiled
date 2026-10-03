// @flags /O2 /Ob1 /arch:SSE /Gr
/* UNKNOWN_2483F0.CPP: the particle locations (g_51ec8c) and the particle
   emitters chained to each (g_51ec88) */

#include "cseries.h"
#include "real_math.h"
#include "data_array.h"
#include "globals.h"

extern s_data_array *g_51ec84;
extern s_data_array *g_51ec88;
extern s_data_array *g_51ec8c;

/* the particle emitters (0x4c bytes each): the particles each one owns
   (g_51ec84) are chained from it */
struct s_particle_emitter_datum
{
	short salt;
	word particle_count;
	long first_particle_index;
	long next_index;
	real unknown0c;
	byte unknown10[0x4c - 0x10];
};

/* the particle locations (0x34 bytes each) */
struct s_particle_location_datum
{
	short salt;
	byte unknown02;
	byte random;
	long first_emitter_index;
	long last_emitter_index;
	long next_index;
	real_point3d position;
	real unknown1c;
	real_vector3d vector;
	real unknown2c;
	dword unknown30;
};

dword g_4ba034;

/* the particles (0x40 bytes each) */
struct s_particle_datum
{
	short salt;
	word unknown02;
	long next_index;
	byte unknown08[0x40 - 0x08];
};

/* deletes the particles of an emitter */
// @retail 0x2483b0
void function_2483b0(s_particle_emitter_datum *emitter)
{
	long index = emitter->first_particle_index;
	if (index != NONE)
	{
		long next;
		do
		{
			next = ((s_particle_datum *)g_51ec84->data)[index & 0xffff].next_index;
			datum_delete(g_51ec84, index);
			index = next;
		} while (next != NONE);
	}
	emitter->particle_count = 0;
	emitter->first_particle_index = NONE;
}

/* appends an emitter to a chain */
// @retail 0x2483f0
void function_2483f0(s_particle_emitter_datum *emitter, long *first_index, long *last_index)
{
	s_data_array *data = g_51ec88;
	long emitter_index = (emitter->salt << 16) | (emitter - (s_particle_emitter_datum *)data->data);

	emitter->next_index = NONE;
	if (*first_index == NONE)
		*first_index = emitter_index;
	if (*last_index != NONE)
		((s_particle_emitter_datum *)data->data)[*last_index & 0xffff].next_index = emitter_index;
	*last_index = emitter_index;
}

/* appends a particle location to a chain */
// @retail 0x248d90
void function_248d90(s_particle_location_datum *particle_location, long *first_index, long *last_index)
{
	s_data_array *data = g_51ec8c;
	long location_index = (particle_location->salt << 16) | (particle_location - (s_particle_location_datum *)data->data);

	particle_location->next_index = NONE;
	if (*first_index == NONE)
		*first_index = location_index;
	if (*last_index != NONE)
		((s_particle_location_datum *)data->data)[*last_index & 0xffff].next_index = location_index;
	*last_index = location_index;
}

/* a new particle location at the origin */
// @retail 0x248620
long function_248620()
{
	long location_index = datum_new(g_51ec8c);

	if (location_index != NONE)
	{
		s_particle_location_datum *particle_location = &((s_particle_location_datum *)g_51ec8c->data)[location_index & 0xffff];

		particle_location->first_emitter_index = NONE;
		particle_location->last_emitter_index = NONE;
		particle_location->next_index = NONE;
		particle_location->position = *g_468788;
		*(real_point3d *)&particle_location->vector = *g_468788;
		particle_location->unknown2c = 0.5f;
		particle_location->unknown1c = 1.0f;
		particle_location->unknown30 = g_4ba034;
		particle_location->random = (byte)random_index(&g_4e7408->seed, 0xff);
	}

	return location_index;
}

/* deletes a particle location with its emitters */
// @retail 0x2486e0
void __stdcall function_2486e0(long particle_location_index)
{
	s_particle_location_datum *particle_location = &((s_particle_location_datum *)g_51ec8c->data)[particle_location_index & 0xffff];
	long emitter_index = particle_location->first_emitter_index;

	if (emitter_index != NONE)
	{
		s_data_array *data = g_51ec88;
		long next_index;

		do
		{
			next_index = ((s_particle_emitter_datum *)data->data)[emitter_index & 0xffff].next_index;
			function_2483b0(&((s_particle_emitter_datum *)data->data)[emitter_index & 0xffff]);
			datum_delete(data, emitter_index);
			emitter_index = next_index;
		} while (next_index != NONE);
	}

	datum_delete(g_51ec8c, particle_location_index);
}

/* the particles of a location's emitters */
// @retail 0x248d50
long function_248d50(s_particle_location_datum *particle_location)
{
	long count = 0;
	long emitter_index = particle_location->first_emitter_index;

	if (emitter_index != NONE)
	{
		s_particle_emitter_datum *emitters = (s_particle_emitter_datum *)g_51ec88->data;

		do
		{
			s_particle_emitter_datum *emitter = &emitters[emitter_index & 0xffff];

			count += emitter->particle_count;
			emitter_index = emitter->next_index;
		} while (emitter_index != NONE);
	}

	return count;
}
