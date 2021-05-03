#pragma once

#if GAME_PLATFORM_WINDOWS
#include <Windows.h>
#endif
#include <iostream>

namespace cs
{
    void HideConsole()
    {
#if GAME_PLATFORM_WINDOWS
        ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
#endif
    }

    void ShowConsole()
    {
#if GAME_PLATFORM_WINDOWS
        ::ShowWindow(::GetConsoleWindow(), SW_SHOW);
#endif
    }

    void Print(std::string p)
    {
        ShowConsole();
#if GAME_PLATFORM_WINDOWS
        std::cout << p << std::endl;
#endif
    }

    bool IsConsoleVisible()
    {
#if GAME_PLATFORM_WINDOWS
        return ::IsWindowVisible(::GetConsoleWindow()) != FALSE;
#else
        return 0;
#endif
    }
}