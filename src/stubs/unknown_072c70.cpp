// stubs for game functions not decompiled yet, called by unknown_072c70.cpp
#include "cseries.h"
#include "unknown_072c70.h"

struct s_event;
struct s_stats;
struct s_stats_state;

#define STUB_BODY static volatile long g_stub_##__LINE__

static volatile long g_stub_sink;

// @stub 0x15b7c0
void function_15b7c0(long a, long b) { g_stub_sink = a + b + 1; }
// @stub 0x15eaf0
bool function_15eaf0() { return g_stub_sink != 2; }
// @stub 0x23f260
long function_23f260(long a, long b, long c) { return a + b + c + 3; }
// @stub 0x1523c0
void function_1523c0() { g_stub_sink = 4; }
// @stub 0x196780
void function_196780() { g_stub_sink = 5; }
// @stub 0x15cba0
void function_15cba0() { g_stub_sink = 6; }
// @stub 0x1389c0
void function_1389c0() { g_stub_sink = 7; }
// @stub 0x19ec40
void function_19ec40(long a, long b, long c, long d, long e, long f, s_stats *g) { g_stub_sink = a + b + c + d + e + f + (long)g + 8; }
// @stub 0xa7c50
void function_a7c50(s_event *a) { g_stub_sink = (long)a + 9; }
// @stub 0x19eb30
void function_19eb30(s_event *a) { g_stub_sink = (long)a + 10; }
// @stub 0x19f3c0
long function_19f3c0(long a, long b) { return a * b + 11; }
// @stub 0x19eb90
void function_19eb90(s_event *a) { g_stub_sink = (long)a + 12; }
// @stub 0x15e410
s_stats_state *function_15e410() { return (s_stats_state *)(g_stub_sink + 13); }
// @stub 0x162550
bool function_162550(long a) { return a != 14; }
// @stub 0x19f240
bool function_19f240(long *a) { return a[0] != 15; }
// @stub 0x24e59f
void function_24e59f(long *a) { g_stub_sink = (long)a + 16; }
// @stub 0x2bc5c0
void function_2bc5c0(long a, long *b) { g_stub_sink = a + (long)b + 17; }
// @stub 0x2bcf10
void function_2bcf10(long *a, long *b) { g_stub_sink = (long)a + (long)b + 18; }
// @stub 0x2bcf90
bool function_2bcf90(long *a, long *b, long c) { return (long)a + (long)b + c != 19; }
// @stub 0x15fe70
void function_15fe70(long a) { g_stub_sink = a + 20; }
// @stub 0x2bc1f0
void function_2bc1f0() { g_stub_sink = 21; }
// @stub 0x2bc990
void function_2bc990(long a) { g_stub_sink = a + 22; }
// @stub 0x1a0180
void function_1a0180(long a, long b) { g_stub_sink = a + b + 23; }

/* the peer object class (not decompiled); distinct bodies, so the methods are never folded together */
void c_engine_peer::p0() { g_stub_sink = 0 + 100; }
void c_engine_peer::p1() { g_stub_sink = 1 + 100; }
void c_engine_peer::p2() { g_stub_sink = 2 + 100; }
void c_engine_peer::p3() { g_stub_sink = 3 + 100; }
void c_engine_peer::p4() { g_stub_sink = 4 + 100; }
void c_engine_peer::p5() { g_stub_sink = 5 + 100; }
void c_engine_peer::p6() { g_stub_sink = 6 + 100; }
void c_engine_peer::p7() { g_stub_sink = 7 + 100; }
void c_engine_peer::p8() { g_stub_sink = 8 + 100; }
void c_engine_peer::p9() { g_stub_sink = 9 + 100; }
void c_engine_peer::p10() { g_stub_sink = 10 + 100; }
void c_engine_peer::p11() { g_stub_sink = 11 + 100; }
void c_engine_peer::p12() { g_stub_sink = 12 + 100; }
void c_engine_peer::p13() { g_stub_sink = 13 + 100; }
void c_engine_peer::p14() { g_stub_sink = 14 + 100; }
void c_engine_peer::p15() { g_stub_sink = 15 + 100; }
void c_engine_peer::p16() { g_stub_sink = 16 + 100; }
void c_engine_peer::p17() { g_stub_sink = 17 + 100; }
void c_engine_peer::p18() { g_stub_sink = 18 + 100; }
void c_engine_peer::p19() { g_stub_sink = 19 + 100; }
void c_engine_peer::p20() { g_stub_sink = 20 + 100; }
void c_engine_peer::p21() { g_stub_sink = 21 + 100; }
void c_engine_peer::p22() { g_stub_sink = 22 + 100; }
void c_engine_peer::p23() { g_stub_sink = 23 + 100; }
void c_engine_peer::p24() { g_stub_sink = 24 + 100; }
void c_engine_peer::p25() { g_stub_sink = 25 + 100; }
void c_engine_peer::p26() { g_stub_sink = 26 + 100; }
bool c_engine_peer::p27(short a, short b) { return a == b; }
void c_engine_peer::p28() { g_stub_sink = 28 + 100; }
void c_engine_peer::p29() { g_stub_sink = 29 + 100; }
void c_engine_peer::p30() { g_stub_sink = 30 + 100; }
void c_engine_peer::p31() { g_stub_sink = 31 + 100; }
void c_engine_peer::p32() { g_stub_sink = 32 + 100; }
void c_engine_peer::p33() { g_stub_sink = 33 + 100; }
void c_engine_peer::p34() { g_stub_sink = 34 + 100; }
bool c_engine_peer::p35(long a, long b) { return a == b + 1; }
void c_engine_peer::p36() { g_stub_sink = 36 + 100; }
void c_engine_peer::p37() { g_stub_sink = 37 + 100; }
void c_engine_peer::p38() { g_stub_sink = 38 + 100; }
void c_engine_peer::p39() { g_stub_sink = 39 + 100; }
void c_engine_peer::p40() { g_stub_sink = 40 + 100; }
void c_engine_peer::p41(s_stats *a) { g_stub_sink = (long)a + 41; }
long c_engine_peer::p42(long a, long *b, long c) { return a + (long)b + c + 42; }
byte c_engine_peer::p43(long a, long b) { return (byte)(a + b + 43); }
