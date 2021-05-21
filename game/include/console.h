#pragma once
#if GAME_PLATFORM_WINDOWS
#include <Windows.h>
#endif
#include <iostream>

namespace console
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
            if (console::enableLock)
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
    inline void log(Params&&... params)
    {
#if DEBUG
        implementation::printToConsole(params...);
#endif // DEBUG
    }

    template <typename... Params>
    inline void error(Params&&... params)
    {
        implementation::printToConsole(params...);
    }

    inline void show()
    {
#if DEBUG
        implementation::showConsole();
#endif // DEBUG
    }

    inline void showImportant()
    {
        implementation::showConsole();
    }

    inline void hide()
    {
        implementation::hideConsole();
    }

    inline bool isVisible()
    {
        return implementation::isConsoleVisible();
    }
}
