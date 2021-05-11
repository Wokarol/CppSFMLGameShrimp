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

    template <typename... Params>
    void Print(Params&&... params)
    {
#ifdef DEBUG
        ShowConsole();
        ((std::cout << std::forward<Params>(params)), ...);
        std::cout << std::endl;
#endif // DEBUG
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