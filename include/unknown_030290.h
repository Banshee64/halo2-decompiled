/* UNKNOWN_030290.H: view, camera and flag structures used by 0x30290..0x30e30 */

#ifndef UNKNOWN_030290_H
#define UNKNOWN_030290_H

struct real_point2d { real x, y; };
struct real_rectangle2d { real x0, x1, y0, y1; };
struct short_rectangle2d { short top, left, bottom, right; };

/* a view: position first, screen bounds at 0x30 */
struct s_view
{
	real_point3d position;
	byte unknown0c[0x24];
	short_rectangle2d bounds;
};

/* matrix4x3 (scale, rotation, position) followed by projection values */
struct s_camera
{
	real scale;
	real_vector3d right;
	real_vector3d up;
	real_vector3d forward;
	real_point3d position;
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
	real_point3d position;
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

struct s_obj_table
{
	byte unknown00[0x44];
	s_obj_array *table;
};

struct s_flag_entry
{
	long key;
	long value;
	real x;
};

#endif
