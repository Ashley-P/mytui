#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include "tui.h"
#include "draw.h"
#include "widgets.h"
#include "input.h"
#include "const.h"


// Variables : Everything is static because it shouldn't be used outside this file (could change)
static HANDLE h_console;            // Handle for the console
static CHAR_INFO *ci_screen;        // Buffer to write characters to
static HANDLE h_inpthr;
HANDLE h_stdin;
CHAR_INFO **tui_current_screen;     // Pointer to the current screen
int sn_screenwidth;
int sn_screenheight;
int i_bufsize;                      // Size of *ci_screen in elements

// Specially created root widget
sWidget *w_root;
sWidget *focused_wid;

// Needed for initialisation
static COORD c_screensize;
static SMALL_RECT sr_screensize;

CHAR_INFO * alloc_ci_array(const int n_screenwidth, const int n_screenheight) {
        CHAR_INFO *ptr = (CHAR_INFO *)calloc(n_screenwidth * n_screenheight, sizeof(CHAR_INFO));

    if (ptr == NULL)
        tui_err(TUI_ERROR, 1, "Error in alloc_ci_array. Calloc failed to allocate memory");

    return ptr;
}

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
    i_bufsize = n_screenwidth * n_screenheight;
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
                            (LPTHREAD_START_ROUTINE) inpthr_loop,
                            NULL,
                            0,
                            NULL);

    if (h_inpthr == INVALID_HANDLE_VALUE)
        win_err("CreateThread");

    // Other stuff
    tui_root_frame();
    init_time();
    focused_wid = NULL;

    return 1;
}


void find_widget(sStack *stack, sWidget *a, const int x, const int y) {
    stack_push(stack, a);
    switch (a->type) {
        case FRAME:
            for(int i = 0; i < MAX_CHILDREN; i++) {
                if (a->widget.frame.children[i]) {
                    sWidget *b = a->widget.frame.children[i];
                    /*
                    if ((x >= b->pos.x && x <= (b->pos.x + b->csize.x - 1)) &&
                        (y >= b->pos.y && y <= (b->pos.y + b->csize.y - 1))) {
                        find_widget(stack, a->widget.frame.children[i], x, y);
                    */
                    /* Check if it's actually pressing inside csize */
                    /*
                    if ((x >= (b->pos.x + b->msize.x + b->bsize.x + b->psize.x)
                      && x <= (b->pos.x + b->rsize.x - b->msize.x + b->bsize.x + b->psize.x - 1))
                      && y >= (b->pos.y + b->msize.y + b->bsize.y + b->psize.y)
                      && y <= (b->pos.y + b->rsize.y - b->msize.y + b->bsize.y + b->psize.y - 1))
                        find_widget(stack, a->widget.frame.children[i], x, y);
                    */
                    if ((x >= b->cpos.x && x <= b->cpos.x + b->csize.x - 1) &&
                        (y >= b->cpos.y && y <= b->cpos.y + b->csize.y - 1))
                        find_widget(stack, a->widget.frame.children[i], x, y);
                    }
                }
            break;
        default:
            break;
    }
}

void tui_draw_helper(sWidget *a) {
    switch (a->type) {
        case FRAME:
            /* Just draw a box then iterate through children */
            draw_frame(a, 0);
            sFrame *af = &a->widget.frame;
            for(int i = 0; i < MAX_GRID_COLS; i++) {
                for(int k = 0; k < MAX_GRID_ROWS; k++) {
                if (af->grid[i][k])
                    tui_draw_helper(af->grid[i][k]);
                }
            }
            break;
        case BUTTON:      draw_button(a);      break;
        case LABEL:       draw_label(a);       break;
        case CHECKBOX:    draw_checkbox(a);    break;
        case RADIOBUTTON: draw_radiobutton(a); break;
        case CANVAS:      draw_canvas(a);      break;
        case FIELD:       draw_field(a);       break;
        default: break;
    }
}

void tui_draw(sWidget *a) {
    // Basic drawing for now
    // Resetting each element
    reset_buf(*tui_current_screen, i_bufsize);   
    tui_draw_helper(a);
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
    /* Stuff that needs to happen after setup but before the loop starts */
    redraw_widgets(w_root);

    while(1) {
        tui_draw(w_root);
    }
}
