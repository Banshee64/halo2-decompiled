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
