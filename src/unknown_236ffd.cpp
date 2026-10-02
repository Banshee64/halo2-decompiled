#include "cseries.h"

// @flags /O1 /arch:SSE /Gr

union field_param
{
	long i;
	real r;
};

typedef void (__stdcall *field_info_proc)(long *type, long *offset, field_param *param);

void __stdcall function_236ffd(long *type, long *offset, field_param *param);
void __stdcall function_237024(long *type, long *offset, field_param *param);
void __stdcall function_23704b(long *type, long *offset, field_param *param);
void __stdcall function_237072(long *type, long *offset, field_param *param);
void __stdcall function_23708e(long *type, long *offset, field_param *param);
void __stdcall function_2370ac(long *type, long *offset, field_param *param);
void __stdcall function_2370d3(long *type, long *offset, field_param *param);
void __stdcall function_2370fa(long *type, long *offset, field_param *param);
void __stdcall function_237121(long *type, long *offset, field_param *param);
void __stdcall function_237148(long *type, long *offset, field_param *param);
void __stdcall function_23716f(long *type, long *offset, field_param *param);
void __stdcall function_237196(long *type, long *offset, field_param *param);
void __stdcall function_2371bd(long *type, long *offset, field_param *param);
void __stdcall function_2371e4(long *type, long *offset, field_param *param);
void __stdcall function_23720b(long *type, long *offset, field_param *param);
void __stdcall function_237232(long *type, long *offset, field_param *param);
void __stdcall function_237259(long *type, long *offset, field_param *param);
void __stdcall function_237280(long *type, long *offset, field_param *param);

// Retail table of field descriptor callbacks, indexed by field id. Only the
// entries whose functions are in this file are filled in; the rest (null here)
// belong to neighbouring batches.
field_info_proc g_470828[0x70] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, function_23708e, function_237072, 0, 0,
	0, 0, 0, 0, 0, 0, function_23708e, function_237072, function_2370ac, 0, 0, function_2370ac, function_236ffd, function_237024, function_23708e, function_23704b,
	0, 0, 0, function_237072, 0, 0, 0, 0, 0, 0, 0, 0, function_237072, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, function_237072, 0, function_23708e, function_2370d3, function_237121, function_237148, function_23716f, function_237196, function_2371bd,
	function_2371e4, function_23720b, function_237232, function_237259, function_237280, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	function_2370ac, 0, function_2370fa, function_2370ac, function_237072, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// @retail 0x236ffd
void __stdcall function_236ffd(long *type, long *offset, field_param *param)
{
	*type = 4;
	*offset = 0xf6;
	param->r = 1.0f;
}

// @retail 0x237024
void __stdcall function_237024(long *type, long *offset, field_param *param)
{
	*type = 4;
	*offset = 0xf8;
	param->r = 1.0f;
}

// @retail 0x23704b
void __stdcall function_23704b(long *type, long *offset, field_param *param)
{
	*type = 4;
	*offset = 0xfa;
	param->r = 1.0f;
}

// @retail 0x237072
void __stdcall function_237072(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0xf0;
	param->i = 1;
}

// @retail 0x23708e
void __stdcall function_23708e(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0xf0;
	param->i = 0;
}

// @retail 0x2370ac
void __stdcall function_2370ac(long *type, long *offset, field_param *param)
{
	*type = 4;
	*offset = 0xf4;
	param->r = 1.0f;
}

// @retail 0x2370d3
void __stdcall function_2370d3(long *type, long *offset, field_param *param)
{
	*type = 4;
	*offset = 0xf0;
	param->r = 1.0f;
}

// @retail 0x2370fa
void __stdcall function_2370fa(long *type, long *offset, field_param *param)
{
	*type = 4;
	*offset = 0xf2;
	param->r = 1.0f;
}

// @retail 0x237121
void __stdcall function_237121(long *type, long *offset, field_param *param)
{
	*type = 3;
	*offset = 0xcc;
	param->r = 1.0f;
}

// @retail 0x237148
void __stdcall function_237148(long *type, long *offset, field_param *param)
{
	*type = 3;
	*offset = 0xcd;
	param->r = 1.0f;
}

// @retail 0x23716f
void __stdcall function_23716f(long *type, long *offset, field_param *param)
{
	*type = 3;
	*offset = 0xce;
	param->r = 1.0f;
}

// @retail 0x237196
void __stdcall function_237196(long *type, long *offset, field_param *param)
{
	*type = 3;
	*offset = 0xcf;
	param->r = 1.0f;
}

// @retail 0x2371bd
void __stdcall function_2371bd(long *type, long *offset, field_param *param)
{
	*type = 3;
	*offset = 0xd0;
	param->r = 1.0f;
}

// @retail 0x2371e4
void __stdcall function_2371e4(long *type, long *offset, field_param *param)
{
	*type = 3;
	*offset = 0xd1;
	param->r = 1.0f;
}

// @retail 0x23720b
void __stdcall function_23720b(long *type, long *offset, field_param *param)
{
	*type = 3;
	*offset = 0xd2;
	param->r = 1.0f;
}

// @retail 0x237232
void __stdcall function_237232(long *type, long *offset, field_param *param)
{
	*type = 3;
	*offset = 0xd3;
	param->r = 1.0f;
}

// @retail 0x237259
void __stdcall function_237259(long *type, long *offset, field_param *param)
{
	*type = 3;
	*offset = 0xd4;
	param->r = 1.0f;
}

// @retail 0x237280
void __stdcall function_237280(long *type, long *offset, field_param *param)
{
	*type = 1;
	*offset = 0x48;
	param->i = 8;
}
