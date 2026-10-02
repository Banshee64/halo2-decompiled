#pragma once

// the page allocator interface c_page_heap implements; its destructor is
// library code in retail (0x329e10, vtable 0x41661c)
class c_page_allocator
{
public:
	virtual void free_pages(byte *address, long size, long unused) = 0;
	virtual long get_page_size() = 0;
	virtual void *allocate_pages(long alignment_mask, long size, long unused) = 0;
	virtual ~c_page_allocator();
};
