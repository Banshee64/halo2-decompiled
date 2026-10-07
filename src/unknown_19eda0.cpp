#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_0259d0.h"
#include "engine_peer.h"

// @flags /O2 /arch:SSE /Gr

/* UNKNOWN_19EDA0.CPP: the multiplayer weapon choices (a weapon type names
   one of the multiplayer globals' weapon references, or a random one) and
   weighted random choices from a tag's list */

struct s_tag_reference_view
{
	dword group_tag;
	long index;
};

/* the multiplayer globals' weapon references */
struct s_multiplayer_weapons
{
	s_tag_reference_view references[18];
};

struct s_multiplayer_runtime_globals
{
	byte unknown00[0x6c];
	s_multiplayer_weapons *weapons;
};

struct s_multiplayer_globals_definition
{
	byte unknown00[0xc];
	s_multiplayer_runtime_globals *runtime;
};

/* the weapon of a weapon type: none, a random one other than the excluded
   one, one of the globals' weapons, or the default */
// @retail 0x19eda0
long __stdcall function_19eda0(long default_weapon, char type, long excluded)
{
	long result = default_weapon;
	long index = g_4e034c->index & 0xffff;
	s_multiplayer_runtime_globals *runtime = ((s_multiplayer_globals_definition *)g_4e3b44[index].bytes)->runtime;
	switch (type)
	{
	case 1:
		result = NONE;
		break;
	case 2:
	{
		do
		{
			result = function_19eda0(default_weapon, (char)(random_index(&g_4e7408->unknown0, 16) + 3), excluded);
		} while (result == excluded && excluded != NONE || result == NONE);
		break;
	}
	case 3:
		result = runtime->weapons->references[3].index;
		break;
	case 4:
		result = runtime->weapons->references[0].index;
		break;
	case 5:
		result = runtime->weapons->references[1].index;
		break;
	case 6:
		result = runtime->weapons->references[6].index;
		break;
	case 7:
		result = runtime->weapons->references[7].index;
		break;
	case 8:
		result = runtime->weapons->references[5].index;
		break;
	case 9:
		result = runtime->weapons->references[2].index;
		break;
	case 10:
		result = runtime->weapons->references[4].index;
		break;
	case 11:
		result = runtime->weapons->references[12].index;
		break;
	case 12:
		result = runtime->weapons->references[9].index;
		break;
	case 13:
		result = runtime->weapons->references[10].index;
		break;
	case 14:
		result = runtime->weapons->references[13].index;
		break;
	case 15:
		result = runtime->weapons->references[11].index;
		break;
	case 16:
		result = runtime->weapons->references[15].index;
		break;
	case 17:
		result = runtime->weapons->references[14].index;
		break;
	case 18:
		result = runtime->weapons->references[8].index;
		break;
	case 19:
		result = runtime->weapons->references[16].index;
		break;
	case 20:
		result = runtime->weapons->references[17].index;
		break;
	}
	return result;
}

struct s_weighted_choice
{
	real weight;
	byte unknown04[4];
	long value;
	long extra;
};

struct s_weighted_choice_block
{
	long count;
	s_weighted_choice *choices;
};

// @retail 0x19f130
real weighted_choices_total(s_weighted_choice_block *block)
{
	long count = block->count;
	s_weighted_choice *choices = block->choices;
	real total = 0.0f;

	for (long i = 0; i < count; i++)
		total = choices[i].weight + total;

	return total;
}

// @retail 0x19f1a0
long weighted_choice_random(long tag_index, long *extra)
{
	s_weighted_choice_block *block = (s_weighted_choice_block *)g_4e3b44[tag_index & 0xffff].bytes;
	long count = block->count;
	s_weighted_choice *choices = block->choices;
	long result = NONE;
	real total = weighted_choices_total(block);

	if (count > 1)
	{
		real value = (real)random_index(&g_4e7408->unknown0, (short)(long)total);

		s_weighted_choice *choice = choices;

		for (long i = 0; i < count; i++, choice++)
		{
			value -= choice->weight;
			if (value <= 0.0f)
			{
				result = choice->value;
				if (extra)
					*extra = choice->extra;
				break;
			}
		}
	}
	else if (count > 0)
	{
		result = choices[0].value;
		if (extra)
			*extra = choices[0].extra;
	}

	return result;
}


