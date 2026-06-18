#pragma once
#include <cstdint>
#include <string>
typedef std::string string;
typedef struct
{
    uint32_t error_code;
    string error_message;
} _hl_error;

#define _H_L__EX_CD_0 _hl_error{0, "(0)"}
#define _H_L__EX_CD_1 _hl_error{1, "Error: Linux is not supported!"}
#define _H_L__EX_CD_2 _hl_error{2, "Error: Unknown OS!"}