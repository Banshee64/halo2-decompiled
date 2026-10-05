#include "unknown_11c920.h"
#include "screen_widgets.h"
#include "data_array.h"
#include <wchar.h>
#include <string.h>

// @flags /O1 /Oi /Gr

struct s_entry_24a64d
{
	long index;
	byte field_04[8];
	word field_0c[32];
	char field_4c[16];
	byte field_5c[0x11c - 0x5c];
	byte field_11c[0x1e4 - 0x11c];
	word field_1e4[50];
};

class c_list_24a64d : public c_list_widget_with_items
{
public:
	virtual void *function_24a64d(long index);
	virtual void v24();
	virtual word const *function_24a691(long index, bool append);
	bool function_24a2e7(long controller);
	byte field_88;
	bool field_89;
};

short player_slot_count_active(void);
bool function_148c3e(long controller, long type);

// @retail 0x24a2e7
bool c_list_24a64d::function_24a2e7(long controller)
{
	long type = 0;
	if (!field_89)
		type = player_slot_count_active() ? 2 : 1;
	return function_148c3e(controller, type);
}

// @retail 0x24a64d
void *c_list_24a64d::function_24a64d(long index)
{
	long count;
	s_entry_24a64d *entries = (s_entry_24a64d *)get_items(&count);
	void *result = 0;
	for (long i = 0; i < count; ++i)
	{
		if (entries[i].index == index)
		{
			result = (byte *)&entries[i] + 0x11c;
			break;
		}
	}
	return result;
}

struct s_datum_24a763
{
	long field_00;
	long index;
};

// @retail 0x24a763
s_entry_24a64d *function_24a763(c_list_24a64d *list)
{
	s_entry_24a64d *result = 0;
	s_datum_24a763 *datum = (s_datum_24a763 *)record_pool_lookup(list->data, list->get_focused_datum());
	if (datum && datum->index != NONE)
	{
		long count;
		s_entry_24a64d *entries = (s_entry_24a64d *)list->get_items(&count);
		for (long i = 0; i < count; ++i)
		{
			if (entries[i].index == datum->index)
			{
				result = &entries[i];
				break;
			}
		}
	}
	return result;
}

class c_screen_24a7bf : public c_screen_with_menu
{
public:
	virtual void v19();
	s_entry_24a64d *function_24a828();
	bool function_24a855();
	c_list_24a64d field_614;
	byte field_6a0[0x176c - 0x614 - sizeof(c_list_24a64d)];
	s_entry_24a64d field_176c;
	bool field_19b4;
};

bool network_session_manager_session_unready(void);
void function_253c3a(long block_index, long index, c_text_widget_45a5e0 *widget);

// @retail 0x24a7bf
void c_screen_24a7bf::v19()
{
	c_text_widget_45a5e0 *text = (c_text_widget_45a5e0 *)find_child(6, 1, true);
	long index;
	if (network_session_manager_session_unready())
		index = 1;
	else
		index = 0;
	function_253c3a(0, index, text);
	c_class_1473c9::v19();
}

// @retail 0x24a855
bool c_screen_24a7bf::function_24a855()
{
	s_datum_24a763 *datum = (s_datum_24a763 *)record_pool_lookup(field_614.data, field_614.get_focused_datum());
	if (datum)
		return field_19b4 = datum->index == NONE;
	return field_19b4;
}

// @retail 0x24a828
s_entry_24a64d *c_screen_24a7bf::function_24a828()
{
	s_entry_24a64d *result = function_24a763(&field_614);
	if (result)
		field_176c = *result;
	else
		result = &field_176c;
	return result;
}

void unicode_string_copy(word *destination, word const *source, long maximum_count);
void ascii_string_to_unicode(char const *source, word *destination, long maximum_count);

// @retail 0x24a691
word const *c_list_24a64d::function_24a691(long index, bool append)
{
	long count;
	s_entry_24a64d *entries = (s_entry_24a64d *)get_items(&count);
	s_entry_24a64d *volatile local_entries = entries;
	word const *result = (word const *)L"";
	for (long i = 0; i < count; ++i)
	{
		if (entries[i].index == index)
		{
			if (append && entries[i].field_4c[0])
			{
				word suffix[16] = { 0 };
				ascii_string_to_unicode(entries[i].field_4c, suffix, 15);
				word *buffer = entries[i].field_1e4;
				unicode_string_copy(buffer, entries[i].field_0c, 0x31);
				wcsncat((wchar_t *)buffer, L" (", 0x31);
				wcsncat((wchar_t *)buffer, (wchar_t *)suffix, 0x31);
				wcsncat((wchar_t *)buffer, L")", 0x31);
				local_entries[i].field_1e4[49] = 0;
				result = buffer;
			}
			else
				result = entries[i].field_0c;
			break;
		}
	}
	return result;
}

void function_1630b0(long *values, long value);
bool function_1a0540(s_player_profile_settings *settings, long file_index);
word *function_215b50(long file_index, word *name);

// @retail 0x24a52b
void c_list_24a64d::v24()
{
	long count;
	s_entry_24a64d *entries = (s_entry_24a64d *)get_items(&count);
	long capacity = count;
	long indices[16];
	function_1630b0(indices, NONE);
	long i = 0;
	for (c_class_1a2c81 *item = child; item && i < 16; item = item->next, ++i)
	{
		long datum_index = ((c_class_14750b *)item)->value70;
		if (datum_index != NONE)
		{
			s_datum_24a763 *datum = (s_datum_24a763 *)record_pool_lookup(data, datum_index);
			if (datum)
				indices[i] = datum->index;
		}
	}
	for (i = 0; i < count; ++i)
	{
		long index = entries[i].index;
		long j;
		for (j = 0; j < 16; ++j)
		{
			if (index == indices[j] && index != g_54e5d0.profile_index)
			{
				indices[j] = NONE;
				break;
			}
		}
		if (j == 16 && index != NONE)
			entries[i].index = NONE;
	}
	for (i = 0; i < 16; ++i)
	{
		long index = indices[i];
		if (index != NONE)
		{
			long j;
			for (j = 0; j < capacity; ++j)
				if (entries[j].index == NONE)
					break;
			if (j >= 0 && j < capacity)
			{
				entries[j].index = index;
				s_player_profile_settings *settings = (s_player_profile_settings *)((byte *)&entries[j] + 4);
				if (index == g_54e5d0.profile_index)
					*settings = g_54e5d0.settings;
				else if (!function_1a0540(settings, index))
				{
					memset(settings, 0, sizeof(*settings));
					function_215b50(entries[j].index, entries[j].field_0c);
				}
			}
		}
	}
}
