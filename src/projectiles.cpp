// @flags /O2 /Ob1 /arch:SSE /Gr
/* PROJECTILES.CPP: projectiles

The functions follow projectiles.obj in Bungie's May 2003 builds
(halo-symbol-atlas): the projectile object type's callbacks (its
definition at 0x467f28), and its flight, collision, attachment and
detonation. */

#include "cseries.h"
#include "globals.h"
