/* ENGINE_PEER.H: the engine objects in the table at 0x55e4d0 (indexed by the
   engine index of g_4e9ae8). Slots 41..43 are wrapped by the game engine
   table of 072c70 and 2bcdd0, slot 39 and slots 44..46 are called by 0a45d0.
   Their methods live in the library range or another source file; the slots
   are empty. */

#ifndef ENGINE_PEER_H
#define ENGINE_PEER_H

#include "cseries.h"

struct s_stats;

class c_engine_peer
{
public:
	virtual void p0() {}
	virtual void p1() {}
	virtual void p2() {}
	virtual void p3() {}
	virtual void p4() {}
	virtual void p5() {}
	virtual void p6() {}
	virtual void p7() {}
	virtual void p8() {}
	virtual void p9() {}
	virtual void p10() {}
	virtual void p11() {}
	virtual void p12() {}
	virtual void p13() {}
	virtual void p14() {}
	virtual void p15() {}
	virtual void p16() {}
	virtual void p17() {}
	virtual void p18() {}
	virtual void p19() {}
	virtual void p20() {}
	virtual void p21() {}
	virtual void p22() {}
	virtual void p23() {}
	virtual void p24() {}
	virtual void p25() {}
	virtual void p26() {}
	virtual bool p27(short, short) { return false; }
	virtual void p28() {}
	virtual void p29() {}
	virtual void p30() {}
	virtual void p31() {}
	virtual void p32() {}
	virtual void p33() {}
	virtual void p34() {}
	virtual bool p35(long, long) { return false; }
	virtual void p36() {}
	virtual void p37() {}
	virtual void p38() {}
	virtual long get_current_id() { return 0; }
	virtual void p40() {}
	virtual void p41(s_stats *) {}
	virtual long p42(long, long *, long) { return 0; }
	virtual byte p43(long, long) { return 0; }
	virtual long p44(long, long) { return 0; }
	virtual long p45(long, long, long) { return 0; }
	virtual bool p46(long, long, long) { return false; }
};

#endif
