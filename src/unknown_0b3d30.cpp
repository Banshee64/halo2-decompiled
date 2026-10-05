// @flags /O2 /Gr
#include "unknown_11c920.h"
#include "globals.h"
#include "unknown_053310.h"
#include "online_tasks.h"
#include <xtl.h>
#include <xonline.h>
#include <wchar.h>

dword g_547610;
long g_547614;
long g_547618;
byte g_54761c;
long g_547620;
byte g_546a8c[1];

bool __stdcall function_b5e40(void *block);
void online_result_registration_clear(void);
void function_6b640(long task_index);
extern bool g_510580;
extern long g_510584;
extern dword g_5107d8;

// @retail 0xb3d30
void __stdcall function_b3d30(long stage)
{
	long size;

	if (stage > 1)
	{
		if (stage > 3)
		{
			return;
		}
		size = 0x400;
	}
	else
	{
		size = 0xa000;
	}

	g_547610 = (dword)physical_memory_malloc_fixed(size, PAGE_READWRITE);
	g_547614 = size;
}

// @retail 0xb3db0
void function_b3db0(void)
{
	if (g_547610)
	{
		function_b5e40(g_546a8c);
		g_547610 = 0;
		g_547614 = 0;
		g_547618 = 0;
		g_54761c = 0;
	}
}

// @retail 0xb4020
void function_b4020(void)
{
	g_54761c = 0;
	online_result_registration_clear();
	long task_index = g_510584;
	if (task_index != NONE)
	{
		function_6b640(task_index);
		g_510584 = NONE;
	}
	g_510580 = false;
}

// @retail 0xb4050
void function_b4050(void)
{
	g_547620++;
	long count = g_510580 ? g_5107d8 : 0;
	if (g_547620 >= 3 * count)
	{
		online_result_registration_clear();
		long task_index = g_510584;
		if (task_index != NONE)
		{
			function_6b640(task_index);
			g_510584 = NONE;
		}
		g_510580 = false;
		g_54761c = 0;
	}
}


// @retail 0xb45f0
long function_b45f0(long task_index, dword *total_size, dword *received_size)
{
    s_type_9df9da *task = online_task_try_get(task_index);
    BYTE *received = 0;
    ULONGLONG owner;
    FILETIME creation;
    if (XOnlineStorageDownloadToMemoryGetResults((XONLINETASK_HANDLE)task->handle, &received, received_size, total_size, &owner, &creation) < 0)
        task->flags |= 0x20;
    return 0;
}


extern bool g_50944e;
wchar_t const *g_4672dc = L"test";
void unicode_string_snprintf(word *buffer, long maximum_count, word const *format, ...);

// @retail 0xb4120
bool function_b4120(long kind, struct _XUID owner, wchar_t const *filename, wchar_t *arg_d4faef, dword *path_size)
{
    long facility;
    switch (kind)
    {
    case 0: facility = 4; break;
    case 1: facility = 5; break;
    default: facility = 3; break;
    }
    unsigned __int64 user_id = 0;
    unsigned __int64 team_id = 0;
    wchar_t prefix[0x100];
    wchar_t path[0x100];
    if (facility == 4)
    {
        if (!g_50944e)
            unicode_string_snprintf((word *)prefix, 0x100, (word const *)L"%d", 0x2651);
        else
        {
            wcsncpy(prefix, g_4672dc, 0xff);
            prefix[0xff] = 0;
        }
    }
    else
    {
        wcsncpy(prefix, L"", 0xff);
        prefix[0xff] = 0;
    }
    unsigned long length = 0;
    for (; length < 0x100; length++)
        if (!prefix[length]) break;
    if (length > 0)
        unicode_string_snprintf((word *)path, 0x100, (word const *)L"%s/%s", prefix, filename);
    else
    {
        wcsncpy(path, filename, 0xff);
        path[0xff] = 0;
    }
    if (facility == 3) team_id = owner.qwUserID;
    else user_id = owner.qwUserID;
    if (XOnlineStorageCreateServerPath(facility, user_id, team_id, path, arg_d4faef, path_size) < 0)
        return false;
    return true;
}
