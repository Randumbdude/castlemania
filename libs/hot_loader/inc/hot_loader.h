#pragma once

#include "_H_L__symb.h"

#define _H_L__L string("hl_mksr")
#define _H_L__DEF ("." + _H_L__L)

typedef int16_t (*proc_dll_t)();

struct loaded_dll_t
{
    HMODULE dll;
    proc_dll_t initialize_dll;
};

int16_t initialize_hot_loader();
int16_t initialize_loaded_dlls();