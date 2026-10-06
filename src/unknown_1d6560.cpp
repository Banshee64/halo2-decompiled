// @flags /O2 /arch:SSE /Gr
/* UNKNOWN_1D6560.CPP: per-object physics setup: which entries of an object's
   physics model hold (judged by its damage sections), and the layer each of
   its rigid bodies collides in */

#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_1cec30.h"

/* a damage section an entry depends on (0xc bytes) */
struct s_physics_entry_section
{
	short key_a;
	short key_b;
	byte flags;
	byte unknown05[0xc - 5];
};

/* an entry of the physics model (0x18 bytes) */
struct s_physics_entry
{
	byte unknown00[8];
	long section_count;
	s_physics_entry_section *sections;
	byte unknown10[0x18 - 0x10];
};

/* a rigid body of the physics model (0x90 bytes) */
struct s_physics_rigid_body
{
	byte unknown00[0x18];
	byte flags;
	byte unknown19[0x90 - 0x19];
};

struct s_physics_model
{
	byte unknown00[0x30];
	long entry_count;
	s_physics_entry *entries;
	byte unknown38[4];
	s_physics_rigid_body *rigid_bodies;
};

struct s_physics_model_owner
{
	byte unknown00[0x48];
	s_physics_model *model;
};

struct s_physics_object_header
{
	short identifier;
	byte flags;
	byte type;
	byte unknown04[4];
	byte *object;
};

bool function_db540(long object_index, long key_a, long key_b);

/* sets the bit of each entry that has no sections, or one of whose flagged
   sections is gone; clears the others */
// @retail 0x1d6560
void __stdcall function_1d6560(s_havok_component *component, s_physics_model_owner *owner, dword *mask)
{
	long i;

	for (i = 0; i < owner->model->entry_count; i++)
	{
		s_physics_entry *entry = &owner->model->entries[i];

		mask[i >> 5] &= ~(1 << (i & 31));
		if (entry->section_count == 0)
		{
			mask[i >> 5] |= 1 << (i & 31);
		}
		else
		{
			long j;

			for (j = 0; j < entry->section_count; j++)
			{
				s_physics_entry_section *section = &entry->sections[j];

				if ((section->flags & 1) && !function_db540(component->object_index, section->key_a, section->key_b))
				{
					mask[i >> 5] |= 1 << (i & 31);
					break;
				}
			}
		}
	}
}

/* the collision layer of one of the object's rigid bodies, by object type */
// @retail 0x1d69d0
long function_1d69d0(s_havok_component *component, long kind, s_physics_model_owner *owner, long level, long rigid_body_index)
{
	s_physics_object_header *header = &((s_physics_object_header *)g_4e0300->data)[component->object_index & 0xffff];
	long type = header->type;
	s_physics_rigid_body *rigid_body = &owner->model->rigid_bodies[rigid_body_index];
	long result;

	if (kind == 7)
	{
		switch (type)
		{
		case 1:
			result = 0xc;
			break;
		default:
			result = 1;
			break;
		}
		return result;
	}
	switch (type)
	{
	case 0:
		result = (bool)(((dword)header->object[0x10a] >> 2) & 1) ? 11 : 9;
		break;
	case 1:
		result = (rigid_body->flags & 8) ? 14 : 12;
		break;
	case 7:
		if (kind == 6)
		{
			result = 8;
		}
		else
		{
			result = (rigid_body->flags & 8) ? 7 : 6;
		}
		break;
	case 11:
		if (level <= 1)
		{
			result = 4;
		}
		else
		{
			result = level >= 5 ? 5 : 3;
		}
		break;
	default:
		__assume(0);
	}
	return result;
}

struct s_physics_graph_body
{
    byte unknown00[0x18];
    byte flags;
    byte unknown19[3];
    short replacement;
    byte unknown1e[0x90 - 0x1e];
};

struct s_physics_graph_entry
{
    byte unknown00[4];
    short groups[2];
    long section_count;
    s_physics_entry_section *sections;
    byte unknown10[8];
};

struct s_physics_graph_model
{
    byte unknown00[0x30];
    long entry_count;
    s_physics_graph_entry *entries;
    long body_count;
    s_physics_graph_body *bodies;
};

