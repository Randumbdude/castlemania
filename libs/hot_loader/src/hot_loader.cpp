#include "hot_loader.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

// global variable for the vector of current dlls loaded
std::vector<loaded_dll_t> _hl_loaded_dlls;

int initialize_hot_loader()
{
#if defined(_WIN32)
    std::cout << "Running on Windows." << std::endl;
#elif defined(__linux__)
#else
    std::cout << _HL__E_FATAL_1.error_message << std::endl;
    std::exit(0);
#endif
    // first create the ".hl_mksr" directory
    if (!fs::exists(_H_L__DEF))
    {
        std::cout << "Creating \"" + _H_L__DEF + "\" directory." << std::endl;
        fs::create_directory(_H_L__DEF);
    }
    // set a list the contents of the ".hl_mksr" directory
    std::vector<string> _hl_dir_list;
    for (const auto &i : fs::directory_iterator(_H_L__DEF))
    {
        _hl_dir_list.push_back(i.path().string());
    }
    // handling of the ".hl_mksr" directory to prepare for hot loading
    size_t _hl_dl_s = _hl_dir_list.size();
    std::cout << _hl_dl_s << " \'dll\' " << ((_hl_dl_s == 1) ? "file" : "files") << " found." << std::endl;
    // iterate through the ".hl_mksr" directory
    for (size_t i = 0; i < _hl_dl_s; i++)
    {
        std::cout << "Hot loading " << _hl_dir_list[i] << std::endl;
        // Use the ANSI variant when passing a narrow C-string
        HMODULE dll = LoadLibraryA(_hl_dir_list[i].c_str());
        if (!dll)
        {
            std::cout << _HL__E_MINOR_1.error_message << std::endl;
            DWORD err = GetLastError();
            std::cout << " -More Info: " << err << std::endl;
        }
        proc_dll_t initialize_dll = (proc_dll_t)GetProcAddress(dll, "_HL_DLL_INIT");
        if (!initialize_dll)
        {
            std::cout << _HL__E_MINOR_2.error_message << std::endl;
            FreeLibrary(dll);
        }
        initialize_dll();
        // now that the dll has been loaded and initialized, we slap it in our list for later.
        loaded_dll_t _hl_ld_dll;
        _hl_ld_dll.dll = dll;
        _hl_ld_dll.initialize_dll = initialize_dll;
        // this puts it in the vector of loaded dlls
        _hl_loaded_dlls.push_back(_hl_ld_dll);
    }
    std::cout << std::endl;
    return 0;
}