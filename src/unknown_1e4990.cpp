// @flags /O2 /Gr
#include "cseries.h"
#include "globals.h"
#include "slot_handler.h"
#include "unknown_2729b0.h"


// @retail 0x1e4990
long function_1e4990(long index)
{
	long result = 0;
	if (index != NONE)
	{
		do
		{
			byte *data = g_4e3b44[index & 0xffff].bytes;
			if (*(long *)(data + 0x34) > 0)
			{
				result = *(long *)(data + 0x38);
				break;
			}
			index = *(long *)(data + 8);
		} while (index != NONE);
	}
	return result;
}

// @retail 0x1e49d0
long function_1e49d0(long index)
{
	long result = 0;
	if (index != NONE)
	{
		do
		{
			byte *data = g_4e3b44[index & 0xffff].bytes;
			if (*(long *)(data + 0x44) > 0)
			{
				result = *(long *)(data + 0x48);
				break;
			}
			index = *(long *)(data + 8);
		} while (index != NONE);
	}
	return result;
}

// @retail 0x1e4a10
long function_1e4a10(long index)
{
	long result = 0;
	if (index != NONE)
	{
		do
		{
			byte *data = g_4e3b44[index & 0xffff].bytes;
			if (*(long *)(data + 0x3c) > 0)
			{
				result = *(long *)(data + 0x40);
				break;
			}
			index = *(long *)(data + 8);
		} while (index != NONE);
	}
	return result;
}

// @retail 0x1e4a50
long function_1e4a50(long index)
{
	long result = 0;
	if (index != NONE)
	{
		do
		{
			byte *data = g_4e3b44[index & 0xffff].bytes;
			if (*(long *)(data + 0x5c) > 0)
			{
				result = *(long *)(data + 0x60);
				break;
			}
			index = *(long *)(data + 8);
		} while (index != NONE);
	}
	return result;
}

// @retail 0x1e4a90
long function_1e4a90(long index)
{
	long result = 0;
	if (index != NONE)
	{
		do
		{
			byte *data = g_4e3b44[index & 0xffff].bytes;
			if (*(long *)(data + 0xb4) > 0)
			{
				result = *(long *)(data + 0xb8);
				break;
			}
			index = *(long *)(data + 8);
		} while (index != NONE);
	}
	return result;
}

// @retail 0x1e4ad0
long function_1e4ad0(long index)
{
	long result = 0;
	if (index != NONE)
	{
		do
		{
			byte *data = g_4e3b44[index & 0xffff].bytes;
			if (*(long *)(data + 0xbc) > 0)
			{
				result = *(long *)(data + 0xc0);
				break;
			}
			index = *(long *)(data + 8);
		} while (index != NONE);
	}
	return result;
}

// @retail 0x1e4b10
long function_1e4b10(long index)
{
	long result = 0;
	if (index != NONE)
	{
		do
		{
			byte *data = g_4e3b44[index & 0xffff].bytes;
			if (*(long *)(data + 0xc4) > 0)
			{
				result = *(long *)(data + 0xc8);
				break;
			}
			index = *(long *)(data + 8);
		} while (index != NONE);
	}
	return result;
}

/* ---- the blocks of an actor's character tag, chosen by its variant ---- */

struct s_character_block
{
	long count;
	byte *address;
};

/* a character tag: its parent tag at +8, then its blocks */
struct s_character_definition
{
	byte unknown00[8];
	long parent_index;
	byte unknown0c[0x64 - 0xc];
	s_character_block blocks[10];
};


// @retail 0x1e4b50
void *function_1e4b50(long actor_index)
{
	long character_index = actor_get(actor_index)->unknown054;
	void *result = 0;

	if (character_index != NONE)
	{
		do
		{
			short index = 0;
			s_character_variant *variant = (s_character_variant *)function_272a00(actor_index);
			if (variant)
			{
				if (variant->block_indices[10])
					index = variant->block_indices[10] - 1;
				else
					index = variant->default_index;
			}

			s_character_definition *character = (s_character_definition *)g_4e3b44[character_index & 0xffff].bytes;
			if (character->blocks[0].count > 0)
			{
				if (index < character->blocks[0].count)
					result = character->blocks[0].address + index * 0x28;
				else
					result = character->blocks[0].address;
				break;
			}
			character_index = character->parent_index;
		} while (character_index != NONE);
	}

	return result;
}

