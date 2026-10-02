// @flags /O2 /Gr
#include "cseries.h"
#include "globals.h"


// @retail 0x1e4990
long function_1e4990(long index)
{
	long result = 0;
	if (index != NONE)
	{
		do
		{
			byte *data = g_4e3b44[index & 0xffff].bytes;
			if (*(long *)(data + 0x34) > 0)
			{
				result = *(long *)(data + 0x38);
				break;
			}
			index = *(long *)(data + 8);
		} while (index != NONE);
	}
	return result;
}

// @retail 0x1e49d0
long function_1e49d0(long index)
{
	long result = 0;
	if (index != NONE)
	{
		do
		{
			byte *data = g_4e3b44[index & 0xffff].bytes;
			if (*(long *)(data + 0x44) > 0)
			{
				result = *(long *)(data + 0x48);
				break;
			}
			index = *(long *)(data + 8);
		} while (index != NONE);
	}
	return result;
}

// @retail 0x1e4a10
long function_1e4a10(long index)
{
	long result = 0;
	if (index != NONE)
	{
		do
		{
			byte *data = g_4e3b44[index & 0xffff].bytes;
			if (*(long *)(data + 0x3c) > 0)
			{
				result = *(long *)(data + 0x40);
				break;
			}
			index = *(long *)(data + 8);
		} while (index != NONE);
	}
	return result;
}

// @retail 0x1e4a50
long function_1e4a50(long index)
{
	long result = 0;
	if (index != NONE)
	{
		do
		{
			byte *data = g_4e3b44[index & 0xffff].bytes;
			if (*(long *)(data + 0x5c) > 0)
			{
				result = *(long *)(data + 0x60);
				break;
			}
			index = *(long *)(data + 8);
		} while (index != NONE);
	}
	return result;
}

// @retail 0x1e4a90
long function_1e4a90(long index)
{
	long result = 0;
	if (index != NONE)
	{
		do
		{
			byte *data = g_4e3b44[index & 0xffff].bytes;
			if (*(long *)(data + 0xb4) > 0)
			{
				result = *(long *)(data + 0xb8);
				break;
			}
			index = *(long *)(data + 8);
		} while (index != NONE);
	}
	return result;
}

// @retail 0x1e4ad0
long function_1e4ad0(long index)
{
	long result = 0;
	if (index != NONE)
	{
		do
		{
			byte *data = g_4e3b44[index & 0xffff].bytes;
			if (*(long *)(data + 0xbc) > 0)
			{
				result = *(long *)(data + 0xc0);
				break;
			}
			index = *(long *)(data + 8);
		} while (index != NONE);
	}
	return result;
}

// @retail 0x1e4b10
long function_1e4b10(long index)
{
	long result = 0;
	if (index != NONE)
	{
		do
		{
			byte *data = g_4e3b44[index & 0xffff].bytes;
			if (*(long *)(data + 0xc4) > 0)
			{
				result = *(long *)(data + 0xc8);
				break;
			}
			index = *(long *)(data + 8);
		} while (index != NONE);
	}
	return result;
}
