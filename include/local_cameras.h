/* LOCAL_CAMERAS.H: the per local player cameras in g_4e6380 (defined in
   unknown_03d380.cpp as s_4e6380), as unknown_125360.cpp and
   unknown_18d290.cpp read them */
#ifndef LOCAL_CAMERAS_H
#define LOCAL_CAMERAS_H

#include "cseries.h"
#include "real_math.h"

struct s_4e6380;
extern s_4e6380 *g_4e6380;

struct s_local_camera
{
	byte unknown00[4];
	short index;
	bool active;
	byte unknown07[0x30 - 0x7];
	point3f position;
	byte unknown3c[0x48 - 0x3c];
};

struct s_local_cameras
{
	byte unknown00[0x88];
	s_local_camera cameras[4];
};

static inline s_local_camera *local_camera_get(long index)
{
	return &((s_local_cameras *)g_4e6380)->cameras[index];
}

s_local_camera *function_125360(void);

#endif
