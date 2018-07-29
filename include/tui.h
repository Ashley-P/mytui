#ifndef TUI_H_
#define TUI_H_
#include <windows.h>

extern int sn_screenwidth;
extern int sn_screenheight;
extern int i_bufsize;           // Size of *ci_screen in elements
extern CHAR_INFO **tui_current_screen;

int tui_init(const int n_screenwidth, const int n_screenheight);

void tui_root_frame();

CHAR_INFO * alloc_ci_array(const int n_screenwidth, const int n_screenheight); // Creates an array of CHAR_INFO structs 

void tui_handle_input();

void tui_draw();

void inpthr_loop();

void tui_loop();

#endif
