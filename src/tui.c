#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include "tui.h"
#include "draw.h"
#include "widgets.h"


// Variables : Everything is static because it shouldn't be used outside this file (could change)
static HANDLE h_console;            // Handle for the console
static HANDLE h_stdin;
static HANDLE h_inpthr;
static CHAR_INFO *ci_screen;        // Buffer to write characters to
CHAR_INFO **tui_current_screen;     // Pointer to the current screen
int sn_screenwidth;
int sn_screenheight;
int i_bufsize;                      // Size of *ci_screen in elements

// Root frame and widget wrapper
sWidget *w_root;
sFrame  *f_root;


// Needed for initialisation
static COORD c_screensize;
static SMALL_RECT sr_screensize;


int tui_init(const int n_screenwidth, const int n_screenheight) {
    // setting up error logging
    init_stderr();

    // New console process
    if (!FreeConsole())
        win_err("FreeConsole");
    if (!AllocConsole())
        win_err("AllocConsole");
    
    h_stdin = GetStdHandle(STD_INPUT_HANDLE);
    if (h_stdin == INVALID_HANDLE_VALUE)
        win_err("Bad Handle");

    h_console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
                                          0,
                                          NULL,
                                          CONSOLE_TEXTMODE_BUFFER,
                                          NULL);
    
    if (h_console == INVALID_HANDLE_VALUE)
        win_err("Bad Handle");
    ci_screen = alloc_ci_array(n_screenwidth, n_screenheight);
    tui_current_screen = &ci_screen;
    sn_screenwidth = n_screenwidth;
    sn_screenheight = n_screenheight;
    
    /*
     * Slightly hacky way of setting up the console.
     * Sets the console window size to be very small, then scales it back
     * up to the size of the buffer
     */
    c_screensize = (COORD) {(short)n_screenwidth, (short)n_screenheight};
    sr_screensize = (SMALL_RECT) {0, 0, (short)n_screenwidth - 1, (short)n_screenheight - 1};

    if (!SetConsoleWindowInfo(h_console, TRUE, &((SMALL_RECT) {0, 0, 1, 1})))
        win_err("SetConsoleWindowInfo 1");
    if (!SetConsoleScreenBufferSize(h_console, c_screensize))
        win_err("SetConsoleScreenBufferSize");
    if (!SetConsoleActiveScreenBuffer(h_console))
        win_err("SetConsoleActiveScreenBuffer");
    if (!SetConsoleWindowInfo(h_console, TRUE, &sr_screensize))
        win_err("SetConsoleWindowInfo 2");
    if (!SetConsoleMode(h_console, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
        win_err("SetConsoleMode 1");
    if (!SetConsoleMode(h_stdin, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
        win_err("SetConsoleMode 2");

    // Remove the blinking cursor
    if (!SetConsoleCursorInfo(h_console, &((CONSOLE_CURSOR_INFO) {1, FALSE})))
        win_err("SetConsoleCursorInfo");

    // Threading setup for input handling
    h_inpthr = CreateThread(NULL,
                            0,
                            inpthr_loop,
                            NULL,
                            0,
                            NULL);

    if (h_inpthr == INVALID_HANDLE_VALUE)
        win_err("CreateThread");

    // Other stuff
    tui_root_frame();

    return 1;
}

void tui_root_frame() {
    w_root = (sWidget *)malloc(sizeof(sWidget));
    f_root = (sFrame *)malloc(sizeof(sFrame));

    w_root->type         = FRAME;
    w_root->px           = 0;
    w_root->py           = 0;
    w_root->min_width    = sn_screenwidth;
    w_root->min_height   = sn_screenheight;
    w_root->parent       = NULL;
    w_root->widget.frame = f_root;

    for(int i = 0; i < 16; i++)
        f_root->children[i] = NULL;
}

CHAR_INFO * alloc_ci_array(const int n_screenwidth, const int n_screenheight) {
        CHAR_INFO *ptr = (CHAR_INFO *)calloc(n_screenwidth * n_screenheight, sizeof(CHAR_INFO));

    if (ptr == NULL)
        tui_err("Calloc failed to allocate memory", TUI_ERROR, 1);

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
                tui_err("Key Event", TUI_OTHER, 0);
                break;

            case MOUSE_EVENT:
                tui_err("Mouse Event", TUI_OTHER, 0);
                break;

            case WINDOW_BUFFER_SIZE_EVENT:
                tui_err("Window Buffer Size Event", TUI_OTHER, 0);
                break;

            case FOCUS_EVENT: case MENU_EVENT: // Ignore these
                break;
        }
    }
}

void tui_draw() {
    // Basic drawing for now
    // Resetting each element
    reset_buf(*tui_current_screen);   

    if(!WriteConsoleOutputW(h_console,
                            *tui_current_screen,
                            c_screensize,
                            (COORD) {0, 0},
                            &sr_screensize))
        win_err("WriteConsoleOutput");


}

void inpthr_loop() {
    while(1) {
        tui_handle_input();
    }
}

void tui_loop() {
    calculate_min_size(w_root);

    while(1) {
        tui_draw();
    }
}
