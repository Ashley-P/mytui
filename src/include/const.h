#ifndef CONST_H_
#define CONST_H_

#include <windows.h>
#include "widgets.h"

extern int sn_screenwidth;
extern int sn_screenheight;
extern int i_bufsize;           // Size of *ci_screen in elements
extern HANDLE h_stdin;
extern CHAR_INFO **tui_current_screen;

extern struct tWidget *focused_wid;
extern struct tWidget *w_root; // Specially created root widget

#endif
