#pragma once
#if GAME_PLATFORM_WINDOWS
#include <Windows.h>
#endif
#include <iostream>

namespace cs
{
    inline bool enableLock = false;

    namespace implementation
    {
        template <typename... Params>
        inline void printToConsole(Params&&... params)
        {
            showConsole();
#if GAME_PLATFORM_WINDOWS
            ((std::cout << std::forward<Params>(params)), ...);
            std::cout << std::endl;
#else
#error Unsupported logger platform
#endif
        }

        inline void hideConsole()
        {
#if GAME_PLATFORM_WINDOWS
            ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
#else
#error Unsupported logger platform
#endif
        }

        inline void showConsole()
        {
            if (cs::enableLock)
                return;

#if GAME_PLATFORM_WINDOWS
            auto window = ::GetActiveWindow();
            ::ShowWindow(::GetConsoleWindow(), SW_SHOW);
            ::SetActiveWindow(window);
#else
#error Unsupported logger platform
#endif
        }

        inline bool isConsoleVisible()
        {
#if GAME_PLATFORM_WINDOWS
            return ::IsWindowVisible(::GetConsoleWindow()) != FALSE;
#else
#error Unsupported logger platform
#endif
        }
    }

    template <typename... Params>
    inline void printLog(Params&&... params)
    {
#if DEBUG
        implementation::printToConsole(params...);
#endif // DEBUG
    }

    template <typename... Params>
    inline void printError(Params&&... params)
    {
        implementation::printToConsole(params...);
    }

    inline void showConsole()
    {
#if DEBUG
        implementation::showConsole();
#endif // DEBUG
    }

    inline void showConsoleImportant()
    {
        implementation::showConsole();
    }

    inline void hideConsole()
    {
        implementation::hideConsole();
    }

    inline bool isConsoleVisible()
    {
        return implementation::isConsoleVisible();
    }
}
