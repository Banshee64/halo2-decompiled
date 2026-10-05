# Path transition geometry

Retail range claimed: `0x26f150`–`0x27058f`.

16 missing functions, 5,052 retail bytes. Transition geometry and object candidate selection. Existing implementations and external callees remain in place. Replaced stubs:26f150/26f3f0 in src/stubs/path.cpp and26fc80 in src/stubs/lane_b.cpp. A verified missing caller argument for26fc80 needs separate user approval; defer that function if approval is withheld.

Boundaries are inferred from retail callgraph and behavior, not proven original source-file identity. Banshee64 approved this bounded group. This draft claims the range before implementation. Results will follow.
