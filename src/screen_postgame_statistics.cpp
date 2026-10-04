// @flags /O1 /Gr
/* SCREEN_POSTGAME_STATISTICS.CPP: the postgame statistics screens (named
   after the debug build's source file). The screens of the vtables 0x4593c0,
   0x459430, 0x459900, 0x4597b0 and 0x459820 share every slot but the one that
   builds them, which titles their texts. */

#include "cseries.h"
#include "screen_widgets.h"

class c_postgame_statistics_screen_4593c0 : public c_screen_widget
{
public:
	virtual void v18(void *parameters);
};

class c_postgame_statistics_screen_459430 : public c_screen_widget
{
public:
	virtual void v18(void *parameters);
};

class c_postgame_statistics_screen_459900 : public c_screen_widget
{
public:
	virtual void v18(void *parameters);
};

class c_postgame_statistics_screen_4597b0 : public c_screen_widget
{
public:
	virtual void v18(void *parameters);
};

class c_postgame_statistics_screen_459820 : public c_screen_widget
{
public:
	virtual void v18(void *parameters);
};

// @retail 0x23381b
void c_postgame_statistics_screen_4593c0::v18(void *parameters)
{
	c_text_widget_45a5e0 *title;
	c_text_widget_45a5e0 *heading1;
	c_text_widget_45a5e0 *heading2;

	c_user_interface_widget::v1();
	title = (c_text_widget_45a5e0 *)find_child(6, 0, false);
	heading1 = (c_text_widget_45a5e0 *)find_child(6, 1, false);
	heading2 = (c_text_widget_45a5e0 *)find_child(6, 2, false);
	if (title)
	{
		title->set_string(0x4000278);
		title->value6e = true;
	}
	if (heading1)
	{
		heading1->set_string(0x5000733);
		heading1->value6e = true;
	}
	if (heading2)
	{
		heading2->set_string(0x5000735);
		heading2->value6e = true;
	}
}

// @retail 0x233b81
void c_postgame_statistics_screen_459430::v18(void *parameters)
{
	c_text_widget_45a5e0 *title;
	c_text_widget_45a5e0 *heading1;
	c_text_widget_45a5e0 *heading2;

	c_user_interface_widget::v1();
	title = (c_text_widget_45a5e0 *)find_child(6, 0, false);
	heading1 = (c_text_widget_45a5e0 *)find_child(6, 1, false);
	heading2 = (c_text_widget_45a5e0 *)find_child(6, 2, false);
	if (title)
	{
		title->set_string(0x6000734);
		title->value6e = true;
	}
	if (heading1)
	{
		heading1->set_string(0xa00073a);
		heading1->value6e = true;
	}
	if (heading2)
	{
		heading2->set_string(0xd00073b);
		heading2->value6e = true;
	}
}

// @retail 0x233c35
void c_postgame_statistics_screen_459900::v18(void *parameters)
{
	c_text_widget_45a5e0 *title;
	c_text_widget_45a5e0 *heading1;
	c_text_widget_45a5e0 *heading2;

	c_user_interface_widget::v1();
	title = (c_text_widget_45a5e0 *)find_child(6, 0, false);
	heading1 = (c_text_widget_45a5e0 *)find_child(6, 1, false);
	heading2 = (c_text_widget_45a5e0 *)find_child(6, 2, false);
	if (title)
	{
		title->set_string(0x6000734);
		title->value6e = true;
	}
	if (heading1)
	{
		heading1->set_string(0xc00073f);
		heading1->value6e = true;
	}
	if (heading2)
	{
		heading2->set_string(0xa000740);
		heading2->value6e = true;
	}
}

// @retail 0x233a7e
void c_postgame_statistics_screen_4597b0::v18(void *parameters)
{
	c_text_widget_45a5e0 *title;
	c_text_widget_45a5e0 *heading1;
	c_text_widget_45a5e0 *heading2;
	c_text_widget_45a5e0 *heading3;
	c_text_widget_45a5e0 *heading4;

	c_user_interface_widget::v1();
	title = (c_text_widget_45a5e0 *)find_child(6, 0, false);
	heading1 = (c_text_widget_45a5e0 *)find_child(6, 1, false);
	heading2 = (c_text_widget_45a5e0 *)find_child(6, 2, false);
	heading3 = (c_text_widget_45a5e0 *)find_child(6, 3, false);
	heading4 = (c_text_widget_45a5e0 *)find_child(6, 4, false);
	if (title)
	{
		title->set_string(0x6000734);
		title->value6e = true;
	}
	if (heading1)
	{
		heading1->set_string(0x5000739);
		heading1->value6e = true;
	}
	if (heading2)
	{
		heading2->set_string(0x7000737);
		heading2->value6e = true;
	}
	if (heading3)
	{
		heading3->set_string(0x6000738);
		heading3->value6e = true;
	}
	if (heading4)
	{
		heading4->set_string(0x800073c);
		heading4->value6e = true;
	}
}

// @retail 0x233d07
void c_postgame_statistics_screen_459820::v18(void *parameters)
{
	c_text_widget_45a5e0 *title;
	c_text_widget_45a5e0 *heading1;
	c_text_widget_45a5e0 *heading2;
	c_text_widget_45a5e0 *heading3;
	c_text_widget_45a5e0 *heading4;

	c_user_interface_widget::v1();
	title = (c_text_widget_45a5e0 *)find_child(6, 0, false);
	heading1 = (c_text_widget_45a5e0 *)find_child(6, 1, false);
	heading2 = (c_text_widget_45a5e0 *)find_child(6, 2, false);
	heading3 = (c_text_widget_45a5e0 *)find_child(6, 3, false);
	heading4 = (c_text_widget_45a5e0 *)find_child(6, 4, false);
	if (title)
	{
		title->set_string(0x6000734);
		title->value6e = true;
	}
	if (heading1)
	{
		heading1->set_string(0x900075a);
		heading1->value6e = true;
	}
	if (heading2)
	{
		heading2->set_string(0xb00075b);
		heading2->value6e = true;
	}
	if (heading3)
	{
		heading3->set_string(0xe00075c);
		heading3->value6e = true;
	}
	if (heading4)
	{
		heading4->set_string(0xa00075d);
		heading4->value6e = true;
	}
}
