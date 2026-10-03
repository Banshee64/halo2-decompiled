/* CACHE_FILES.H: the map cache file (cache_files.cpp) */
#ifndef CACHE_FILES_H
#define CACHE_FILES_H

#include "cseries.h"

struct s_cache_tag_group;
struct s_cache_tag_instance;

struct s_cache_header
{
	long header_signature;
	long version;
	long size;
	dword unknown0c;
	dword tag_data_offset;
	dword tag_data_size;
	dword unknown18;
	long unknown1c;
	byte unknown20[0x120];
	short type;
	byte unknown142[0x56];
	char name[32];
	byte unknown1b8[0x644];
	long footer_signature;
};

struct s_cache_tags_header
{
	s_cache_tag_group *groups;
	long group_count;
	s_cache_tag_instance *instances;
	long scenario_index;
	long globals_index;
	dword unknown14;
	long instance_count;
	long signature;
};

struct s_structure_bsp_header
{
	dword unknown00;
	void *bsp_address;
	void *lightmap_address;
	long signature;
};

struct s_cache_file_location
{
	long file_index;
	dword offset;
};

/* 0x5476f8 */
struct s_cache_file_globals
{
	bool loaded;
	void *tag_data;
	s_cache_header header;
	s_cache_tags_header *tags;
	s_structure_bsp_header *bsp;
};

extern s_cache_file_globals cache_file_globals;

#endif
