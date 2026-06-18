#include <iostream>
#include <cstdint>

extern "C" __declspec(dllexport) int16_t _HL_DLL_INIT()
{
	std::cout << "Hello I am a dummy dll process to be loaded by castlemania's new hot loader. Pretty hot right?" << std::endl;
	return 0;
}
