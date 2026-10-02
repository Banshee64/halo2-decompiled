// @flags /O2 /Gr
#include "cseries.h"
#include "loop_allocator.h"

struct s_476fc8
{
	long unknown00;
	byte initialized;
	byte unknown05[3];
	long mode;
	long unknown0c;
	long field10;
};

s_476fc8 g_476fc8;
byte g_476fbc[4];
long g_4c9890;
long g_4c9894;
s_loop_allocator *g_4c995c;

void function_556a0(s_476fc8 *voice);
void function_55720(s_476fc8 *voice);
bool function_55810(s_476fc8 *voice);
void function_550b0(s_476fc8 *voice);

// @retail 0x53310
void __stdcall function_53310(long stage)
{
	long type;
	long size = 0;
	long mode = 0;
	c_memory_source *source = (c_memory_source *)g_476fbc;
	s_loop_allocator *loop;

	if (stage > 1)
	{
		type = (stage <= 3);
	}
	else
	{
		type = 2;
	}

	switch (type)
	{
	case 1:
		mode = 1;
		break;
	case 2:
		size = 0x78000;
		mode = 2;
		break;
	}

	g_4c9890 = type;
	g_4c9894 = mode;

	if (type == 2)
	{
		loop = (s_loop_allocator *)source->allocate(size + 0x50);
		if (loop)
		{
			function_18e250(loop, size, "voice pool", source);
		}
		g_4c995c = loop;
		g_4c995c->field3c = 1;
		g_4c995c->field3d = 1;
		g_4c995c->field3e = 1;

		if (!g_476fc8.initialized)
		{
			g_476fc8.field10 = 0;
			g_476fc8.mode = mode;
			function_556a0(&g_476fc8);
			function_55720(&g_476fc8);
			g_476fc8.initialized = function_55810(&g_476fc8);
		}
	}
}

// @retail 0x533e0
void function_533e0(void)
{
	if (g_4c995c)
	{
		function_550b0(&g_476fc8);
		function_18e230(g_4c995c);
		g_4c995c = 0;
	}
}