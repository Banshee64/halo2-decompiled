#pragma once

// By-value wrappers: under the register convention a struct argument is
// passed on the stack, giving the all-stack layout retail has.
struct playback_arg
{
	void *p;
	playback_arg(const volatile playback_arg &o) : p(o.p) {}
};

struct playback_dest_arg
{
	byte *p;
	playback_dest_arg(const volatile playback_dest_arg &o) : p(o.p) {}
};

template <class T>
struct playback_cursor_arg
{
	T **p;
	playback_cursor_arg(const volatile playback_cursor_arg &o) : p(o.p) {}
};
