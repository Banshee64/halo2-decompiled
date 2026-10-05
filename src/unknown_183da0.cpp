// @flags /O2 /arch:SSE /Gr /GL-
/* UNKNOWN_183DA0.CPP: two small havok.cpp methods that retail compiled
   without link-time code generation (this in ecx, the float returned on the
   x87 stack) */

#include "unknown_11c920.h"

struct s_node
{
	byte unknown00[0xc];
	s_node *next;
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
