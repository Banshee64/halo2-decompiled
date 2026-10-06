// @flags /O2 /Gr
/* UNKNOWN_11BE40.CPP: a lifecycle callback (entry 24, field_10_2) */

#include "unknown_11c920.h"
#include <string.h>

struct s_state_11be20
{
	bool initialized;
	byte unknown001[0xd0b];
};

s_state_11be20 g_4f93b8;

void function_211cc0(void);

// @retail 0x11be20
void function_11be20(void)
{
	memset(&g_4f93b8, 0, sizeof(g_4f93b8));
	g_4f93b8.initialized = true;
	function_211cc0();
}

// @retail 0x11be40
void function_11be40(void)
{
	g_4f93b8.initialized = false;
}
