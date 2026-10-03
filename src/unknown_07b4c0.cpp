// @flags /O2 /Ob1 /Gr
#include "cseries.h"
#include "globals.h"
#include "data_array.h"
#include <xtl.h>
#include <string.h>

/* QoS (quality of service) probe handles: a data array of 8-byte elements
   {salt, state, XNQOS *} kept in g_4cf8d8 */
struct s_qos_handle
{
	short salt;
	short state;
	XNQOS *qos;
};

struct s_qos_target
{
	XNKID kid;
	XNKEY key;
	XNADDR xna;
};

struct s_qos_result
{
	long probes_sent;
	long probes_received;
	long rtt_minimum;
	long rtt_median;
	long upstream_bits_per_second;
	long downstream_bits_per_second;
	long data_size;
	byte *data;
};

struct s_session_description
{
	byte unknown0[2];
	short field2;
	long field4;
	long field8;
	long fieldc;
	short field10;
	short field12;
	short field14;
	byte unknown16[0x7e];
	short field94;
	short field96;
	short field98;
	short field9a;
	short field9c;
	short field9e;
	byte unknowna0[8];
	long fielda8;
	byte unknownac[8];
	long fieldb4;
	long fieldb8;
	short fieldbc;
	byte unknownbe[0x622];
	dword field6e0;
};


static const long g_440188[2] = { 8, 0 };
static const long g_440190[2] = { 16384, 4096 };

/* stand-in: retail reloads g_4cf8d8 after datum_new because some function not yet decompiled
   (probably the QoS initializer) takes its address; remove this once that function exists */
PRIVATE s_data_array **qos_handle_array(void)
{
	return &g_4cf8d8;
}

static __forceinline s_qos_handle *qos_handle_get(long handle)
{
	s_data_array *data = g_4cf8d8;
	long index = handle & 0xffff;

	if (index < data->high_water_index)
	{
		s_qos_handle *element = (s_qos_handle *)(data->data + data->size * index);

		if (element->salt && element->salt == (handle >> 16))
		{
			return element;
		}
	}

	return 0;
}

// @retail 0x7b4c0
long qos_lookup(long kind, long count, long bits_per_second, s_qos_target *targets)
{
	long handle = NONE;

	if (g_4cf8d4)
	{
		long probes = g_440188[kind];
		long bandwidth = bits_per_second;
		XNQOS *qos;
		const XNKEY *keys[64];
		const XNKID *kids[64];
		const XNADDR *addresses[64];

		if (bits_per_second == NONE)
		{
			bandwidth = g_440190[kind];
		}

		if (count > 64)
		{
			count = 64;
		}

		for (long i = 0; i < count; i++)
		{
			kids[i] = &targets[i].kid;
			keys[i] = &targets[i].key;
			addresses[i] = &targets[i].xna;
		}

		if (XNetQosLookup(count, addresses, kids, keys, 0, 0, 0, probes, bandwidth, 0, 0, &qos) == 0)
		{
			handle = datum_new(g_4cf8d8);
			if (handle != NONE)
			{
				s_qos_handle *element = &((s_qos_handle *)g_4cf8d8->data)[handle & 0xffff];

				element->state = 1;
				element->qos = qos;
			}
			else
			{
				XNetQosRelease(qos);
			}
		}
	}

	return handle;
}

// @retail 0x7b5e0
long qos_service_lookup(void)
{
	long handle = NONE;

	if (g_4cf8d4)
	{
		XNQOS *qos;

		if (XNetQosServiceLookup(0, 0, &qos) == 0)
		{
			handle = datum_new(g_4cf8d8);
			if (handle != NONE)
			{
				s_qos_handle *element = &((s_qos_handle *)g_4cf8d8->data)[handle & 0xffff];

				element->state = 1;
				element->qos = qos;
			}
			else
			{
				XNetQosRelease(qos);
			}
		}
	}

	return handle;
}

// @retail 0x7b650
void qos_release(long handle)
{
	if (g_4cf8d4 && handle != NONE)
	{
		s_qos_handle *element = qos_handle_get(handle);

		if (element)
		{
			if (XNetQosRelease(element->qos) == 0)
			{
				element->qos = 0;
			}

			datum_delete(g_4cf8d8, handle);
		}
	}
}

