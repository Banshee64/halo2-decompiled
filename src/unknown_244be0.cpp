#include "unknown_11c920.h"

// @flags /O2 /arch:SSE /Gr

struct __declspec(align(16)) s_query_result_244be0
{
	byte field_00[0x14];
	real field_14;
	byte field_18[8];
};

class c_query_callback_244be0
{
public:
	virtual void v0(void const *context, s_query_result_244be0 const *result) {}
};

class c_query_244be0
{
public:
	virtual void function_244be0(void const *query, void const *context, c_query_callback_244be0 *callback);
};

// @retail 0x244be0
void c_query_244be0::function_244be0(void const *query, void const *context, c_query_callback_244be0 *callback)
{
	s_query_result_244be0 result;
	result.field_14 = 1.0f;
	callback->v0(context, &result);
}
