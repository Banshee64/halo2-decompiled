/* SCREEN_WIDGET.H: the user interface screen widgets (the 28 slot vtables
   at 0x458840, 0x4588c0 and their subclasses) as the code in
   0x230000..0x239b80 uses them, and the screen requests that create them */

#ifndef SCREEN_WIDGET_H
#define SCREEN_WIDGET_H

#include "cseries.h"
#include "unknown_19b516.h"

struct s_event;
class c_screen_widget;

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

/* the screen widget (vtable 0x458840); the screens in 0x230000.. override
   slots 10, 18, 19 and 26 */
class c_screen_widget
{
public:
	virtual ~c_screen_widget() {}
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

	/* code outside 0x230000..0x239b80, stubbed */
	void function_22e957(long a);

	dword unknown04;
	short m8;
	byte unknown0a[0x6];
	c_screen_widget *parent;
	c_screen_widget *child;
	c_screen_widget *next;
	c_screen_widget *prev;
	byte unknown20[0x22];
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
	byte unknown58[0x12];
	short m6a;
	byte m6c;
	byte m6d;
	byte m6e;
	byte unknown6f;
	long m70;
	long m74;
	long m78;
	byte unknown7c[0x5f2 - 0x7c];
	byte m5f2;
	byte m5f3;
	byte m5f4;
	byte unknown5f5[0x610 - 0x5f5];
};

void __stdcall function_1a4826(void *pointer);
void *__stdcall function_1a47fd(long size);
void function_148148(c_screen_widget *screen);

#endif
