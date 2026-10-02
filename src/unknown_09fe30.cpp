#include "cseries.h"
#include "globals.h"
#include "unknown_09fe30.h"

// @flags /O2 /arch:SSE /Gr

/* a view of the object header array of g_4e0300 (12 bytes each, the object
   pointer at +8) */
struct s_object_header_view
{
	byte unknown00[8];
	byte *object;
};

#define OBJECT_FROM_INDEX(index) \
	(((s_object_header_view *)g_4e0300->headers)[(index) & 0xffff].object)

/* a view of the tag instances of g_4e3b44 (data pointer at +8) */
struct s_tag_instance_view
{
	byte unknown00[8];
	byte *data;
	byte unknown0c[4];
};

#define TAG_DATA_FROM_INDEX(index) \
	(((s_tag_instance_view *)g_4e3b44)[(index) & 0xffff].data)

/* the vehicle relevance table (0x4cef68, 0x4c bytes per entry) */
struct s_relevance_entry
{
	real threshold;
	long parameter;
	byte unknown08[0x44];
};

s_relevance_entry g_4cef68[1];

struct s_object_data_view
{
	byte unknown00[8];
	long unknown08;
	byte unknown0c[0xb8];
};

struct s_object_creation
{
	c_vehicle_type *owner;
	s_object_data_view data;
};

/* callees not decompiled yet */
extern real function_aa4d0(long a, void *request, long parameter, long b, long c);
extern char *csnprintf(char *buffer, long size, const char *format, ...);
extern void function_a6660(s_stream_view *stream);
extern void function_1955d0(void *destination, const void *source, long size);
extern bool function_a6810(s_reader_view *reader);
extern void function_195820(void *destination, long size);
extern void function_a5d90(void *data, s_object_spec *spec, void *c, long e);
extern long function_a73b0(s_object_spec *spec);
extern long function_b7b40(void *creation);
extern void function_b9b90(void *object, bool flag, long index);

// @retail 0x9fe30
long c_vehicle_type::get_size()
{
	return 12;
}

// @retail 0xa09a0
const char *c_vehicle_type::get_name()
{
	return "vehicle";
}

// @retail 0xa0880
void c_vehicle_type::v9(long a, long b, long *result)
{
	*result = 0x85;
}

// @retail 0xa0930
void c_vehicle_type::v21(s_object_link *link)
{
	if (link->object_index != NONE)
	{
		byte *object = OBJECT_FROM_INDEX(link->object_index);
		*(long *)(object + 0xd4) = link->value;
	}
}

// @retail 0xa0700
void c_vehicle_type::v26(long object_index, long unused, s_object_spec *spec)
{
	byte *first = OBJECT_FROM_INDEX(object_index);
	byte *object;
	byte *definition;

	object_spec_clear(spec);

	object = OBJECT_FROM_INDEX(object_index);
	spec->unknown04 = *(long *)object;
	spec->unknown00 = *(short *)(object + 0x1a);
	spec->byte08 = object[0xaf];
	spec->unknown0c = *(long *)(object + 0x24);

	definition = TAG_DATA_FROM_INDEX(*(long *)first);
	if (first[0xb1] != 0xff && *(long *)(definition + 0x38) != NONE)
	{
		byte *tag = TAG_DATA_FROM_INDEX(*(long *)(definition + 0x38));
		spec->unknown10 = *(long *)(*(byte **)(tag + 0x54) + (char)first[0xb1] * 0x38);
	}
	else
	{
		spec->unknown10 = 0;
	}
}
// @retail 0xa07f0
long c_vehicle_type::v29(long a, s_object_spec *spec, void *c, long d, long e)
{
	s_object_creation creation;
	long index;

	creation.owner = this;
	function_a5d90(&creation.data, spec, c, e);
	creation.data.unknown08 = spec->unknown10;
	if (spec->unknown00 != NONE)
		index = function_a73b0(spec);
	else
		index = function_b7b40(&creation);
	if (index != NONE)
	{
		byte *object = OBJECT_FROM_INDEX(index);
		object[0xaf] = spec->byte08;
		function_b9b90(object, true, index);
	}
	return index;
}

// @retail 0xa0900
void c_vehicle_type::v12(long a, s_stream_view *stream, long c, const void *data)
{
	function_a6660(stream);
	function_1955d0(stream->buffer, data, 0x20);
}

// @retail 0xa0a60
bool c_vehicle_type::v13(long a, s_stream_view *stream, s_reader_view *reader)
{
	bool ok = function_a6810(reader);

	function_195820(stream->buffer, 0x20);
	return reader->position <= (reader->size << 3) && ok;
}

// @retail 0xa09f0
void c_vehicle_type::v10(s_vehicle_request *request, long parameter, char *buffer, long size)
{
	real relevance = -1.0f;
	s_relevance_entry *entry = &g_4cef68[request->type];

	if (!(entry->threshold > g_45dbd8))
		relevance = function_aa4d0(1, request, entry->parameter, parameter, 0);
	csnprintf(buffer, size, "vehicle creation: relevance=%5.3f", relevance);
}

// @retail 0x2bcc80
bool c_vehicle_type::v30(long a)
{
	return false;
}