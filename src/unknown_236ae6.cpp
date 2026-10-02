#include "cseries.h"

// @flags /O1 /arch:SSE /Gr

union field_param
{
	long i;
	real r;
};

typedef void (__stdcall *field_info_proc)(long *type, long *offset, field_param *param);

void __stdcall function_236ae6(long *type, long *offset, field_param *param);
void __stdcall function_236b0d(long *type, long *offset, field_param *param);
void __stdcall function_236b34(long *type, long *offset, field_param *param);
void __stdcall function_236b5b(long *type, long *offset, field_param *param);
void __stdcall function_236b7c(long *type, long *offset, field_param *param);
void __stdcall function_236b9d(long *type, long *offset, field_param *param);
void __stdcall function_236bbe(long *type, long *offset, field_param *param);
void __stdcall function_236be5(long *type, long *offset, field_param *param);
void __stdcall function_236c0c(long *type, long *offset, field_param *param);
void __stdcall function_236c33(long *type, long *offset, field_param *param);
void __stdcall function_236c5a(long *type, long *offset, field_param *param);
void __stdcall function_236c81(long *type, long *offset, field_param *param);
void __stdcall function_236ca8(long *type, long *offset, field_param *param);
void __stdcall function_236ccf(long *type, long *offset, field_param *param);
void __stdcall function_236ceb(long *type, long *offset, field_param *param);
void __stdcall function_236d0c(long *type, long *offset, field_param *param);
void __stdcall function_236d2d(long *type, long *offset, field_param *param);
void __stdcall function_236d54(long *type, long *offset, field_param *param);
void __stdcall function_236d7b(long *type, long *offset, field_param *param);
void __stdcall function_236d9c(long *type, long *offset, field_param *param);
void __stdcall function_236dc3(long *type, long *offset, field_param *param);
void __stdcall function_236de4(long *type, long *offset, field_param *param);
void __stdcall function_236e05(long *type, long *offset, field_param *param);
void __stdcall function_236e26(long *type, long *offset, field_param *param);
void __stdcall function_236e47(long *type, long *offset, field_param *param);
void __stdcall function_236e6e(long *type, long *offset, field_param *param);
void __stdcall function_236e8f(long *type, long *offset, field_param *param);
void __stdcall function_236eb6(long *type, long *offset, field_param *param);
void __stdcall function_236edd(long *type, long *offset, field_param *param);
void __stdcall function_236f04(long *type, long *offset, field_param *param);
void __stdcall function_236f25(long *type, long *offset, field_param *param);
void __stdcall function_236f4c(long *type, long *offset, field_param *param);
void __stdcall function_236f6d(long *type, long *offset, field_param *param);
void __stdcall function_236f8e(long *type, long *offset, field_param *param);
void __stdcall function_236fb5(long *type, long *offset, field_param *param);
void __stdcall function_236fdc(long *type, long *offset, field_param *param);

// Retail table of field descriptor callbacks, indexed by field id. Only the
// entries whose functions are in this file are filled in; the rest (null here)
// belong to neighbouring batches.
field_info_proc g_470828[0x70] =
{
	function_236ae6, function_236b0d, function_236b0d, function_236b0d, function_236b0d, function_236b0d, function_236b0d, function_236b0d, function_236b0d, function_236b0d, function_236b34, function_236b5b, function_236b7c, function_236b9d, function_236bbe, function_236be5,
	function_236c33, function_236c5a, function_236c81, function_236ca8, function_236ccf, function_236ceb, function_236d0c, function_236d2d, function_236d7b, function_236d54, function_236d9c, 0, 0, 0, function_236e26, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, function_236e26, function_236e6e, 0, 0, 0, 0, 0,
	0, 0, function_236e47, 0, function_236e26, function_236e6e, function_236e8f, function_236eb6, function_236edd, function_236f04, function_236f25, function_236e47, 0, function_236e8f, function_236e6e, function_236f8e,
	function_236eb6, function_236edd, function_236f04, function_236f25, function_236e6e, function_236f04, function_236fdc, 0, function_236e26, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, function_236c0c, 0, function_236e26, 0, 0, function_236fb5,
	0, function_236fdc, 0, 0, 0, function_236f04, function_236e26, function_236e6e, function_236dc3, function_236de4, function_236f4c, function_236f6d, function_236f4c, function_236f4c, function_236f6d, function_236e05,
};

// @retail 0x236ae6
void __stdcall function_236ae6(long *type, long *offset, field_param *param)
{
	*type = 5;
	*offset = 0x4c;
	param->r = 1.0f;
}

// @retail 0x236b0d
void __stdcall function_236b0d(long *type, long *offset, field_param *param)
{
	*type = 5;
	*offset = 0x50;
	param->r = 1.0f;
}

// @retail 0x236b34
void __stdcall function_236b34(long *type, long *offset, field_param *param)
{
	*type = 5;
	*offset = 0x54;
	param->r = 1.0f;
}

