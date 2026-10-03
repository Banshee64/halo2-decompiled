// @flags /O2 /Ob1 /Gr /arch:SSE
/* RECORDED_ANIMATION_PLAYBACK.CPP: recorded input direction controllers.
   Existing field readers and update_controller_char remain in
   unknown_29ec30.cpp with their upstream definitions. */

#include "cseries.h"
#include "real_math.h"

struct vector_short_difference_data
{
	short yaw;
	short pitch;
};

struct direction_playback_controller
{
	short yaw;
	short pitch;
};

// @retail 0x29ed40
PRIVATE void update_controller_short(vector_short_difference_data const *data, direction_playback_controller *controller)
{
	controller->yaw += data->yaw;
	if (controller->yaw > 1000)
		controller->yaw -= 1000;
	else if (controller->yaw < -1000)
		controller->yaw += 1000;
	controller->pitch += data->pitch;
}

// @retail 0x29ed80
PRIVATE void uncompress_vector_from_controller(real_vector3d *vector, direction_playback_controller const *controller)
{
	real yaw = (real)controller->yaw * 0.00314159272f;
	real pitch = (real)controller->pitch * 0.00314159272f;
	vector->i = (real)(cos(yaw) * cos(pitch));
	vector->j = (real)(sin(yaw) * cos(pitch));
	vector->k = (real)sin(pitch);
}

// The callback table stores four stack arguments for each event handler.
struct animation_playback_controller
{
    direction_playback_controller facing;
    direction_playback_controller aiming;
    direction_playback_controller looking;
};

struct playback_unit_control_view
{
    byte fields[0x28];
    real_vector3d facing;
    real_vector3d aiming;
    real_vector3d looking;
};

struct animation_event_header
{
    byte type_and_time;
};

void update_controller_char(const char *data, short *controller);

// @retail 0x29edc0
PRIVATE void __stdcall apply_vector_char_difference(animation_playback_controller *controller,
    playback_unit_control_view *control, animation_event_header const *header, byte const **cursor)
{
    short mask = (header->type_and_time >> 2) - 7;
    char const *data = (char const *)*cursor;
    short facing = mask & 1;
    if (facing)
    {
        update_controller_char(data, (short *)&controller->facing);
        real yaw = (real)controller->facing.yaw * 0.00314159272f;
        real pitch = (real)controller->facing.pitch * 0.00314159272f;
        control->facing.i = (real)(cos(yaw) * cos(pitch));
        control->facing.j = (real)(sin(yaw) * cos(pitch));
        control->facing.k = (real)sin(pitch);
    }
    short aiming = mask & 2;
    if (aiming)
    {
        if (facing)
        {
            controller->aiming = controller->facing;
            control->aiming = control->facing;
        }
        else
        {
            update_controller_char(data, (short *)&controller->aiming);
            uncompress_vector_from_controller(&control->aiming, &controller->aiming);
        }
    }
    if (mask & 4)
    {
        if (facing)
        {
            controller->looking = controller->facing;
            control->looking = control->facing;
        }
        else if (aiming)
        {
            controller->looking = controller->aiming;
            control->looking = control->aiming;
        }
        else
        {
            update_controller_char(data, (short *)&controller->looking);
            uncompress_vector_from_controller(&control->looking, &controller->looking);
        }
    }
    *cursor += 2;
}

// @retail 0x29ef20
PRIVATE void __stdcall apply_vector_short_difference(animation_playback_controller *controller,
    playback_unit_control_view *control, animation_event_header const *header, byte const **cursor)
{
    short mask = (header->type_and_time >> 2) - 15;
    vector_short_difference_data const *data = (vector_short_difference_data const *)*cursor;
    short facing = mask & 1;
    if (facing)
    {
        update_controller_short(data, &controller->facing);
        real yaw = (real)controller->facing.yaw * 0.00314159272f;
        real pitch = (real)controller->facing.pitch * 0.00314159272f;
        control->facing.i = (real)(cos(yaw) * cos(pitch));
        control->facing.j = (real)(sin(yaw) * cos(pitch));
        control->facing.k = (real)sin(pitch);
    }
    short aiming = mask & 2;
    if (aiming)
    {
        if (facing)
        {
            controller->aiming = controller->facing;
            control->aiming = control->facing;
        }
        else
        {
            update_controller_short(data, &controller->aiming);
            uncompress_vector_from_controller(&control->aiming, &controller->aiming);
        }
    }
    if (mask & 4)
    {
        if (facing)
        {
            controller->looking = controller->facing;
            control->looking = control->facing;
        }
        else if (aiming)
        {
            controller->looking = controller->aiming;
            control->looking = control->aiming;
        }
        else
        {
            update_controller_short(data, &controller->looking);
            uncompress_vector_from_controller(&control->looking, &controller->looking);
        }
    }
    *cursor += 4;
}

// Existing field readers retain their upstream declarations.
void __stdcall function_29ec30(long a, byte *dest, long c, char **cursor);
void __stdcall function_29ec50(long a, byte *dest, long c, char **cursor);
void __stdcall function_29ec70(long a, byte *dest, long c, long **cursor);
void __stdcall function_29ec90(long a, byte *dest, long c, byte **cursor);
void __stdcall function_29ecb0(long a, byte *dest, long c, byte **cursor);
void __stdcall function_29ecd0(long a, byte *dest, long c, real **cursor);

typedef void (__stdcall *animation_event_handler)(animation_playback_controller *,
    playback_unit_control_view *, animation_event_header const *, byte const **);

// Retail event dispatch table: type is the header's upper six bits.
animation_event_handler const g_4710f0[24] =
{
    NULL,
    NULL,
    (animation_event_handler)function_29ec30,
    (animation_event_handler)function_29ec50,
    (animation_event_handler)function_29ec70,
    (animation_event_handler)function_29ec90,
    (animation_event_handler)function_29ecd0,
    apply_vector_char_difference,
    apply_vector_char_difference,
    apply_vector_char_difference,
    apply_vector_char_difference,
    apply_vector_char_difference,
    apply_vector_char_difference,
    apply_vector_char_difference,
    apply_vector_char_difference,
    apply_vector_short_difference,
    apply_vector_short_difference,
    apply_vector_short_difference,
    apply_vector_short_difference,
    apply_vector_short_difference,
    apply_vector_short_difference,
    apply_vector_short_difference,
    apply_vector_short_difference,
    (animation_event_handler)function_29ecb0
};
