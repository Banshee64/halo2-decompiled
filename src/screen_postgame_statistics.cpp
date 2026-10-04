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

/* the screen of the vtable 0x459890 (constructed by 0x233892), whose
   columns are named by the game engine of the last game */
class c_postgame_statistics_screen_459890 : public c_screen_widget
{
public:
	virtual void v18(void *parameters);
};

/* the game engine of the game the statistics are for */
long g_50224c;

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

// @retail 0x2338cf
void c_postgame_statistics_screen_459890::v18(void *parameters)
{
	c_text_widget_45a5e0 *title;
	c_text_widget_45a5e0 *heading1;
	c_text_widget_45a5e0 *heading2;
	c_text_widget_45a5e0 *column1;
	c_text_widget_45a5e0 *column2;

	c_user_interface_widget::v1();
	title = (c_text_widget_45a5e0 *)find_child(6, 0, false);
	heading1 = (c_text_widget_45a5e0 *)find_child(6, 1, false);
	heading2 = (c_text_widget_45a5e0 *)find_child(6, 4, false);
	column1 = (c_text_widget_45a5e0 *)find_child(6, 2, false);
	column2 = (c_text_widget_45a5e0 *)find_child(6, 3, false);
	if (title)
	{
		title->set_string(0x6000734);
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
	if (column1 && column2)
	{
		if (g_50224c == 2)
		{
			column1->set_string(0x8000723);
			column2->set_string(0xa000724);
		}
		else if (g_50224c == 1)
		{
			column1->set_string(0xa000725);
			column2->set_string(0xc000726);
		}
		else if (g_50224c == 3)
		{
			column1->set_string(0xd00072a);
			column2->set_string(0xf00072b);
		}
		else if (g_50224c == 4)
		{
			column1->set_string(0xd000728);
			column2->set_string(0xf000729);
		}
		else if (g_50224c == 7)
		{
			column1->set_string(0xc00072d);
			column2->set_string(0xd00072e);
		}
		else if (g_50224c == 8)
		{
			column1->set_string(0xb00072f);
			column2->set_string(0xa000730);
		}
		else if (g_50224c == 9)
		{
			column1->set_string(0xa000731);
			column2->set_string(0xa000732);
		}
		column1->value6e = true;
		column2->value6e = true;
	}
}
