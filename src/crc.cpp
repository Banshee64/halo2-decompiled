// @flags /O2 /Ob1 /Gr
/* CRC.CPP: CRC-32 checksums (from Halo CE's crc.c, punpckhdq/halo, CC0)

The loop counters must be short: with long ones the compiler unrolls
function_163c00's inner loop. crc.cpp is compiled /Ob1: with /Ob2 the
compiler inlines function_163ba0 into function_123d40, which retail
does not. */

#include "unknown_11c920.h"
#include "crc.h"

enum
{
	CRC_NEW = -1,
	CRC_TABLE_SIZE = 256,
	CRC32_POLYNOMIAL = 0xEDB88320
};

PRIVATE void function_163c00(dword *crc_table);

void function_x86aaf2(
	dword *crc_reference)
{
	*crc_reference = CRC_NEW;
}

// @retail 0x163ba0
void function_163ba0(
	dword *crc_reference,
	void const *buffer,
	long buffer_size)
{
	static dword crc_table[CRC_TABLE_SIZE];
	static bool crc_table_built;

	dword crc;
	dword temp1;
	dword temp2;

	if (!crc_table_built)
	{
		function_163c00(crc_table);
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

// @retail 0x163c00
PRIVATE void function_163c00(
	dword *crc_table)
{
	short table_index;

	for (table_index = 0; table_index < CRC_TABLE_SIZE; ++table_index)
	{
		dword crc = table_index;
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
