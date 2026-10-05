#ifndef UNKNOWN_25D020_H
#define UNKNOWN_25D020_H

#include "props.h"
#include "object_queries.h"

struct s_2640c0
{
	point3f field_0;
	point3f field_c;
	byte field_18[0xc];
	s_location field_24;
	vector3f field_2c;
};

struct s_2641c0
{
	byte field_0[0xc];
	point3f field_c;
	byte field_18[0x20];
};

void __stdcall function_2640c0(s_2640c0 *arg_0, long arg_1);
bool __stdcall function_2641c0(long arg_0, s_2641c0 *arg_1, point3f const *arg_2);
void __stdcall function_264330(long arg_0, long arg_1, s_2641c0 *arg_2, s_2640c0 *arg_3, bool arg_4);
real __stdcall function_265d30(long actor_index, long prop_index);

#endif
