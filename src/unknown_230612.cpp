// @flags /O1 /Gr
/* UNKNOWN_230612.CPP: small virtual methods of the lists and widgets in
   0x230000..0x239b80 (one placeholder class per vtable until the classes are
   written) and two flag setting callbacks */

#include "cseries.h"
#include "unknown_19b516.h"

/* the list widgets' 22 slot vtables (0x458a74, 0x458b48 ...) */
class c_list_vtable
{
public:
	virtual ~c_list_vtable() {}
	virtual void v1() {}
	virtual void v2() {}
	virtual void v3() {}
	virtual void v4() {}
	virtual void v5() {}
	virtual void v6() {}
	virtual void v7() {}
	virtual void v8() {}
	virtual void v9() {}
	virtual bool v10(s_event *event) { return false; }
	virtual void v11() {}
	virtual void v12() {}
	virtual void v13() {}
	virtual void v14() {}
	virtual void v15() {}
	virtual void v16() {}
	virtual void v17() {}
	virtual void v18() {}
	virtual long v19() { return 0; }
	virtual void v20() {}
	virtual void v21() {}
};

/* the list of the screen at 0x458a00 (vtable 0x458a74) */
class c_list_458a74 : public c_list_vtable
{
public:
	virtual long v19();
};

/* the lists whose slot 10 is 0x230f31 (vtable 0x458b48 and others) */
class c_list_458b48 : public c_list_vtable
{
public:
	virtual bool v10(s_event *event);
};

/* the widget with the 18 slot vtable at 0x45a628 */
class c_widget_45a628
{
public:
	virtual ~c_widget_45a628() {}
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
	virtual long *v15();
	virtual void v16() {}
	virtual void v17() {}

	byte unknown04[0x6c];
	long m70;
};

// @retail 0x230612
long c_list_458a74::v19()
{
	return 5;
}

// @retail 0x230f31
bool c_list_458b48::v10(s_event *event)
{
	return ((c_widget *)this)->c_widget::v18(event);
}

// @retail 0x237607
long *c_widget_45a628::v15()
{
	return &m70;
}

byte g_547f71;
byte g_547f6e;

// @retail 0x2323ab
bool __stdcall function_2323ab(void *data)
{
	g_547f71 = true;
	return true;
}

// @retail 0x2323b7
bool __stdcall function_2323b7(void *data)
{
	g_547f6e = true;
	return true;
}

/* three counted lists of words */
struct s_word_lists_232d54
{
	s_word_lists_232d54();

	short count00;
	byte unknown02[0x3e];
	short count40;
	byte unknown42[0x9e];
	short counte0;
};

struct s_word_lists_232d67
{
	s_word_lists_232d67();

	short count00;
	byte unknown02[0x3e];
	short count40;
	byte unknown42[0x22];
	short count64;
};

// @retail 0x232d54
s_word_lists_232d54::s_word_lists_232d54()
{
	count00 = 0;
	count40 = 0;
	counte0 = 0;
}

// @retail 0x232d67
s_word_lists_232d67::s_word_lists_232d67()
{
	count00 = 0;
	count40 = 0;
	count64 = 0;
}
