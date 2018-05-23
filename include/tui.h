#ifndef TUI_H_
#define TUI_H_

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <windows.h>

void initTui();

HANDLE createRoot(const int nScreenWidth, const int nScreenHeight); // Basic setup to get the console ready

wchar_t * allocwArray(const int nScreenWidth, const int nScreenHeight); // Creates a wide character array

#endif
