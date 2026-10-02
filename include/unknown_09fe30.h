/* UNKNOWN_09FE30.H: the vehicle object type (vtable at 0x4520f0) */

#ifndef UNKNOWN_09FE30_H
#define UNKNOWN_09FE30_H

#include "cseries.h"

struct s_object_spec
{
	long unknown00;
	long unknown04;
	union
	{
		long unknown08;
		byte byte08;
	};
	long unknown0c;
	long unknown10;

};

inline void object_spec_clear(s_object_spec *spec)
{
	spec->unknown00 = 0;
	spec->unknown04 = 0;
	spec->unknown08 = 0;
	spec->unknown0c = 0;
	spec->unknown10 = 0;
}

struct s_object_link
{
	long value;
	long unknown04;
	long object_index;
};

struct s_vehicle_request
{
	byte unknown00[4];
	short type;
};

struct s_stream_view
{
	byte unknown00[0x10];
	byte buffer[0x20];
};

struct s_reader_view
{
	byte unknown00[4];
	long size;
	byte unknown08[8];
	long position;
};

class c_vehicle_type
{
public:
	virtual long get_size();
	virtual const char *get_name();
	virtual void v2() {}
	virtual void v3() {}
	virtual void v4() {}
	virtual void v5() {}
	virtual void v6() {}
	virtual void v7() {}
	virtual void v8() {}
	virtual void v9(long a, long b, long *result);
	virtual void v10(s_vehicle_request *request, long parameter, char *buffer, long size);
	virtual void v11() {}
	virtual void v12(long a, s_stream_view *stream, long c, const void *data);
	virtual bool v13(long a, s_stream_view *stream, s_reader_view *reader);
	virtual void v14() {}
	virtual void v15() {}
	virtual void v16() {}
	virtual void v17() {}
	virtual void v18() {}
	virtual void v19() {}
	virtual void v20() {}
	virtual void v21(s_object_link *link);
	virtual void v22() {}
	virtual void v23() {}
	virtual void v24() {}
	virtual void v25() {}
	virtual void v26(long object_index, long unused, s_object_spec *spec);
	virtual void v27() {}
	virtual void v28() {}
	virtual long v29(long a, s_object_spec *spec, void *c, long d, long e);
	virtual bool v30(long a);
	virtual void v31() {}
	virtual void v32() {}
	virtual void v33() {}
	virtual void v34() {}
	virtual void v35() {}
};

#endif