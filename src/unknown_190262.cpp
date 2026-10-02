// @flags /O1 /Ob2 /arch:SSE /Gr
#include "cseries.h"
#include "unknown_19b516.h"

// @retail 0x190262
long function_190262(long value)
{
	long result;

	switch (value)
	{
	case NONE:
		result = 0;
		break;
	case 0:
		result = 1;
		break;
	case 1:
		result = 2;
		break;
	case 2:
		result = 3;
		break;
	default:
		result = NONE;
		break;
	}
	return result;
}

// the widget at the top of the parent chain, or this widget when it has no
// parent and no child
// @retail 0x22eeee
c_widget *c_widget::function_22eeee()
{
	c_widget *widget = parent;

	if (widget)
	{
		c_widget *next;
		while ((next = widget->parent) != 0)
			widget = next;
	}
	if (!widget && *(long *)unknown04 == 0)
		widget = this;
	return widget;
}

// the abstract base class with the vtable at 0x4599dc: slot 0 clears the
// fields, slot 2 is pure, slot 3 releases the resources at +8 and +0xc
class c_resource_pair
{
public:
	c_resource_pair();
	c_resource_pair(const c_resource_pair &) {}

	virtual void clear() {}
	virtual void v1() {}
	virtual void v2() {}
	virtual void release() {}

	long field_4;
	long field_8;
	long field_c;
	byte unknown10[0xc];
	long field_1c;
	byte unknown20[0x10];
	long field_30;
	long field_34;
};

// @retail 0x234e43
c_resource_pair::c_resource_pair()
{
	field_4 = 4;
	field_8 = 0;
	field_c = 0;
	field_1c = 0;
	field_30 = 0;
	field_34 = 0;
}
