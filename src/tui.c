#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include "utils.h"
#include "tui.h"
#include "draw.h"


// Variables : Everything is static because it shouldn't be used outside this file (could change)
static HANDLE h_console;            // Handle for the console
static HANDLE h_stdin;
static wchar_t *wc_screen;          // Buffer to write characters to
static DWORD dw_bytes_written = 0;  // Required by windows.h
int i_bufsize;               // Size of *wc_screen in elements
int sn_screenwidth;
int sn_screenheight;

// Needed for initialisation
static COORD c_screensize;
static SMALL_RECT y;


int tui_init(const int n_screenwidth, const int n_screenheight) {
    // setting up error logging
    init_stderr();
    
    h_stdin = GetStdHandle(STD_INPUT_HANDLE);
    if (h_stdin == INVALID_HANDLE_VALUE)
        win_err("Bad Handle");


    // New console process
    if (!FreeConsole())
        win_err("FreeConsole");
    if (!AllocConsole())
        win_err("AllocConsole");

    h_console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
                                                0,
                                                NULL,
                                                CONSOLE_TEXTMODE_BUFFER,
                                                NULL);
    
    if (h_console == INVALID_HANDLE_VALUE)
        win_err("Bad Handle");
    wc_screen = alloc_wc_array(n_screenwidth, n_screenheight);
    sn_screenwidth = n_screenwidth;
    sn_screenheight = n_screenheight;
    
    /*
     * Slightly hacky way of setting up the console.
     * Sets the console window size to be very small, then scales it back
     * up to the size of the buffer
     */
    c_screensize = (COORD) {(short)n_screenwidth, (short)n_screenheight};
    y = (SMALL_RECT) {0, 0, (short)n_screenwidth - 1, (short)n_screenheight - 1};

    if (!SetConsoleWindowInfo(h_console, TRUE, &((SMALL_RECT) {0, 0, 1, 1})))
        win_err("SetConsoleWindowInfo");
    if (!SetConsoleScreenBufferSize(h_console, c_screensize))
        win_err("SetConsoleScreenBufferSize");
    if (!SetConsoleActiveScreenBuffer(h_console))
        win_err("SetConsoleActiveScreenBuffer");
    if (!SetConsoleWindowInfo(h_console, TRUE, &y))
        win_err("SetConsoleWindowInfo");
    if (!SetConsoleMode(h_console, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
        win_err("SetConsoleMode");

    // Remove the blinking cursor
    if (!SetConsoleCursorInfo(h_console, &((CONSOLE_CURSOR_INFO) {1, FALSE})))
        win_err("SetConsoleCursorInfo");

    return 1;
}

wchar_t * alloc_wc_array(const int n_screenwidth, const int n_screenheight) {
    wchar_t *ptr = (wchar_t *)calloc(n_screenwidth * n_screenheight, sizeof(wchar_t));

    if (ptr == NULL)
        tui_err("Calloc failed to allocate memory", 1);

    i_bufsize = n_screenwidth * n_screenheight;

    return ptr;
}

void tui_handle_input() {
    unsigned long ul_evread;
    INPUT_RECORD ir_inpbuf[256];

    if (!ReadConsoleInput(h_stdin,
                          ir_inpbuf,
                          256,
                          &ul_evread))
        win_err("ReadConsoleInput");

    for(int i = 0; i < ul_evread; i++) {
        switch (ir_inpbuf[i].EventType) {
            case KEY_EVENT:
                tui_err("Key Event", 0);
                break;

            case MOUSE_EVENT:
                tui_err("Mouse Event", 0);
                break;

            case WINDOW_BUFFER_SIZE_EVENT:
                tui_err("Window Buffer Size Event", 0);
                break;

            case FOCUS_EVENT: case MENU_EVENT: // Ignore these
                break;
        }
    }
}

void tui_draw() {
    // Basic drawing for now
    // Resetting each element
    reset_buf(wc_screen);   
    
    // Drawing test because it requires the buffer to be passed currently
    psButton mybutton = tui_button(5, 5, 7, 3, L"MEMES");
    draw_button(wc_screen, mybutton);

    WriteConsoleOutputCharacterW(h_console,
                                 wc_screen,
                                 i_bufsize,
                                 (COORD) {0, 0},
                                 &dw_bytes_written);
}


void tui_loop() {
    while(1) {
        tui_handle_input();
        tui_draw();
    }
}
