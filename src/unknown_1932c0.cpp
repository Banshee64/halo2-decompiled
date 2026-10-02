// @flags /O2 /Gr
#include "cseries.h"

struct s_surface_description
{
	long type;
	byte unknown004[0x5e8];
	long width;       // 0x5ec
	long height;      // 0x5f0
	long depth;       // 0x5f4
	long field_5f8;   // 0x5f8
	byte unknown5fc[8];
	bool flag_604;    // 0x604
	byte unknown605[3];
	long field_608;   // 0x608
	long field_60c;   // 0x60c
};
// @retail 0x001932c0
long function_1932c0(s_surface_description* p)
{
	switch (p->type)
	{
	case 1:
		return p->width;
	case 2:
		return p->width;
	case 3:
		return p->height * p->width;
	case 4:
		return p->height * p->width;
	case 5:
		return p->height * p->width;
	default:
		__assume(0);
	}
}

// @retail 0x00193300
long function_193300(s_surface_description* p)
{
	switch (p->type)
	{
	case 1:
		return p->height;
	case 2:
		return p->height;
	case 3:
		return p->depth * p->width;
	case 4:
		return p->depth * p->width;
	case 5:
		return p->depth * p->width;
	default:
		__assume(0);
	}
}

// @retail 0x00193340
long function_193340(s_surface_description* p)
{
	switch (p->type)
	{
	case 1:
		return 1;
	case 2:
		return 1;
	case 3:
		return p->height;
	case 4:
		return p->height;
	case 5:
		return p->height;
	default:
		__assume(0);
	}
}

// @retail 0x00193370
long function_193370(s_surface_description* p)
{
	switch (p->type)
	{
	case 1:
		return 1;
	case 2:
		return 1;
	case 3:
		return p->depth;
	case 4:
		return p->depth;
	case 5:
		return p->depth;
	default:
		__assume(0);
	}
}

// @retail 0x001933a0
long function_1933a0(s_surface_description* p)
{
	long result = 0;
	if (p->type == 5)
	{
		result = p->depth;
	}
	else if (p->type == 4 && p->flag_604)
	{
		result = p->field_60c;
	}
	return result;
}

// @retail 0x001933d0
long function_1933d0(s_surface_description* p)
{
	long result = 0;
	if (p->type == 5)
	{
		result = p->height;
	}
	else if (p->type == 4 && p->flag_604)
	{
		result = p->field_608;
	}
	return result;
}

// @retail 0x00193400
long function_193400(s_surface_description* p)
{
	long result = 0;
	switch (p->type)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		if (*(byte*)&p->field_5f8)
		{
			result = 1;
		}
		break;
	case 4:
		result = p->field_5f8;
		break;
	case 5:
		result = p->field_5f8;
		break;
	default:
		__assume(0);
	}
	return result;
}

// @retail 0x00193440
long function_193440(s_surface_description* p)
{
	long result = NONE;
	switch (p->type)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		result = p->width;
		break;
	case 4:
		result = p->width;
		break;
	case 5:
		result = p->width;
		break;
	default:
		__assume(0);
	}
	return result;
}
// @retail 0x00193470
bool function_193470(s_surface_description* p)
{
	return p->type == 5 || p->type == 2 || p->type == 4;
}

// @retail 0x00193490
bool function_193490(s_surface_description* p)
{
	return p->type == 1 || p->type == 3;
}

// @retail 0x001934b0
bool function_1934b0(s_surface_description* p)
{
	return p->type == 5 || p->type == 3 || p->type == 4;
}

// @retail 0x001934d0
bool function_1934d0(s_surface_description* p)
{
	return (p->type == 4 && p->flag_604) || p->type == 5;
}
