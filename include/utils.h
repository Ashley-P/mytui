#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <windows.h>


void init_stderr();

void win_err(const char *msg);

void tui_err(const char *msg, const int quit_prog);

#endif
