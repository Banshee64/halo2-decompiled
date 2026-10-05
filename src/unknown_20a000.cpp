// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"
#include "hs.h"

// @retail 0x20a000
script_value __stdcall function_20a000(script_value v)
{
	v.b = (v.d == 0);
	return v;
}

// @retail 0x20a020
script_value __stdcall function_20a020(script_value v)
{
	v.b = (v.s == 0);
	return v;
}

// @retail 0x20a040
script_value __stdcall function_20a040(script_value v)
{
	const char *s = (const char *)v.d;
	const char *t = s + 1;
	char c;
	do
	{
		c = *s++;
	}
	while (c);
	v.d = (dword)(s - t);
	v.b = (v.d == 0);
	return v;
}

// @retail 0x20a070
script_value __stdcall function_20a070(script_value v)
{
	v.r = (real)v.s;
	return v;
}

// @retail 0x20a090
script_value __stdcall function_20a090(script_value v)
{
	v.r = (real)(int)v.d;
	return v;
}

// @retail 0x20a0b0
script_value __stdcall function_20a0b0(script_value v)
{
	v.r = (real)(v.s + 1);
	return v;
}

// @retail 0x20a0d0
script_value __stdcall function_20a0d0(script_value v)
{
	v.s = (short)(int)v.r;
	return v;
}

// @retail 0x20a0f0
int __stdcall function_20a0f0(script_value v)
{
	return (int)v.r;
}

// @retail 0x20a100
int __stdcall function_20a100(script_value v)
{
	return v.s;
}

// @retail 0x20a110
script_value __stdcall function_20a110(script_value v)
{
	v.s = (short)v.w;
	return v;
}

// These pointers are initialised statically as a stand-in for the runtime
// init near 0x1fa1b0, which stores these addresses into the script function
// tables; remove them when that init is decompiled.
typedef script_value (__stdcall *script_cast_proc)(script_value);
script_cast_proc g_4f5c60 = function_20a000;
script_cast_proc g_4f5c64 = function_20a020;
script_cast_proc g_4f5c6c = function_20a040;
script_cast_proc g_4f5d5c = function_20a070;
script_cast_proc g_4f5d60 = function_20a090;
script_cast_proc g_4f5df0 = function_20a0b0;
script_cast_proc g_4f5e50 = function_20a0d0;
script_cast_proc g_4f5e58 = function_20a110;
script_cast_proc g_4f5f48 = (script_cast_proc)function_20a0f0;
script_cast_proc g_4f5f4c = (script_cast_proc)function_20a100;