// @retail 0x7b6c0
bool qos_is_complete(long handle)
{
	bool result = true;

	if (g_4cf8d4 && handle != NONE)
	{
		s_qos_handle *element = qos_handle_get(handle);

		if (element)
		{
			result = element->qos->cxnqosPending == 0;
		}
	}

	return result;
}

// @retail 0x7b720
long qos_target_status(long handle, long index)
{
	long result = 3;

	if (g_4cf8d4 && handle != NONE)
	{
		s_qos_handle *element = qos_handle_get(handle);

		if (element)
		{
			XNQOSINFO *info = &element->qos->axnqosinfo[index];

			if (info->bFlags & XNET_XNQOSINFO_COMPLETE)
			{
				if (info->bFlags & XNET_XNQOSINFO_TARGET_DISABLED)
				{
					result = 4;
				}
				else
				{
					result = (info->bFlags & XNET_XNQOSINFO_TARGET_CONTACTED) ? 5 : 3;
				}
			}
			else if (info->bFlags & XNET_XNQOSINFO_PARTIAL_COMPLETE)
			{
				result = 2;
			}
			else
			{
				result = info->cProbesXmit > 0;
			}
		}
	}

	return result;
}

// @retail 0x7b7b0
bool qos_target_result(long handle, s_qos_result *result, long index)
{
	bool success = false;

	if (g_4cf8d4 && handle != NONE)
	{
		s_qos_handle *element = qos_handle_get(handle);

		if (element)
		{
			XNQOSINFO *info = &element->qos->axnqosinfo[index];

			if ((info->bFlags & XNET_XNQOSINFO_TARGET_CONTACTED) && (info->bFlags & (XNET_XNQOSINFO_COMPLETE | XNET_XNQOSINFO_PARTIAL_COMPLETE)))
			{
				memset(result, 0, sizeof(*result));

				result->probes_sent = info->cProbesXmit;
				result->probes_received = info->cProbesRecv;
				result->rtt_minimum = info->wRttMinInMsecs;
				result->rtt_median = info->wRttMedInMsecs;
				result->upstream_bits_per_second = info->dwUpBitsPerSec;
				result->downstream_bits_per_second = info->dwDnBitsPerSec;
				if (info->bFlags & XNET_XNQOSINFO_DATA_RECEIVED)
				{
					result->data_size = info->cbData;
					result->data = info->pbData;
				}
				success = true;
			}
		}
	}

	return success;
}

// @retail 0x7b880
bool session_description_valid(const s_session_description *description)
{
	if (!description)
	{
		return false;
	}
	if (description->field2 < 0 || description->field2 >= 2)
	{
		return false;
	}
	if (description->field4 < 0 || description->field4 >= 5)
	{
		return false;
	}
	if (description->field8 != -1 && (description->field8 <= 0 || description->field8 > 0xfffe))
	{
		return false;
	}
	if (description->fieldc != -1 && (description->fieldc <= 0 || description->field8 > 0xfffe))
	{
		return false;
	}
	if (description->field10 < 0 || description->field10 >= 3)
	{
		return false;
	}
	if (description->field12 < 0 || description->field12 >= 3)
	{
		return false;
	}
	if (description->field14 < 0 || description->field14 >= 3)
	{
		return false;
	}
	if (description->field94 < 0 || description->field94 > 16)
	{
		return false;
	}
	if (description->field96 < 0 || description->field96 > 16)
	{
		return false;
	}
	if (description->field98 < 0 || description->field98 > 16)
	{
		return false;
	}
	if (description->field9a < 0 || description->field9a > 16)
	{
		return false;
	}
	if (description->field9c < 0 || description->field9c >= 5)
	{
		return false;
	}
	if (description->field9e < 0 || description->field9e >= 10)
	{
		return false;
	}
	if (description->fieldb4 < 0 || description->fieldb4 >= 3)
	{
		return false;
	}
	if (description->fieldb8 < 0)
	{
		return false;
	}
	if (description->fieldbc < 0 || description->fieldbc > 16)
	{
		return false;
	}
	if (description->field6e0 & 0xffffff00)
	{
		return false;
	}
	if (description->fielda8 != -1 && (description->fielda8 < 0 || description->fielda8 >= 4))
	{
		return false;
	}

	return true;
}
