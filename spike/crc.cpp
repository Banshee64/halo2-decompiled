/*
CRC.CPP: the LTCG feasibility spike's first match.

Halo CE's crc.c from the punpckhdq/halo decompilation (CC0), compiled as
C++. With XDK 5849's compiler under LTCG (/O2 /GL /Gr), build_crc_table and
crc_checksum_buffer match the retail Halo 2 XBE at 0x163c00 and 0x163ba0:

    python tools/match.py spike/crc.cpp "/O2 /Gr" \
        "?build_crc_table@@YIXPAK@Z=163c00" \
        "?crc_checksum_buffer@@YIXPAKPBXJ@Z=163ba0"

The loop counters must be short: with long ones the compiler unrolls
build_crc_table's inner loop.
*/

typedef unsigned char byte;

enum
{
	CRC_NEW = -1,
	CRC_TABLE_SIZE = 256,
	CRC32_POLYNOMIAL = 0xEDB88320
};

static void build_crc_table(unsigned long *crc_table);

void crc_new(
	unsigned long *crc_reference)
{
	*crc_reference = CRC_NEW;
}

void crc_checksum_buffer(
	unsigned long *crc_reference,
	void const *buffer,
	long buffer_size)
{
	static unsigned long crc_table[CRC_TABLE_SIZE];
	static bool crc_table_built;

	unsigned long crc;
	unsigned long temp1;
	unsigned long temp2;

	if (!crc_table_built)
	{
		build_crc_table(crc_table);
		crc_table_built = true;
	}

	crc = *crc_reference;

	while (buffer_size-- > 0)
	{
		temp1 = (crc >> 8) & 0x00FFFFFF;
		temp2 = crc_table[(crc ^ *((byte *)buffer)) & 0xFF];
		buffer = (byte *)buffer + 1;
		crc = temp1 ^ temp2;
	}

	*crc_reference = crc;
}

static void build_crc_table(
	unsigned long *crc_table)
{
	short table_index;

	for (table_index = 0; table_index < CRC_TABLE_SIZE; ++table_index)
	{
		unsigned long crc = table_index;
		short round;

		for (round = 0; round < 8; ++round)
		{
			if (crc & 1)
			{
				crc = (crc >> 1) ^ CRC32_POLYNOMIAL;
			}
			else
			{
				crc = crc >> 1;
			}
		}

		crc_table[table_index] = crc;
	}
}

/* ---------- harness: callers the inliner will not fold */

byte g_buffer[256];
volatile long g_size;
volatile unsigned long g_out;

void caller_a(void)
{
	unsigned long crc;
	crc_new(&crc);
	crc_checksum_buffer(&crc, g_buffer, g_size);
	g_out = crc;
}

void caller_b(void)
{
	unsigned long crc;
	crc_new(&crc);
	crc_checksum_buffer(&crc, g_buffer + 7, g_size - 3);
	crc_checksum_buffer(&crc, g_buffer + 9, g_size - 5);
	g_out = crc;
}

void caller_c(void)
{
	unsigned long crc = g_out;
	crc_checksum_buffer(&crc, g_buffer + 1, g_size);
	g_out = crc;
}

extern "C" int entry(void)
{
	caller_a();
	caller_b();
	caller_c();
	return 0;
}
