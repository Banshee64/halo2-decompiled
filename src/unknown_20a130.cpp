#include "unknown_11c920.h"
#include "hs.h"

// @flags /O2 /Gr

extern long *g_4de2d0;
long function_1ded60(void);
void function_1dedb0(long list_index, long object_index);

// @retail 0x20a130
long __stdcall function_20a130(script_value value)
{
	long result = NONE;
	if (value.s >= 0 && value.s < 0x280)
	{
		long object_index = g_4de2d0[value.s];
		if (object_index != NONE)
		{
			result = function_1ded60();
			function_1dedb0(result, object_index);
		}
	}
	return result;
}

// @retail 0x20a170
long __stdcall function_20a170(long object_index)
{
	long result = NONE;
	if (object_index != NONE)
	{
		result = function_1ded60();
		function_1dedb0(result, object_index);
	}
	return result;
}

// The initialization at 0x20a1a0 installs these casts in the runtime table.
long (__stdcall *g_4f7640)(long) = function_20a170;
long (__stdcall *g_4f7658)(script_value) = function_20a130;


typedef long (__stdcall *t_runtime_cast)(long);
extern t_runtime_cast g_4f5770[0x3e * 0x3e];
long __stdcall function_11ca70(long unused);
long __stdcall function_272ea0(long ai_index);
script_value __stdcall function_20a000(script_value value);
script_value __stdcall function_20a020(script_value value);
script_value __stdcall function_20a040(script_value value);
script_value __stdcall function_20a070(script_value value);
script_value __stdcall function_20a090(script_value value);
script_value __stdcall function_20a0b0(script_value value);
script_value __stdcall function_20a0d0(script_value value);
int __stdcall function_20a0f0(script_value value);
int __stdcall function_20a100(script_value value);
script_value __stdcall function_20a110(script_value value);

// @retail 0x20a1a0
void function_20a1a0(void)
{
    for (long i = 5; i < 62; i++)
        g_4f5770[4 * 62 + i] = function_11ca70;
    g_4f5770[5 * 62 + 6] = (t_runtime_cast)function_20a000;
    g_4f5770[5 * 62 + 8] = (t_runtime_cast)function_20a000;
    g_4f5770[6 * 62 + 44] = (t_runtime_cast)function_20a0b0;
    g_4f5770[6 * 62 + 45] = (t_runtime_cast)function_20a0b0;
    g_4f5770[6 * 62 + 46] = (t_runtime_cast)function_20a0b0;
    g_4f5770[6 * 62 + 47] = (t_runtime_cast)function_20a0b0;
    g_4f5770[6 * 62 + 48] = (t_runtime_cast)function_20a0b0;
    g_4f5770[6 * 62 + 49] = (t_runtime_cast)function_20a0b0;
    g_4f5770[31 * 62 + 50] = (t_runtime_cast)function_20a170;
    g_4f5770[31 * 62 + 51] = (t_runtime_cast)function_20a170;
    g_4f5770[31 * 62 + 52] = (t_runtime_cast)function_20a170;
    g_4f5770[31 * 62 + 53] = (t_runtime_cast)function_20a170;
    g_4f5770[31 * 62 + 54] = (t_runtime_cast)function_20a170;
    g_4f5770[31 * 62 + 55] = (t_runtime_cast)function_20a170;
    g_4f5770[5 * 62 + 7] = (t_runtime_cast)function_20a020;
    g_4f5770[5 * 62 + 9] = (t_runtime_cast)function_20a040;
    g_4f5770[6 * 62 + 7] = (t_runtime_cast)function_20a070;
    g_4f5770[6 * 62 + 8] = (t_runtime_cast)function_20a090;
    g_4f5770[7 * 62 + 6] = (t_runtime_cast)function_20a0d0;
    g_4f5770[7 * 62 + 8] = (t_runtime_cast)function_20a110;
    g_4f5770[8 * 62 + 6] = (t_runtime_cast)function_20a0f0;
    g_4f5770[8 * 62 + 7] = (t_runtime_cast)function_20a100;
    g_4f5770[31 * 62 + 56] = (t_runtime_cast)function_20a130;
    g_4f5770[31 * 62 + 57] = (t_runtime_cast)function_20a130;
    g_4f5770[31 * 62 + 58] = (t_runtime_cast)function_20a130;
    g_4f5770[31 * 62 + 59] = (t_runtime_cast)function_20a130;
    g_4f5770[31 * 62 + 60] = (t_runtime_cast)function_20a130;
    g_4f5770[31 * 62 + 61] = (t_runtime_cast)function_20a130;
    g_4f5770[31 * 62 + 19] = (t_runtime_cast)function_272ea0;
}
