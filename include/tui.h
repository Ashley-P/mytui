#ifndef TUI_H_
#define TUI_H_


int init_tui(const int n_screenwidth, const int n_screenheight);

wchar_t * alloc_wc_array(const int n_screenwidth, const int n_screenheight); // Creates a wide character array

void tui_draw();

void tui_loop();

#endif