struct s_weapon_rule_choice
{
    long tag_index;
    long unknown04;
};

struct s_weapon_rule
{
    byte flags;
    byte unknown01[3];
    short types[4];
    byte unknown0c[0x40 - 0x0c];
    s_weapon_rule_choice choices[5];
    byte unknown68[0x9c - 0x68];
};

struct s_weapon_rule_globals
{
    byte unknown000[0x128];
    long count;
    s_weapon_rule *rules;
};

struct s_weapon_rule_header
{
    byte unknown00[8];
    byte *object;
};

struct s_effect_owner;
long function_15a090(short const *types, long type, long count);
void __stdcall function_ccff0(long unit_index);
void function_b7930(void *data, long tag_index, long object_index, s_effect_owner const *owner);
long function_b7b40(void *creation);
bool unit_has_weapon_definition(long unit_index, long definition_index);
bool __stdcall function_cd0c0(long unit_index, long weapon_index, short mode);
void __stdcall function_b8540(long object_index);

// @retail 0x19ef40
void __stdcall function_19ef40(long unit_index, long *first_count, long *second_count)
{
    s_weapon_rule_globals *globals = (s_weapon_rule_globals *)g_4e0350;
    long i = 0;
    if (globals->count > 0)
    {
        s_weapon_rule *rule = globals->rules;
        do
        {
            c_engine_peer *engine = g_55e4d0[g_4e9ae8->engine_index];
            long type = NONE;
            if (engine)
                type = engine->p0();
            if (function_15a090(rule->types, type, 4) == NONE)
            {
                rule++;
                i++;
                continue;
            }
            function_ccff0(unit_index);
            long weapons[2];
            weapons[0] = NONE;
            weapons[1] = NONE;
            for (long j = 0; j < 5; j++)
            {
                long tag_index = rule->choices[j].tag_index;
                if (tag_index != NONE)
                {
                    long extra;
                    long weapon = weighted_choice_random(tag_index, &extra);
                    if (weapon != NONE)
                    {
                        if (weapons[0] == NONE)
                            weapons[0] = weapon;
                        else if (weapon != weapons[0])
                        {
                            weapons[1] = weapon;
                            break;
                        }
                    }
                }
            }
            byte first_type = *((byte *)g_4e6948 + 0x212);
            if (first_type == 2)
            {
                weapons[1] = function_19eda0(weapons[1], *((char *)g_4e6948 + 0x213), NONE);
                weapons[0] = function_19eda0(weapons[0], *((char *)g_4e6948 + 0x212), weapons[1]);
            }
            else
            {
                weapons[0] = function_19eda0(weapons[0], (char)first_type, NONE);
                weapons[1] = function_19eda0(weapons[1], *((char *)g_4e6948 + 0x213), weapons[0]);
            }
            for (long k = 0; k < 2; k++)
            {
                if (weapons[k] != NONE)
                {
                    byte creation[0xc4];
                    function_b7930(creation, weapons[k], NONE, NULL);
                    *(long *)(creation + 0x0c) = 0;
                    long weapon_index = function_b7b40(creation);
                    if (weapon_index != NONE)
                    {
                        byte *weapon = ((s_weapon_rule_header *)g_4e0300->data)[weapon_index & 0xffff].object;
                        if (!unit_has_weapon_definition(unit_index, *(long *)weapon))
                            function_cd0c0(unit_index, weapon_index, 1);
                        else
                            function_b8540(weapon_index);
                    }
                }
            }
            if (rule->flags & 1)
            {
                *first_count = 0;
                *second_count = 0;
            }
            if (rule->flags & 2)
            {
                *second_count += *first_count;
                *first_count = 0;
            }
            break;
        }
        while (i < globals->count);
    }
}
