/* UNKNOWN_13FD90.H: unicode character and conversion types */

#ifndef UNKNOWN_13FD90_H
#define UNKNOWN_13FD90_H

struct utf32
{
	long value;
};

void unicode_string_copy(word *destination, const word *source, long maximum_count);

#endif
