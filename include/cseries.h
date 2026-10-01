/* CSERIES.H: basic types and conventions shared by every source file */

#ifndef CSERIES_H
#define CSERIES_H

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;
typedef float real;

#define NONE -1

/* Bungie's static functions. Empty here, so the build's stand-in callers in
   other files can reach them; LTCG sees the whole program either way. */
#define PRIVATE

#endif
