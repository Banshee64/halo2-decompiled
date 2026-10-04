// @flags /O2 /Gr
/* UNITS_OBJ.CPP: units (the rest of units.obj)

The unit object type's callbacks (its definition at 0x4679b0) and the unit
functions around them: units.obj in Bungie's 2003 builds (unit placement,
creation, update, seats, inventory, weapons and grenades, melee, aiming,
custom animations and scripting). The functions upstream already has stay
in their files (units.cpp, unknown_0c7070.cpp, unknown_0c86e0.cpp,
unknown_0c8880.cpp, unknown_0cafc0.cpp, unknown_0cbd50.cpp,
unknown_0cc2b0.cpp, unknown_0cd660.cpp, unknown_0d0690.cpp,
unknown_0d0e00.cpp). */

#include "cseries.h"
#include "globals.h"
