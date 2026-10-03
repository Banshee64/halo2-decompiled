/* SCREEN_WIDGET.H: the user interface screen widgets (the 28 slot vtables
   at 0x458840, 0x4588c0 and their subclasses) as the code in
   0x230000..0x239b80 uses them, and the screen requests that create them */

#ifndef SCREEN_WIDGET_H
#define SCREEN_WIDGET_H

#include "cseries.h"
#include "real_math.h"
#include "unknown_19b516.h"

struct s_event;
class __single_inheritance c_screen_widget;

/* a request to bring up a screen (the s_message of unknown_19b516.h): the
   channel mask, the screen's parameters and the screen's create function */
struct s_screen_request
{
	s_screen_request()
	{
		field_c = 0;
	}

	union
	{
		word type;
		struct
		{
			word type_bit0 : 1;
			word type_bit1 : 1;
			word type_bit2 : 1;
		};
	};
	word flags;
	long value;
	long data;
	dword field_c;
	s_id_triplet id;
	c_screen_widget *(__stdcall *create)(s_screen_request *request);
};

typedef c_screen_widget *(__stdcall *screen_create_function)(s_screen_request *request);

void function_149f49(word a, s_message *message, dword *id, word b, long c, long d, long e);

/* an intrusive list node (0x22f0d2 unlinks one) */
struct s_list_node
{
	s_list_node *prev;
	s_list_node *next;
	void *list;
};

/* the user interface widget (vtable 0x458788, constructed by 0x22e27b) */
class c_user_interface_widget
{
public:
	c_user_interface_widget(long type, short index);
	c_user_interface_widget() {}

	virtual ~c_user_interface_widget() {}
	virtual void v1() {}
	virtual void v2() {}
	virtual void v3() {}
	virtual void v4(long a) {}
	virtual void v5() {}
	virtual long v6() { return 0; }
	virtual void v7() {}
	virtual void v8() {}
	virtual void v9() {}
	virtual bool v10(s_event *event) { return false; }
	virtual void v11() {}
	virtual void v12() {}
	virtual void v13() {}
	virtual void v14() {}
	virtual long v15() { return 0; }
	virtual bool v16() { return false; }

	/* code outside 0x230000..0x239b80, stubbed */
	void function_22e957(long a);

	long type;
	short m8;
	short ma;
	long identifier;
	c_screen_widget *parent;
	c_screen_widget *child;
	c_screen_widget *next;
	c_screen_widget *prev;
	short bounds[4];
	real scale[3];
	byte unknown34[0xe];
	word flag42_0 : 1;
	word flag42_1 : 1;
	word flag42_2 : 1;
	word flag42_3 : 1;
	word flag42_4 : 1;
	word flag42_5 : 1;
	word flag42_6 : 1;
	word flag42_7 : 1;
	word flag42_8 : 1;
	word flag42_9 : 1;
	word flag42_10 : 1;
	word flag42_11 : 1;
	word flag42_12 : 1;
	word flag42_13 : 1;
	word flag42_14 : 1;
	word flag42_15 : 1;
	byte unknown44[0x8];
	dword m4c;
	byte unknown50[0x4];
	dword m54;
	byte unknown58[0x10];
	short m68;
	short m6a;
	bool allocated;
	bool m6d;
	bool m6e;
	byte unknown6f;
};

/* a widget member of the screens (vtable 0x458940, constructed by 0x22f583) */
class c_screen_widget_member : public c_user_interface_widget
{
public:
	c_screen_widget_member(long a);

	byte unknown70[0x2b8 - 0x70];
};

/* the screens' callback node at +0x5f8 (vtable 0x45bdb0) */
class c_screen_callback : public s_list_node
{
public:
	c_screen_callback(c_screen_widget *owner, void (c_screen_widget::*handler)(short *delta)) :
		owner(owner),
		handler(handler)
	{
		next = 0;
		prev = 0;
		list = 0;
	}

	virtual void v0() {}

	c_screen_widget *owner;
	void (c_screen_widget::*handler)(short *delta);
};

/* the screen widget (vtable 0x458840, constructed by 0x22f5ca); the screens
   in 0x230000.. override slots 10, 18, 19 and 26 */
class c_screen_widget : public c_user_interface_widget
{
public:
	c_screen_widget(long a, long b, long c, short index);

	virtual void v17() {}
	virtual void v18(void *a) {}
	virtual void v19() {}
	virtual long v20() { return 0; }
	virtual long v21() { return 0; }
	virtual void v22(void *a) {}
	virtual void v23(void *a) {}
	virtual void v24(void *a) {}
	virtual void v25(void *a) {}
	virtual screen_create_function v26() { return 0; }
	virtual bool v27() { return false; }

	virtual bool v10(s_event *event);

	void function_230427(short *delta);

	long m70;
	long m74;
	long m78;
	long m7c;
	c_screen_widget_member member80;
	c_screen_widget_member member338;
	short m5f0;
	byte m5f2;
	char m5f3;
	byte m5f4;
	byte unknown5f5[3];
	c_screen_callback callback;
};

/* the screen with a list (vtable 0x4588c0, constructed by 0x230451) */
class c_screen_with_menu : public c_screen_widget
{
public:
	c_screen_with_menu(long a, long b, long c, short index, void *list);

	void *list;
};

void __stdcall function_1a4826(void *pointer);
void *__stdcall function_1a47fd(long size);
void function_148148(c_screen_widget *screen);

#endif
