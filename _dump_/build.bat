@echo off
echo compiling...
clang++ -std=c++17 dll_main.cpp -shared -o test.dll
pause
