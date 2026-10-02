#pragma once

// the page heap's own destructor body, called from its deleting destructor (library code in retail)
void __fastcall function_329e10(void *heap);

// 0x1a1c20 releases through this (library code in retail)
void __cdecl function_321379(void *memory);
