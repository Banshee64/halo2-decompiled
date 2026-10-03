// @flags /O2 /Gr
/* UNKNOWN_1EB1B0.CPP: the shape blocks of a physics model */

#include "cseries.h"
#include "impacts.h"

// @retail 0x1eb1b0
s_impact_tag_block *physics_model_shape_block_get(
	byte *physics_model,
	s_physics_model_shape_key const *key,
	long *element_size)
{
	s_impact_tag_block *block;

	switch (key->type)
	{
	case 0:
		block = (s_impact_tag_block *)(physics_model + 0xb0);
		*element_size = 0x78;
		break;
	case 1:
		block = (s_impact_tag_block *)(physics_model + 0xe8);
		*element_size = 0x84;
		break;
	case 2:
		block = (s_impact_tag_block *)(physics_model + 0xb8);
		*element_size = 0x94;
		break;
	case 3:
		block = (s_impact_tag_block *)(physics_model + 0xf8);
		*element_size = 0x7c;
		break;
	case 4:
		block = (s_impact_tag_block *)(physics_model + 0xf0);
		*element_size = 0x78;
		break;
	case 5:
		block = (s_impact_tag_block *)(physics_model + 0x100);
		*element_size = 0x84;
		break;
	default:
		__assume(0);
	}
	return block;
}
