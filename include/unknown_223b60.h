/* UNKNOWN_223B60.H: types for the timing counter, the S3TC helpers and the
resource fixup group */

#ifndef UNKNOWN_223B60_H
#define UNKNOWN_223B60_H

struct timing_counter
{
	__int64 total;
	__int64 start;
	bool stopped;
};

struct S3TC_COLOR
{
	byte b, g, r, a;
};

struct s_fixup_entry
{
	byte type;
	byte unknown1;
	word unknown2;
	word target_offset;
	word target_index;
	long value;
	long offset;
};

struct s_fixup_pointer
{
	long count;
	byte *pointer;
};

struct s_fixup_trailer
{
	long count;
	dword packed;
	long unknown8;
};

struct s_fixup_element
{
	byte unknown0;
	byte divisor;
	short quotient;
	long unknown4;
	long base;
	long relative;
	s_fixup_trailer *trailer;
	s_fixup_trailer inline_trailer;
};

struct s_fixup_group
{
	byte unknown0[8];
	long data_offset;
	byte unknownc[4];
	long entry_count;
	s_fixup_entry *entries;
	byte unknown18[4];
	short pointer_offset;
};

void fixup_group_apply(s_fixup_group *group, byte *base);
bool fixup_group_has_resource(s_fixup_group *group, byte *base);
void fixup_group_release_resources(s_fixup_group *group, byte *base);

#endif
