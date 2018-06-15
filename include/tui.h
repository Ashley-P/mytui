#ifndef TUI_H_
#define TUI_H_
#include <windows.h>

extern int sn_screenwidth;
extern int sn_screenheight;
extern int i_bufsize;

int tui_init(const int n_screenwidth, const int n_screenheight);

wchar_t * alloc_wc_array(const int n_screenwidth, const int n_screenheight); // Creates a wide character array

CHAR_INFO * alloc_ci_array(const int n_screenwidth, const int n_screenheight); // Creates an array of CHAR_INFO structs 

void tui_handle_input();

void tui_draw();

void tui_loop();

#endif
