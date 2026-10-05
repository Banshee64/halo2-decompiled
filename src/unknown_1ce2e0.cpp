// @flags /O2 /arch:SSE /Gr
#include "unknown_11c920.h"

struct c_class_1ce2e0
{
	byte unknown00[4];
	byte value;

	byte *copy_value(byte *output) const;
};

// @retail 0x1ce2e0
byte *c_class_1ce2e0::copy_value(byte *output) const
{
	// Retail keeps the output pointer on the stack.
	byte *const *output_reference = &output;

	**output_reference = value;
	return *output_reference;
}
