// @flags /O2 /Gr
#include "unknown_11c920.h"
#include <string.h>

struct s_player_status_source
{
	byte unknown00[0x4c];
	long value4c;
	byte value50;
	byte value51;
	byte value52;
	byte value53;
};

struct s_player_status_values
{
	long value;
	byte flags[4];
	long unknown08;
	long unknown0c;
};

// @retail 0x80de0
void function_080de0(s_player_status_values *values, const s_player_status_source *source)
{
	memset(values, 0, sizeof(*values));
	values->value = source->value4c;
	values->flags[0] = source->value50;
	values->flags[1] = source->value51;
	values->flags[2] = source->value52;
	values->flags[3] = source->value53;
}
