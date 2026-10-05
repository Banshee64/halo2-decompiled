// @flags /O2 /Gr
#include "unknown_11c920.h"
#include <string.h>

struct s_081550_fields
{
	byte unknown00[0x44];
	bool flag44;
	byte unknown45[3];
	long value48;
	long value4c;
	long value50;
	byte data54[0x20];
	byte unknown74;
	bool flag75;
	byte unknown76[0x9c - 0x76];
	long value9c;
	long valuea0;
};

// @retail 0x81550
void function_081550(s_081550_fields *fields)
{
	fields->flag44 = false;
	fields->value48 = 0;
	fields->value4c = 0;
	fields->value50 = 0;
	memset(fields->data54, 0, sizeof(fields->data54));
	fields->flag75 = false;
	fields->value9c = 0;
	fields->valuea0 = 0;
}