// @retail 0x1e4be0
void *function_1e4be0(long actor_index)
{
	long character_index = actor_get(actor_index)->unknown054;
	void *result = 0;

	if (character_index != NONE)
	{
		do
		{
			short index = 0;
			s_character_variant *variant = (s_character_variant *)function_272a00(actor_index);
			if (variant)
			{
				if (variant->block_indices[0])
					index = variant->block_indices[0] - 1;
				else
					index = variant->default_index;
			}

			s_character_definition *character = (s_character_definition *)g_4e3b44[character_index & 0xffff].bytes;
			if (character->blocks[2].count > 0)
			{
				if (index < character->blocks[2].count)
					result = character->blocks[2].address + index * 0x10;
				else
					result = character->blocks[2].address;
				break;
			}
			character_index = character->parent_index;
		} while (character_index != NONE);
	}

	return result;
}

// @retail 0x1e4c70
void *function_1e4c70(long actor_index)
{
	long character_index = actor_get(actor_index)->unknown054;
	void *result = 0;

	if (character_index != NONE)
	{
		do
		{
			short index = 0;
			s_character_variant *variant = (s_character_variant *)function_272a00(actor_index);
			if (variant)
			{
				if (variant->block_indices[1])
					index = variant->block_indices[1] - 1;
				else
					index = variant->default_index;
			}

			s_character_definition *character = (s_character_definition *)g_4e3b44[character_index & 0xffff].bytes;
			if (character->blocks[4].count > 0)
			{
				if (index < character->blocks[4].count)
					result = character->blocks[4].address + index * 0x14;
				else
					result = character->blocks[4].address;
				break;
			}
			character_index = character->parent_index;
		} while (character_index != NONE);
	}

	return result;
}

// @retail 0x1e4d10
void *function_1e4d10(long actor_index)
{
	long character_index = actor_get(actor_index)->unknown054;
	void *result = 0;

	if (character_index != NONE)
	{
		do
		{
			short index = 0;
			s_character_variant *variant = (s_character_variant *)function_272a00(actor_index);
			if (variant)
			{
				if (variant->block_indices[2])
					index = variant->block_indices[2] - 1;
				else
					index = variant->default_index;
			}

			s_character_definition *character = (s_character_definition *)g_4e3b44[character_index & 0xffff].bytes;
			if (character->blocks[5].count > 0)
			{
				if (index < character->blocks[5].count)
					result = character->blocks[5].address + index * 0x40;
				else
					result = character->blocks[5].address;
				break;
			}
			character_index = character->parent_index;
		} while (character_index != NONE);
	}

	return result;
}

// @retail 0x1e4db0
void *function_1e4db0(long actor_index)
{
	long character_index = actor_get(actor_index)->unknown054;
	void *result = 0;

	if (character_index != NONE)
	{
		do
		{
			short index = 0;
			s_character_variant *variant = (s_character_variant *)function_272a00(actor_index);
			if (variant)
			{
				if (variant->block_indices[3])
					index = variant->block_indices[3] - 1;
				else
					index = variant->default_index;
			}

			s_character_definition *character = (s_character_definition *)g_4e3b44[character_index & 0xffff].bytes;
			if (character->blocks[7].count > 0)
			{
				if (index < character->blocks[7].count)
					result = character->blocks[7].address + index * 0x14;
				else
					result = character->blocks[7].address;
				break;
			}
			character_index = character->parent_index;
		} while (character_index != NONE);
	}

	return result;
}

