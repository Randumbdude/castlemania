#pragma once

#include <cstdint>
#include <string>
#include <windows.h>

typedef std::string string;

typedef struct
{
    int16_t error_code;
    string error_message;
} _hl_error;

#define _HL__E_MINOR_1 _hl_error{-1, "Warning: Failed to load DLL!"}
#define _HL__E_MINOR_2 _hl_error{-2, "Warning: Failed to execute DLL initialize!"}
#define _HL__EX_CD_0 _hl_error{0, "(0)"}
#define _HL__E_FATAL_1 _hl_error{1, "Fatal Error: Linux is not supported!"}
#define _HL__E_FATAL_2 _hl_error{2, "Fatal Error: Unknown OS!"}
#define _HL__E_FATAL_3 _hl_error{3, "Fatal Error: Failed to hot load DLL!"}