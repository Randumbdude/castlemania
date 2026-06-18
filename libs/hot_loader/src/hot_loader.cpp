#include "hot_loader.h"
#include <iostream>
#include <filesystem>
#include <windows.h>

namespace fs = std::filesystem;

int initialize_hot_loader()
{
#if defined(_WIN32)
    std::cout << "Running on Windows." << std::endl;
#elif defined(__linux__)
    std::cout << _H_L__EX_CD_1.error_message << std::endl;
    std::exit(0);
#else
    std::cout << _H_L__EX_CD_2.error_message << std::endl;
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

    std::cout << std::endl;
    return 0;
}