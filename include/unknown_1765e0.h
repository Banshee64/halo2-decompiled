#pragma once

#include "unknown_0259d0.h"

/* starts the effect tag_index at point (src/unknown_175bd0.cpp): its markers
   face along direction, against it, along the normal and along the
   direction reflected about the normal; mode 1 adds a marker along the
   direction itself. Retail takes point, direction and normal in esi, ebx
   and eax. */
long function_1765e0(point3f const *point, vector3f const *direction, vector3f const *normal, long tag_index, long mode, long deterministic);
