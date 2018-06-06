#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <windows.h>


void init_stderr();

int win_err(const char *msg);

int tui_err(const char *msg);

#endif
