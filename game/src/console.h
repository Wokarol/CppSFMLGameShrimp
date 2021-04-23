#pragma once

#if GAME_PLATFORM_WINDOWS
#include <Windows.h>
#endif

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

bool IsConsoleVisible()
{
#if GAME_PLATFORM_WINDOWS
    return ::IsWindowVisible(::GetConsoleWindow()) != FALSE;
#else
    return 0;
#endif
}