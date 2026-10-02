// @flags /O2 /Gr
/* UNKNOWN_196D20.CPP: game speed (input/player state accessors and a
   clamped counter update) */

#include "cseries.h"
#include <string.h>

struct s_196d20_vector
{
	dword v[4];
};

struct s_196d20_entry
{
	byte unknown00[4];
	byte active;
	byte unknown05[0x4f];
	s_196d20_vector vector;
	byte unknown64[0x2c];
	char value_90;
	byte unknown91[0xb];
	short value_9c;
	byte unknown9e[6];
};

struct s_counter
{
	word value : 15;
	word flag : 1;
};

struct s_counter_range
{
	word minimum;
	word maximum;
	byte unknown04[12];
};

struct s_196d20_sample
{
	dword data[9];
};

extern byte g_510cb1;
extern byte g_510ca0;
extern byte g_510ca1;
extern dword g_510ca4;
extern s_196d20_entry g_511000[];
extern s_counter_range g_46e108[];
extern s_counter_range g_46e098[];
extern s_counter_range g_46e128[];
extern s_counter g_515294[];
extern s_counter g_511c90[];
extern s_counter g_511c4e[];
extern s_196d20_sample g_515c34[];

byte g_510cb1;
byte g_510ca0;
byte g_510ca1;
dword g_510ca4;
s_196d20_entry g_511000[4];
s_counter_range g_46e108[64];
s_counter_range g_46e098[64];
s_counter_range g_46e128[64];
s_counter g_515294[1024];
s_counter g_511c90[1024];
s_counter g_511c4e[1024];
s_196d20_sample g_515c34[1000];

// @retail 0x196d20
long function_196d20(long index)
{
	long result = NONE;
	if (g_510cb1 && index != NONE && g_511000[index].active)
		result = g_511000[index].value_90;
	return result;
}

// @retail 0x196d50
void function_196d50(long index, s_196d20_vector *out)
{
	if (g_510cb1 && index != NONE && g_511000[index].active)
	{
		*out = g_511000[index].vector;
		return;
	}
	memset(out, 0, sizeof(*out));
}

// @retail 0x196da0
long function_196da0(long index)
{
	long result = NONE;
	if (g_510cb1 && index != NONE && g_511000[index].active)
		result = g_511000[index].value_9c;
	return result;
}

// @retail 0x196dd0
void function_196dd0(long b, long a, long c, long delta)
{
	if (g_510ca0 && !g_510ca1)
	{
		long i = a + (b * 16 + c) * 2;
		s_counter *counter = &g_515294[i];
		long minimum = g_46e108[a].minimum;
		long maximum = g_46e108[a].maximum;
		long value = counter->value + delta;
		if (value < minimum)
			value = minimum;
		else if (value > maximum)
			value = maximum;
		counter->value = value;
	}
}

// @retail 0x196e60
void function_196e60(long b, long a, long c, long delta)
{
	if (g_510ca0 && !g_510ca1)
	{
		long i = b * 0x1b5 + a + c * 8;
		s_counter *counter = &g_511c90[i];
		long minimum = g_46e098[a].minimum;
		long maximum = g_46e098[a].maximum;
		long value = counter->value + delta;
		if (value < minimum)
			value = minimum;
		else if (value > maximum)
			value = maximum;
		counter->value = value;
	}
}

// @retail 0x196ef0
long function_196ef0(long code)
{
	long result = 0;
	switch (code & 0x3f)
	{
	case 0: result = 0; break;
	case 1: result = 1; break;
	case 2: result = 2; break;
	case 3: result = 3; break;
	case 4: result = 4; break;
	case 5: result = 5; break;
	case 6: result = 6; break;
	case 7: result = 7; break;
	case 8: result = 8; break;
	case 9: result = 9; break;
	case 10: result = 10; break;
	case 11: result = 11; break;
	case 12: result = 12; break;
	case 13: result = 13; break;
	case 14: result = 14; break;
	case 19: result = 15; break;
	case 15: result = 16; break;
	case 16: result = 17; break;
	case 17: result = 18; break;
	case 18: result = 19; break;
	case 20: result = 22; break;
	case 21: result = 23; break;
	case 22: result = 24; break;
	case 23: result = 25; break;
	case 24: result = 26; break;
	case 25: result = 40; break;
	case 26: result = 27; break;
	case 27: result = 28; break;
	case 28: result = 29; break;
	case 29: result = 30; break;
	case 30: result = 31; break;
	case 31: result = 32; break;
	case 32: result = 33; break;
	case 33: result = 34; break;
	case 34: result = 35; break;
	case 35: result = 36; break;
	case 36: result = 37; break;
	case 37: result = 38; break;
	case 38: result = 39; break;
	case 39: result = 20; break;
	case 40: result = 21; break;
	}
	return result;
}

// @retail 0x1970a0
void function_1970a0(long b, long a, long delta)
{
	if (g_510ca0 && !g_510ca1)
	{
		long i = b * 0x1b5 + a;
		s_counter *counter = &g_511c4e[i];
		long minimum = g_46e128[a].minimum;
		long maximum = g_46e128[a].maximum;
		long value = counter->value + delta;
		if (value < minimum)
			value = minimum;
		else if (value > maximum)
			value = maximum;
		counter->value = value;
	}
}

// @retail 0x197120
void function_197120(const s_196d20_sample *sample)
{
	if (g_510ca0 && !g_510ca1)
	{
		dword i = g_510ca4;
		g_515c34[i] = *sample;
		g_510ca4 = (i + 1) % 1000;
	}
}
