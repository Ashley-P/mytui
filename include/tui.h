#ifndef TUI_H_
#define TUI_H_

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <windows.h>
#include "utils.h"


int inittui(const int n_screenwidth, const int n_screenheight);

wchar_t * allocwcarray(const int n_screenwidth, const int n_screenheight); // Creates a wide character array


#endif
