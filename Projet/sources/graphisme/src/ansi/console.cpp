#include <iostream>
#include <string>
#include <cstring>
#if defined(_WIN32)
#include <windows.h>
#endif
#include "console.hpp"


void console::init()
{
#if defined(_WIN32)
    HANDLE hconsole;
    hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hconsole != INVALID_HANDLE_VALUE)
    {
        DWORD mode;
        if (GetConsoleMode(hconsole, &mode))
        {
            mode = mode|ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hconsole, mode);
        }
    }
#endif
}

void console::restaure()
{
    std::cout << "\u001b[0m";
}

void console::cls(couleur col)
{
    col.convertir_pour_papier();
    std::cout << col << "\x1b[2J\x1b[H";
}
