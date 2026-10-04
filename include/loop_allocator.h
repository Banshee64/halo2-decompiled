/* LOOP_ALLOCATOR.H: types shared by the memory, voice and callback-table sources and stubs, the
   callbacks and vtable at 0x453c00 */

#ifndef LOOP_ALLOCATOR_H
#define LOOP_ALLOCATOR_H

#include "screen_widgets.h"
#include "unknown_11c920.h"

/* the memory source of a loop allocator, and the allocators at 0x476fbc and
   0x47d924 (objects whose first dword is the vtable) */
class c_memory_source
{
public:
	virtual void *allocate(long size) { return 0; }
	virtual void release(void *block) {}
};

/* a block of a loop allocator: its size (header included), the pointer
   that owns it, and its neighbours; with debug headers, a 'head' header
   (file, line, time) comes first */
struct s_loop_block
{
	long size;
	void **owner;
	s_loop_block *next;
	s_loop_block *previous;
};

struct s_loop_block_debug_header
{
	dword signature;
	char const *file;
	long line;
	dword time;
};

/* a "loop" allocator (0x40 byte header, constructed by 0x18e250; its pool
   follows, aligned to 16 bytes, so the sources allocate 0x50 more bytes) */
struct s_loop_allocator
{
	dword signature;
	char name[0x20];
	c_memory_source *source;
	byte *base;
	long size;
	long free;
	s_loop_block *first;
	s_loop_block *last;
	byte field3c;
	byte field3d;
	byte field3e;
	bool debug_headers;
};

s_loop_allocator *function_18e1f0(c_memory_source *source, long size, const char *name);
void function_18e250(s_loop_allocator *loop, long size, const char *name, c_memory_source *source);
void function_18e230(s_loop_allocator *loop);
bool function_18eea0(long stage);

/* the class of the vtable at 0x453c48 (slot 0 is the deleting destructor at
   0x18f3d4), derived from a base whose destructor is 0x18f3f0; the slots
   after 0x24a01f are not decompiled yet */
/* the list's item widgets (screen_widgets.h) */
typedef c_class_14750b c_unknown_249fa3_entry;

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
