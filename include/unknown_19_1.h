/* UNKNOWN_19_1.H: types shared by the sources and stubs of batch 19-1, the
   callbacks and vtable at 0x453c00 */

#ifndef UNKNOWN_19_1_H
#define UNKNOWN_19_1_H

#include "cseries.h"

/* the memory source of a loop allocator, and the allocators at 0x476fbc and
   0x47d924 (objects whose first dword is the vtable) */
class c_memory_source
{
public:
	virtual void *allocate(long size) { return 0; }
	virtual void release(void *block) {}
};

/* a "loop" allocator (0x50 byte header, constructed by 0x18e250) */
struct s_loop_allocator
{
	byte unknown00[0x3c];
	byte field3c;
	byte field3d;
	byte field3e;
	byte unknown3f[0x11];
};

s_loop_allocator *function_18e1f0(c_memory_source *source, long size, const char *name);
void function_18e250(s_loop_allocator *loop, long size, const char *name, c_memory_source *source);
void function_18e230(s_loop_allocator *loop);
bool function_18eea0(long stage);

/* the class of the vtable at 0x453c48 (slot 0 is the deleting destructor at
   0x18f3d4), derived from a base whose destructor is 0x18f3f0; the slots
   after 0x24a01f are not decompiled yet */
struct c_unknown_249fa3_entry
{
	byte unknown00[0x80];
	~c_unknown_249fa3_entry();
};

class c_unknown_249fa3_base
{
public:
	virtual ~c_unknown_249fa3_base();
};

class c_unknown_249fa3 : public c_unknown_249fa3_base
{
public:
	virtual ~c_unknown_249fa3();
	virtual void slot1();
	virtual void slot2();
	virtual void slot3();
	virtual void slot4() {}
	virtual void slot5() {}
	virtual void slot6() {}
	virtual void slot7() {}
	virtual void slot8() {}
	virtual void slot9() {}
	virtual void slot10() {}
	virtual void slot11() {}
	virtual void slot12() {}
	virtual void slot13() {}
	virtual void slot14() {}
	virtual void slot15() {}
	virtual void slot16() {}
	virtual void slot17() {}
	virtual void slot18() {}
	virtual void slot19() {}
	virtual void slot20() {}
	virtual void slot21() {}
	virtual void slot22() {}
	virtual void slot23() {}
	virtual void slot24() {}

	byte unknown04[0x86];
	bool field8a;
	byte unknown8b;
	long field8c;
	byte unknown90[0x18];
	c_unknown_249fa3_entry entries[4];
};

#endif
