#pragma once

/* UNKNOWN_09A9F0.H: the turret simulation entity definition (vtable 0x452788)
   and the game functions it calls that are not decompiled yet */

/* a bitstream: the data, its size in bytes, and the current bit position */
struct s_bitstream
{
	byte *data;
	long size_in_bytes;
	byte unknown08[8];
	long bit_position;
};

/* the object, as seen by this code */
struct s_object_view
{
	long definition_index;
	byte unknown04[8];
	long next_sibling;
	long first_child;
	long field14;
	byte unknown18[2];
	short field1a;
	byte unknown1c[8];
	long field24;
	byte unknown28[0xaa - 0x28];
	signed char type;
	byte field_ab;
	byte unknownac[3];
	byte field_af;
	byte unknownb0[0xd4 - 0xb0];
	long field_d4;
	byte field_d8;
	byte unknownd9[0x10a - 0xd9];
	word flag0 : 1;
	word flag1 : 1;
	word flag2 : 1;
	word flag3 : 1;
	word flag4 : 1;
	word flag5 : 1;
	word flag6 : 1;
	word flag7 : 1;
	word flag8 : 1;
	word flag9 : 1;
	word flag10 : 1;
	word flag11 : 1;
	word flag12 : 1;
	word flag13 : 1;
	word flag14 : 1;
	word flag15 : 1;
};

struct s_object_header
{
	byte unknown00[2];
	byte flags;
	byte unknown03[5];
	s_object_view *object;
};

/* the per-entity creation info: the definition index, and an identifier at
   +0x10 (10 bits of index and 4 bits of salt) */
struct s_entity_info
{
	long field0;
	long definition_index;
	byte unknown08[8];
	long identifier;
};

/* the entity: field 8 is the object index once created */
struct s_entity
{
	long field0;
	byte unknown04[2];
	bool field_6;
	byte unknown07;
	long object_index;
	byte unknown0c[8];
	s_entity_info *info;
};

struct s_entity_state
{
	long field0;
	long field4;
	byte field8;
	long fieldc;
	long field10;
};

struct s_entity_data
{
	byte unknown00[0x68];
	long block[10];
};

struct s_creation_request
{
	byte unknown00[4];
	short definition_index;
};

struct s_creation_weight
{
	real weight;
	long field4;
	byte unknown08[0x44];
};

/* the library routines the entity code calls; none are decompiled yet */
bool function_a5bd0(long a);
bool function_a6d50(long a, long b, s_bitstream *stream);
void function_a6900(long a, long b);
long function_a5930(long a);
long function_a5e70(long a, long b, long c);
long function_a58d0(long a);
void function_a6430(long a, long b, long c);
real function_aa4d0(long a, long b, long c, long d, long e);
void function_11c9c0(const char *format, ...);
void function_a6660(s_entity_info *info, s_bitstream *stream);
void function_b5650(long identifier, s_bitstream *stream);
bool function_a6810(s_bitstream *stream);
bool function_1957d0();
long function_1959c0(long bit_count);
bool function_a69a0(long a, long b, long c, long d, long e, bool f, long g);
void function_a7180(long a, long b);
void function_bb7b0(long a);
void function_b8540(long a);
