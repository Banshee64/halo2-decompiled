/* UNKNOWN_19B516.H: the widget class whose 24-slot vtable is at 0x4594e0
   (slots 22 and 23 are library/other code), and the types its methods use */

#ifndef UNKNOWN_19B516_H
#define UNKNOWN_19B516_H

#include "cseries.h"
#include "data_array.h"
#include <xtl.h>
#include <xonline.h>

bool xuid_equal(XUID const *a, XUID const *b, bool compare_guest_number);

struct s_id_triplet
{
	dword a;
	dword b;
	dword c;

	/* the triplet is an XUID; 0x63d00 is xuid_equal (network_session_interface.cpp) */
	bool function_63d00(dword *other, long flag)
	{
		return xuid_equal((XUID const *)this, (XUID const *)other, flag != 0);
	}
};

struct s_message
{
	byte unknown00[0xc];
	dword field_c;
	byte unknown10[0xc];
	void (__stdcall *callback)(s_message *message);
};

void function_149f49(word a, s_message *message, dword *id, word b, long c, long d, long e);

struct s_event
{
	long type;
	long unknown04;
	long param;
	byte unknown0c[0x64];
	long index;

	void function_251963();
};


struct s_text_interface
{
	virtual void v0() {}
	virtual void set_text(word *text) {}
};

struct c_text_widget
{
	virtual void v0() {}
	virtual void v1() {}
	virtual void v2() {}
	virtual void v3() {}
	virtual void v4() {}
	virtual void v5() {}
	virtual void v6() {}
	virtual void v7() {}
	virtual void v8() {}
	virtual void v9() {}
	virtual void v10() {}
	virtual void v11() {}
	virtual void v12() {}
	virtual void v13() {}
	virtual void v14() {}
	virtual s_text_interface *get_text() { return 0; }
};

/* slots 18 and 19 as 24bb60 calls them */
struct c_list_view
{
	virtual void s0() {}
	virtual void s1() {}
	virtual void s2() {}
	virtual void s3() {}
	virtual void s4() {}
	virtual void s5() {}
	virtual void s6() {}
	virtual void s7() {}
	virtual void s8() {}
	virtual void s9() {}
	virtual void s10() {}
	virtual void s11() {}
	virtual void s12() {}
	virtual void s13() {}
	virtual void s14() {}
	virtual void s15() {}
	virtual void s16() {}
	virtual void s17() {}
	virtual void *get_first() { return 0; }
	virtual long get_count() { return 0; }
};

class c_widget
{
public:
	virtual bool v0();
	virtual void v1();
	virtual void *v2();
	virtual long v3();
	virtual void v4(s_event *event, long unused);
	virtual bool v5(s_event *event);
	virtual void v6(c_widget *window, long row);
	virtual void v7(c_widget *child) {}
	virtual ~c_widget();
	virtual void v9();
	virtual void v10();
	virtual void v11();
	virtual void v12(long a);
	virtual void v13(s_event *event);
	virtual long v14();
	virtual void v15(c_widget *widget);
	virtual void v16();
	virtual void v17();
	virtual bool v18(s_event *event);
	virtual long v19();
	virtual long v20();
	virtual long v21();
	virtual void v22(s_event *event, long index) {}
	virtual void v23() {}

	/* code not decompiled yet (stubbed) */
	bool function_22e37f();
	void function_22ecb4(bool focus);
	c_widget *function_22eeee();
	bool function_22ef1b();
	void function_22e335();
	void function_22e391();
	void function_22e315();
	void function_22e89c(s_event *event);
	bool function_22ec73(s_event *event);
	bool function_24c3f8(s_event *event);
	void function_230134(long id, word *buffer);
	c_text_widget *function_22edb8(long type, long index, long flag);
	void function_22e9c6(short *bounds);

	byte unknown04[4];
	word m8;
	byte unknown0a[6];
	c_widget *parent;
	c_widget *child;
	c_widget *next;
	c_widget *prev;
	byte unknown20[0x30];
	dword m50;
	byte unknown54[0x19];
	byte m6d;
	byte m6e;
	byte unknown6f;
	s_data_array *m70;
	word m74;
	word m76;
	long m78;
	byte unknown7c[3];
	byte m7f;
	byte unknown80[4];
	byte sub84[4];
	byte sub88[0x818];
	byte m8a0;
	byte unknown8a1[3];
	long m8a4;
	long m8a8;
};

void *function_22ee92(void *item, word index);
void *function_22eb18(void *item);
void function_233f0f(long a, c_widget *widget);
word *function_1630e0(word *buffer, const word *format, ...);
bool function_22ed7a();
void function_24c0c4(c_widget *widget);
long function_24c610(void *a, c_widget *b);
bool function_24c63e(c_widget *widget);
bool function_24c676(c_widget *widget);
c_widget *function_24bae6(c_widget *widget);
void function_24c7e4(void *list, s_event **event, long *key);
void function_24c1c5(c_widget *widget, long direction);



/* the sprite pair drawn by 24bda2: the two elements are drawn from the tag's
   element at +0x48 */
struct s_sprite_element
{
	byte unknown00[4];
	short width;
	short height;
};

struct s_sprite
{
	s_sprite_element main;
	byte unknown08[0x74 - sizeof(s_sprite_element)];
	s_sprite_element second;
};

struct s_sprite_tag
{
	byte unknown00[0x48];
	s_sprite *sprite;
};

struct s_sprite_placement
{
	byte unknown00[8];
	long tag_index;
	short main_x;
	short main_y;
	short second_x;
	short second_y;
};

struct s_float_rect
{
	real x0;
	real x1;
	real y0;
	real y1;
};

struct s_bounds
{
	short a;
	short b;
	short c;
	short d;
};

class c_widget_handler : public c_widget
{
public:
	virtual void v24(long **a, long *b);
};

struct s_name_request;
void function_18ff47(long player, dword *out);
bool function_6c7e0();
bool function_199994();
bool function_1999b3();
bool function_1900a5(long player);
void unicode_string_to_ascii(const word *source, char *destination, long maximum_count);
void __stdcall function_148893(s_name_request *request, long flag);
void function_2363d4(long arg, short *b, short *a);
bool function_22f0ff(c_widget *widget);
long function_24c0b3(c_widget *widget);
s_sprite_placement *function_14837a(long id);
real function_22e9aa(c_widget *widget);
void function_23618e(s_float_rect *rect, real scale, long arg);
void function_235e5e(s_sprite_element *element, s_float_rect *from, s_float_rect *to, dword color, long a, long b);

#endif
