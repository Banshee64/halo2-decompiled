// @flags /O2 /arch:SSE /Gr
#include "cseries.h"
#include "crc.h"
#include "globals.h"
#include <string.h>

struct s_object
{
	long definition_index;
	byte unknown04[0xb0];
	long manager_index;
};

struct s_object_header
{
	byte unknown00[8];
	s_object *object;
};

struct s_object_definition_data
{
	byte unknown00[0x38];
	long model_index;
};

struct s_model_data
{
	byte unknown00[0x24];
	long graph_index;
};

struct s_graph_record
{
	short value;
	byte unknown02[0x8e];
};

struct s_graph_data
{
	byte unknown00[0x3c];
	s_graph_record *records;
};

struct s_manager_node
{
	byte unknown00[0x48];
	signed char *record_indices;
	long record_index_count;
	byte unknown50[0x10];
};

struct s_manager_entry
{
	byte unknown00[0x70];
	s_manager_node *nodes;
	long node_count;
	byte unknown78[0x28];
};

struct s_manager_globals
{
	byte unknown00[0x44];
	s_manager_entry *entries;
};

s_manager_globals *g_51e9b8;

// Havok: a closest-points query against an object's phantoms (inlined
// hkCollisionDispatcher code, hkInplaceArray and hkClosestPointsCollector
// vtables, FPU/SSE control word handling). Out of scope: left as a placeholder.
// @retail 0x183910
bool function_183910(
	long object_index,
	real_point3d const *point,
	real_point3d *out_point,
	real_point3d *out_normal)
{
	return false;
}

real const k_real_zero = 0.0f;

struct s_node
{
	byte unknown00[0xc];
	s_node *next;
	s_node *get_next() const;
	s_node *get_last();
};

struct s_scale_definition
{
	byte unknown00[0x2c];
	real scale;
};

struct s_scale_owner
{
	byte unknown00[0x3c];
	s_scale_definition *definition;
	real get_inverse_scale() const;
};

struct s_scale_holder
{
	byte unknown00[0x2c];
	real scale;
	real get_inverse_scale() const;
};

struct s_flag_byte
{
	byte value;
	bool is_set() const;
	s_flag_byte *set(byte new_value);
};

// @retail 0x183d90
s_node *s_node::get_next() const
{
	return next;
}

// @retail 0x183da0
s_node *s_node::get_last()
{
	s_node *node = this;

	while (node->next)
	{
		node = node->next;
	}

	return node;
}

// @retail 0x183dc0
real s_scale_owner::get_inverse_scale() const
{
	real scale = definition->scale;

	return (scale != 0.0f) ? 1.0f / scale : 0.0f;
}

// @retail 0x183df0
real s_scale_holder::get_inverse_scale() const
{
	real value = scale;

	if (value == k_real_zero)
	{
		return k_real_zero;
	}

	return 1.0f / value;
}

// @retail 0x183e20
bool s_flag_byte::is_set() const
{
	return value != 0;
}

// @retail 0x183e30
s_flag_byte *s_flag_byte::set(byte new_value)
{
	value = new_value;
	return this;
}

byte *g_4e6080;
long g_4e6084;
dword g_4e608c;
byte *g_4ed280;
s_node *(s_node::*g_fn_183d90)() const = &s_node::get_next;
s_node *(s_node::*g_fn_183da0)() = &s_node::get_last;
real (s_scale_owner::*g_fn_183dc0)() const = &s_scale_owner::get_inverse_scale;
real (s_scale_holder::*g_fn_183df0)() const = &s_scale_holder::get_inverse_scale;
s_flag_byte *(s_flag_byte::*g_fn_183e30)(byte) = &s_flag_byte::set;
long g_4ea95c;
long g_4eca60[8];

// @retail 0x183e40
void function_183e40()
{
	long size = 0x4204;
	byte *base = g_4e6080 + g_4e6084;

	g_4e6084 += 0x4204;
	crc_checksum_buffer(&g_4e608c, &size, 4);
	g_4ed280 = base;
	g_4ea95c = 0;
	for (long i = 0; i < 8; i++)
	{
		g_4eca60[i] = NONE;
	}
}

// @retail 0x183ec0
void function_183ec0()
{
	memset(g_4ed280, 0xff, 0x4201);
}

// @retail 0x183c60
long function_183c60(
	long object_index,
	long maximum_value)
{
	long best_value = NONE;
	long best_node = NONE;

	if (maximum_value != NONE)
	{
		s_object *object = g_4e0300->headers[object_index & 0xFFFF].object;
		s_object_definition_data *definition = (s_object_definition_data *)g_4e3b44[object->definition_index & 0xFFFF].data;
		s_model_data *model = (s_model_data *)g_4e3b44[definition->model_index & 0xFFFF].data;
		s_graph_data *graph = (s_graph_data *)g_4e3b44[model->graph_index & 0xFFFF].data;
		s_manager_entry *entry = &g_51e9b8->entries[object->manager_index & 0xFFFF];

		for (long node_index = 0; node_index < entry->node_count; node_index++)
		{
			s_manager_node *node = &entry->nodes[node_index];

			for (long i = 0; i < node->record_index_count; i++)
			{
				s_graph_record *record = &graph->records[node->record_indices[i]];

				if (record->value != NONE && (best_value == NONE || (record->value <= maximum_value && record->value > best_value)))
				{
					best_value = record->value;
					best_node = node_index;
				}
			}
		}
	}

	return best_node;
}
