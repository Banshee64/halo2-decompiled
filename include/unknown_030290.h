/* UNKNOWN_030290.H: view, camera and flag structures used by 0x30290..0x30e30 */

#ifndef UNKNOWN_030290_H
#define UNKNOWN_030290_H

struct short_rectangle2d { short top, left, bottom, right; };

/* a view: position first, screen bounds at 0x30 */
struct s_view
{
	point3f position;
	byte unknown0c[0x24];
	short_rectangle2d bounds;
};

/* matrix4x3 (scale, rotation, position) followed by projection values */
struct s_camera
{
	real scale;
	vector3f right;
	vector3f up;
	vector3f forward;
	point3f position;
	byte unknown34[0x44];
	real unknown78;
	byte unknown7c[0x10];
	real unknown8c;
	byte unknown90[8];
	real unknown98;
	real unknown9c;
};

struct s_obj
{
	byte unknown00[0x30];
	point3f position;
	long unknown3c;
};

struct s_obj_element
{
	s_obj *obj;
	long unknown04;
	long unknown08;
};

struct s_obj_array
{
	byte unknown00[8];
	s_obj_element elements[1];
};

struct s_flag_entry
{
	long key;
	long value;
	real x;
};

#endif