// @retail 0x236b5b
void __stdcall function_236b5b(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0x48;
	param->i = 3;
}

// @retail 0x236b7c
void __stdcall function_236b7c(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0x48;
	param->i = 4;
}

// @retail 0x236b9d
void __stdcall function_236b9d(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0x48;
	param->i = 5;
}

// @retail 0x236bbe
void __stdcall function_236bbe(long *type, long *offset, field_param *param)
{
	*type = 5;
	*offset = 0x58;
	param->r = 1.0f;
}

// @retail 0x236be5
void __stdcall function_236be5(long *type, long *offset, field_param *param)
{
	*type = 5;
	*offset = 0x74;
	param->r = 1.0f;
}

// @retail 0x236c0c
void __stdcall function_236c0c(long *type, long *offset, field_param *param)
{
	*type = 5;
	*offset = 0x78;
	param->r = 1.0f;
}

// @retail 0x236c33
void __stdcall function_236c33(long *type, long *offset, field_param *param)
{
	*type = 5;
	*offset = 0x7c;
	param->r = 1.0f;
}

// @retail 0x236c5a
void __stdcall function_236c5a(long *type, long *offset, field_param *param)
{
	*type = 5;
	*offset = 0x80;
	param->r = 1.0f;
}

// @retail 0x236c81
void __stdcall function_236c81(long *type, long *offset, field_param *param)
{
	*type = 5;
	*offset = 0x84;
	param->r = 1.0f;
}

// @retail 0x236ca8
void __stdcall function_236ca8(long *type, long *offset, field_param *param)
{
	*type = 5;
	*offset = 0x88;
	param->r = 1.0f;
}

// @retail 0x236ccf
void __stdcall function_236ccf(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0x48;
	param->i = 1;
}

// @retail 0x236ceb
void __stdcall function_236ceb(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0x48;
	param->i = 2;
}

// @retail 0x236d0c
void __stdcall function_236d0c(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0x48;
	param->i = 6;
}

// @retail 0x236d2d
void __stdcall function_236d2d(long *type, long *offset, field_param *param)
{
	*type = 5;
	*offset = 0xa4;
	param->r = 1.0f;
}

// @retail 0x236d54
void __stdcall function_236d54(long *type, long *offset, field_param *param)
{
	*type = 5;
	*offset = 0xa8;
	param->r = 1.0f;
}

// @retail 0x236d7b
void __stdcall function_236d7b(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0x48;
	param->i = 7;
}

// @retail 0x236d9c
void __stdcall function_236d9c(long *type, long *offset, field_param *param)
{
	*type = 5;
	*offset = 0xac;
	param->r = 1.0f;
}

// @retail 0x236dc3
void __stdcall function_236dc3(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0x48;
	param->i = 12;
}

// @retail 0x236de4
void __stdcall function_236de4(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0x48;
	param->i = 13;
}

// @retail 0x236e05
void __stdcall function_236e05(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0x48;
	param->i = 14;
}

// @retail 0x236e26
void __stdcall function_236e26(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0xf0;
	param->i = 2;
}

// @retail 0x236e47
void __stdcall function_236e47(long *type, long *offset, field_param *param)
{
	*type = 5;
	*offset = 0x104;
	param->r = 1.0f;
}

// @retail 0x236e6e
void __stdcall function_236e6e(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0xf0;
	param->i = 3;
}

// @retail 0x236e8f
void __stdcall function_236e8f(long *type, long *offset, field_param *param)
{
	*type = 5;
	*offset = 0xf4;
	param->r = 1.0f;
}

// @retail 0x236eb6
void __stdcall function_236eb6(long *type, long *offset, field_param *param)
{
	*type = 5;
	*offset = 0xf8;
	param->r = 1.0f;
}

// @retail 0x236edd
void __stdcall function_236edd(long *type, long *offset, field_param *param)
{
	*type = 5;
	*offset = 0xfc;
	param->r = 1.0f;
}

// @retail 0x236f04
void __stdcall function_236f04(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0xf0;
	param->i = 4;
}

// @retail 0x236f25
void __stdcall function_236f25(long *type, long *offset, field_param *param)
{
	*type = 5;
	*offset = 0x100;
	param->r = 1.0f;
}

// @retail 0x236f4c
void __stdcall function_236f4c(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0xf0;
	param->i = 6;
}

// @retail 0x236f6d
void __stdcall function_236f6d(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0xf0;
	param->i = 7;
}

// @retail 0x236f8e
void __stdcall function_236f8e(long *type, long *offset, field_param *param)
{
	*type = 4;
	*offset = 0x108;
	param->r = 1.0f;
}

// @retail 0x236fb5
void __stdcall function_236fb5(long *type, long *offset, field_param *param)
{
	*type = 4;
	*offset = 0x10a;
	param->r = 1.0f;
}

// @retail 0x236fdc
void __stdcall function_236fdc(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0xf0;
	param->i = 5;
}
