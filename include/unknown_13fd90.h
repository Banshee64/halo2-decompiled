/* UNKNOWN_13FD90.H: unicode.obj types */

#ifndef UNKNOWN_13FD90_H
#define UNKNOWN_13FD90_H

struct utf32
{
	long value;

	utf32() {}
	utf32(const volatile utf32 &other) : value(other.value) {}
};

#endif
