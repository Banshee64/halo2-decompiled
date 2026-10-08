#pragma once

struct s_13eeb1
{
	long field_0;
	__forceinline operator long() const { return field_0; }
};

typedef char s_13eeb2[(sizeof(s_13eeb1) == sizeof(long)) ? 1 : -1];
