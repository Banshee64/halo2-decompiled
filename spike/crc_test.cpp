/*
CRC_TEST.CPP: callers for crc.cpp's functions, so the linker keeps them
out of line.
*/

typedef unsigned char byte;

void crc_new(unsigned long *crc_reference);
void crc_checksum_buffer(unsigned long *crc_reference, void const *buffer, long buffer_size);

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
