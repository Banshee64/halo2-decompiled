/* UNKNOWN_1523C0.H: the game engine base class (vtable of the engine classes at
   0x45c750, 0x45c8f0, 0x45c9c0 and 0x459d18; its default handlers live in
   unknown_072c70.cpp). Only declarations here: the argument types are opaque
   to the classes that derive from it.

   The numbering is off: the real 51-slot engine vtables start at 0x45c6d8,
   0x45c7a8, 0x45c878 and 0x45c948, so 0x45c750 is slot 30 of the first one,
   and each subclass here pairs one engine's slots 30-50 with the next
   engine's slots 0-28 (see unknown_2bbf50.cpp). A later cleanup should
   renumber from the real starts. */

#ifndef UNKNOWN_1523C0_H
#define UNKNOWN_1523C0_H

#include "unknown_11c920.h"

struct s_engine_settings;
struct s_player_update;
struct s_stats;

class c_game_engine
{
public:
	virtual void v0(long, long, bool, long) {}
	virtual long v1(long, long, long);
	virtual void v2(long, long);
	virtual bool v3(long, long) { return true; }
	virtual bool v4(long) { return false; }
	virtual bool v5(long, long);
	virtual void v6(long);
	virtual long v7(long, byte *);
	virtual bool v8(long) { return false; }
	virtual long v9();
	virtual void v10();
	virtual void v11(s_engine_settings *);
	virtual void v12(byte, dword *, s_engine_settings *);
	virtual bool v13(byte, s_engine_settings *);
	virtual void v14(long, s_stats *) {}
	virtual void v15(long, long *, long) {}
	virtual bool v16(long, long) { return false; }
	virtual void v17(long, long, real *);
	virtual bool v18(long, long, long, long) { return false; }
	virtual bool v19(short, dword, long, s_player_update *);
	virtual long v20(long, long, long, long, long);
	virtual void v21_unused() {}
	virtual long v22();
	virtual bool v23() { return false; }
	virtual void v24() {}
	virtual bool v25();
	virtual bool v26(long) { return true; }
	virtual void v27(long) {}
	virtual void v28(long) {}
	virtual void v29(long) {}
	virtual void v30(long) {}
	virtual void v31(long, long) {}
	virtual void v32(long) {}
	virtual void v33() {}
	virtual void v34() {}
	virtual void v35(long) {}
	virtual void v36(long) {}
	virtual void v37(long) {}
	virtual bool v38(long, long) { return true; }
	virtual void v39(long, long) {}
	virtual void v40() {}
	virtual real v41(long);
	virtual long v42(long);
	virtual void v43(long) {}
	virtual void v44(long) {}
	virtual void v45(long, long) {}
	virtual void v46(long, long) {}
	virtual long v47(long, long, long);
	virtual long v48() { return NONE; }
	virtual bool v49(short, short);
	virtual void v50(long);
};

#endif