struct s_physics_body_group
{
    char bodies[64];
    long count;
};

struct s_physics_body_groups
{
    long unknown00;
    s_physics_body_group groups[257];
};

bool function_db4c0(long object_index, long key_a, long key_b);

// @retail 0x1d6630
void function_1d6630(s_havok_component *component, s_physics_model_owner *owner, char const *kinds,
    dword const *entry_mask, s_physics_body_groups *groups, void *context, long parent, long current,
    char *output, long *output_count, void *other_context, dword *visited, bool *active)
{
    (void)&owner;
    (void)&kinds;
    (void)&entry_mask;
    (void)&groups;
    (void)&context;
    (void)&parent;
    (void)&current;
    (void)&output;
    (void)&output_count;
    (void)&other_context;
    (void)&visited;
    (void)&active;
    bool replace = !(bool)((component->unknown04 >> 13) & 1);
    if (groups->groups[current == NONE ? 256 : current].count > 0)
    {
        for (long i = 0; i < groups->groups[current == NONE ? 256 : current].count; ++i)
        {
            long body_index = groups->groups[current == NONE ? 256 : current].bodies[i];
            s_physics_graph_model *model = (s_physics_graph_model *)owner->model;
            s_physics_graph_body *body = &model->bodies[body_index];
            bool replaced = false;
            bool available = false;
            if (body->flags & 0x20)
            {
                long replacement = body->replacement;
                long bounded = replacement < 0 ? 0 : replacement > model->body_count - 1 ? model->body_count - 1 : replacement;
                if (bounded == replacement)
                {
                    available = true;
                    if (replace)
                    {
                        replaced = true;
                        body_index = replacement;
                    }
                }
            }
            output[(*output_count)++] = (char)body_index;
            if (replaced)
                component->unknown04 |= 0x100000;
            if (available)
                component->unknown04 |= 0x80000;
        }
        if (current != NONE)
            visited[current >> 5] |= 1 << (current & 31);
        for (long entry_index = 0; entry_index < owner->model->entry_count; ++entry_index)
        {
            s_physics_graph_entry *entry = &((s_physics_graph_model *)owner->model)->entries[entry_index];
            if ((entry_mask[entry_index >> 5] & (1 << (entry_index & 31))) &&
                (entry->groups[0] == current || entry->groups[1] == current))
            {
                long next = entry->groups[0] == current ? entry->groups[1] : entry->groups[0];
                long first_body = NONE;
                if (next != NONE && groups->groups[next].count > 0)
                    first_body = groups->groups[next].bodies[0];
                if (first_body != NONE && (kinds[first_body] == 6 || (visited[next >> 5] & (1 << (next & 31)))))
                    continue;
                if (next == NONE)
                {
                    for (long j = 0; j < entry->section_count; ++j)
                    {
                        s_physics_entry_section *section = &entry->sections[j];
                        if (!function_db540(component->object_index, section->key_a, section->key_b) &&
                            !function_db4c0(component->object_index, section->key_a, section->key_b))
                        {
                            *active = true;
                            break;
                        }
                    }
                }
                else
                {
                    if (groups->groups[next].count > 0)
                    {
                        if (entry->section_count == 0)
                        {
                            long this_body = groups->groups[current == NONE ? 256 : current].bodies[0];
                            long next_body = groups->groups[next].bodies[0];
                            if (kinds[this_body] == 7 || kinds[next_body] == 7)
                                continue;
                        }
                        else
                        {
                            long j;
                            for (j = 0; j < entry->section_count; ++j)
                            {
                                s_physics_entry_section *section = &entry->sections[j];
                                if (!function_db540(component->object_index, section->key_a, section->key_b) &&
                                    !function_db4c0(component->object_index, section->key_a, section->key_b))
                                    break;
                            }
                            if (j >= entry->section_count)
                                continue;
                        }
                    }
                    if (first_body != NONE)
                        function_1d6630(component, owner, kinds, entry_mask, groups, context, current, next,
                            output, output_count, other_context, visited, active);
                }
            }
        }
    }
}
