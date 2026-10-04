// @flags /O2 /Gr
#include "cseries.h"
#include "globals.h"
#include "engine_peer.h"

struct s_player_view
{
	byte unknown00[0xc0];
	char slot;
};

// the per-slot score table of the multiplayer globals (0x304)
struct s_slot_view
{
	byte unknown00[4];
	short value;
	byte unknown06[0x1c - 6];
};

// @retail 0x19fc70
long function_19fc70(dword player_index)
{
	c_engine_peer *engine = g_55e4d0[g_4e9ae8->engine_index];
	byte *base = 0;
	if (engine)
	{
		base = (byte *)g_4e9ae8 + 0x304;
	}
	long result = 0;

	if (base)
	{
		byte *player = g_4e8c24->data + (player_index & 0xffff) * 0x21c;
		bool flag = false;
		if (engine)
		{
			flag = TEST_FIELD_BIT(g_4e6948->flags184.bit0);
		}
		if (flag)
		{
			char slot = ((s_player_view *)player)->slot;
			if (slot != -1)
			{
				result = *(short *)(base + slot * 18 + 0x1c4);
			}
		}
		else
		{
			result = ((s_slot_view *)base)[player_index & 0xffff].value;
		}
	}

	return result;
}

// @retail 0x19fd00
long function_19fd00(long index)
{
	long result = 0;
	long table[101] =
	{
		0x100030a, 0x100030b, 0x100030c, 0x100030d,
		0x100030e, 0x100030f, 0x1000310, 0x1000311,
		0x1000312, 0x1000313, 0x2000314, 0x2000315,
		0x2000316, 0x2000317, 0x2000318, 0x2000319,
		0x200031a, 0x200031b, 0x200031c, 0x200031d,
		0x200031e, 0x200031f, 0x2000320, 0x2000321,
		0x2000322, 0x2000323, 0x2000324, 0x2000325,
		0x2000326, 0x2000327, 0x2000328, 0x2000329,
		0x200032a, 0x200032b, 0x200032c, 0x200032d,
		0x200032e, 0x200032f, 0x2000330, 0x2000331,
		0x2000332, 0x2000333, 0x2000334, 0x2000335,
		0x2000336, 0x2000337, 0x2000338, 0x2000339,
		0x200033a, 0x200033b, 0x200033c, 0x200033d,
		0x200033e, 0x200033f, 0x2000340, 0x2000341,
		0x2000342, 0x2000343, 0x2000344, 0x2000345,
		0x2000346, 0x2000347, 0x2000348, 0x2000349,
		0x200034a, 0x200034b, 0x200034c, 0x200034d,
		0x200034e, 0x200034f, 0x2000350, 0x2000351,
		0x2000352, 0x2000353, 0x2000354, 0x2000355,
		0x2000356, 0x2000357, 0x2000358, 0x2000359,
		0x200035a, 0x200035b, 0x200035c, 0x200035d,
		0x200035e, 0x200035f, 0x2000360, 0x2000361,
		0x2000362, 0x2000363, 0x2000364, 0x2000365,
		0x2000366, 0x2000367, 0x2000368, 0x2000369,
		0x200036a, 0x200036b, 0x200036c, 0x200036d,
		0x300036e
	};

	if (index >= 0 && (dword)index < 101)
	{
		result = table[index];
	}
	return result;
}