// @retail 0x1e4e50
void *function_1e4e50(long actor_index)
{
	long character_index = actor_get(actor_index)->unknown054;
	void *result = 0;

	if (character_index != NONE)
	{
		do
		{
			short index = 0;
			s_character_variant *variant = (s_character_variant *)function_272a00(actor_index);
			if (variant)
			{
				if (variant->block_indices[4])
					index = variant->block_indices[4] - 1;
				else
					index = variant->default_index;
			}

			s_character_definition *character = (s_character_definition *)g_4e3b44[character_index & 0xffff].bytes;
			if (character->blocks[8].count > 0)
			{
				if (index < character->blocks[8].count)
					result = character->blocks[8].address + index * 0x24;
				else
					result = character->blocks[8].address;
				break;
			}
			character_index = character->parent_index;
		} while (character_index != NONE);
	}

	return result;
}

// @retail 0x1e4ef0
void *function_1e4ef0(long actor_index)
{
	long character_index = actor_get(actor_index)->unknown054;
	void *result = 0;

	if (character_index != NONE)
	{
		do
		{
			short index = 0;
			s_character_variant *variant = (s_character_variant *)function_272a00(actor_index);
			if (variant)
			{
				if (variant->block_indices[5])
					index = variant->block_indices[5] - 1;
				else
					index = variant->default_index;
			}

			s_character_definition *character = (s_character_definition *)g_4e3b44[character_index & 0xffff].bytes;
			if (character->blocks[6].count > 0)
			{
				if (index < character->blocks[6].count)
					result = character->blocks[6].address + index * 0x4c;
				else
					result = character->blocks[6].address;
				break;
			}
			character_index = character->parent_index;
		} while (character_index != NONE);
	}

	return result;
}

// @retail 0x1e4f90
void *function_1e4f90(long actor_index)
{
	long character_index = actor_get(actor_index)->unknown054;
	void *result = 0;

	if (character_index != NONE)
	{
		do
		{
			short index = 0;
			s_character_variant *variant = (s_character_variant *)function_272a00(actor_index);
			if (variant)
			{
				if (variant->block_indices[6])
					index = variant->block_indices[6] - 1;
				else
					index = variant->default_index;
			}

			s_character_definition *character = (s_character_definition *)g_4e3b44[character_index & 0xffff].bytes;
			if (character->blocks[3].count > 0)
			{
				if (index < character->blocks[3].count)
					result = character->blocks[3].address + index * 0x40;
				else
					result = character->blocks[3].address;
				break;
			}
			character_index = character->parent_index;
		} while (character_index != NONE);
	}

	return result;
}

// @retail 0x1e5030
void *function_1e5030(long actor_index)
{
	long character_index = actor_get(actor_index)->unknown054;
	void *result = 0;

	if (character_index != NONE)
	{
		do
		{
			short index = 0;
			s_character_variant *variant = (s_character_variant *)function_272a00(actor_index);
			if (variant)
			{
				if (variant->block_indices[7])
					index = variant->block_indices[7] - 1;
				else
					index = variant->default_index;
			}

			s_character_definition *character = (s_character_definition *)g_4e3b44[character_index & 0xffff].bytes;
			if (character->blocks[1].count > 0)
			{
				if (index < character->blocks[1].count)
					result = character->blocks[1].address + index * 0x8;
				else
					result = character->blocks[1].address;
				break;
			}
			character_index = character->parent_index;
		} while (character_index != NONE);
	}

	return result;
}

// @retail 0x1e50c0
void *function_1e50c0(long actor_index)
{
	long character_index = actor_get(actor_index)->unknown054;
	void *result = 0;

	if (character_index != NONE)
	{
		do
		{
			short index = 0;
			s_character_variant *variant = (s_character_variant *)function_272a00(actor_index);
			if (variant)
			{
				if (variant->block_indices[8])
					index = variant->block_indices[8] - 1;
				else
					index = variant->default_index;
			}

			s_character_definition *character = (s_character_definition *)g_4e3b44[character_index & 0xffff].bytes;
			if (character->blocks[9].count > 0)
			{
				if (index < character->blocks[9].count)
					result = character->blocks[9].address + index * 0xc;
				else
					result = character->blocks[9].address;
				break;
			}
			character_index = character->parent_index;
		} while (character_index != NONE);
	}

	return result;
}
