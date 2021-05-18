#pragma once
#if GAME_PLATFORM_WINDOWS
#include <Windows.h>
#endif
#include <iostream>

namespace cs
{
    inline bool enableLock = false;

    inline void HideConsole()
    {
#if GAME_PLATFORM_WINDOWS
        ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
#endif
    }

    inline void ShowConsole()
    {
        if (enableLock)
            return;

#if GAME_PLATFORM_WINDOWS
        auto window = ::GetActiveWindow();
        ::ShowWindow(::GetConsoleWindow(), SW_SHOW);
        ::SetActiveWindow(window);
#endif
    }

    template <typename... Params>
    inline void Print(Params&&... params)
    {
#ifdef DEBUG
        ShowConsole();
        ((std::cout << std::forward<Params>(params)), ...);
        std::cout << std::endl;
#endif // DEBUG
    }

    inline bool IsConsoleVisible()
    {
#if GAME_PLATFORM_WINDOWS
        return ::IsWindowVisible(::GetConsoleWindow()) != FALSE;
#else
        return 0;
#endif
    }
}
