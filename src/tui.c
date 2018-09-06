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

// Specially created root widget
sWidget *w_root;


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


CHAR_INFO * alloc_ci_array(const int n_screenwidth, const int n_screenheight) {
        CHAR_INFO *ptr = (CHAR_INFO *)calloc(n_screenwidth * n_screenheight, sizeof(CHAR_INFO));

    if (ptr == NULL)
        tui_err("Calloc failed to allocate memory", TUI_ERROR, 1);

    i_bufsize = n_screenwidth * n_screenheight;

    return ptr;
}

void tui_handle_input() {
    static sWidget *old_wid = NULL; 
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
                /* Basic tracking of events in the error log */
                tui_err("Mouse Event", TUI_OTHER, 0);
                MOUSE_EVENT_RECORD *ev = &ir_inpbuf[i].Event.MouseEvent;
                /*
                 * Finding the widget
                 * Magic number picked for the size of the stack
                 */
                sStack *wid_search = create_stack(16);
                find_widget(wid_search, w_root, ev->dwMousePosition.X, ev->dwMousePosition.Y);
                sWidget *wid = stack_pop(wid_search);
                free(wid_search->arr);
                free(wid_search);
                /* Cases get handled in their own function to make the code more readable */
                switch(wid->type) {
                    case FRAME:  frame_mouse_event(wid, &old_wid, ev); break;
                    case BUTTON: button_mouse_event(wid, &old_wid, ev); break;

                    default: break;
                }
                break;

            case WINDOW_BUFFER_SIZE_EVENT:
                tui_err("Window Buffer Size Event", TUI_OTHER, 0);
                break;

            case FOCUS_EVENT: case MENU_EVENT: // Ignore these
                break;
        }
    }
}

void frame_mouse_event(sWidget *a, sWidget **old, MOUSE_EVENT_RECORD *ev) {
    /* Basic stuff just for getting highlighting buttons to work properly */
    tui_err("ENTERED FRAME_MOUSE_EVENT", TUI_OTHER, 0);
    if (a == *old) {
        return;
    } else {
        if (*old)
            (*old)->state = NONE;
        a->state = HOVER;
        *old = a;
    }

}

void button_mouse_event(sWidget *a, sWidget **old, MOUSE_EVENT_RECORD *ev) {
    tui_err("ENTERED BUTTON_MOUSE_EVENT()", TUI_OTHER, 0);
    switch (ev->dwButtonState) {
        case FROM_LEFT_1ST_BUTTON_PRESSED:
            tui_err("FROM_LEFT_1ST_BUTTON_PRESSED", TUI_OTHER, 0);
            break;
        default:
            tui_err("NO_BUTTON_PRESSED", TUI_OTHER, 0);
            if (a == *old) {
                return;
            } else {
                /* for the first activation because you can't dereference a null pointer */
                if (*old)
                    (*old)->state = NONE;
                a->state = HOVER;
                *old = a;
            }

            break;
    }
}

void find_widget(sStack *stack, sWidget *a, int x, int y) {
    stack_push(stack, a);
    switch (a->type) {
        case FRAME:
            for(int i = 0; i < MAX_CHILDREN; i++) {
                if (a->widget.frame.children[i]) {
                    sWidget *b = a->widget.frame.children[i];
                    if ((x >= b->pos.x && x <= (b->pos.x + b->size.x - 1)) &&
                        (y >= b->pos.y && y <= (b->pos.y + b->size.y - 1))) {
                        find_widget(stack, a->widget.frame.children[i], x, y);
                    }
                }
            }
            break;
        case BUTTON:
            break;
        default:
            break;
    }
}

void tui_draw(sWidget *a) {
    // Basic drawing for now
    // Resetting each element
    reset_buf(*tui_current_screen);   
    tui_draw__(a);
    if(!WriteConsoleOutputW(h_console,
                            *tui_current_screen,
                            c_screensize,
                            (COORD) {0, 0},
                            &sr_screensize))
        win_err("WriteConsoleOutput");


}

void tui_draw__(sWidget *a) {
    switch (a->type) {
        case FRAME:
            /* Just draw a box then iterate through children */
            draw_box(a->pos.x, a->pos.y, a->size.x, a->size.y, 0, 0x90);
            sFrame *af = &a->widget.frame;
            for(int i = 0; i < MAX_GRID_COLS; i++) {
                for(int k = 0; k < MAX_GRID_ROWS; k++) {
                if (af->grid[i][k])
                    tui_draw__(af->grid[i][k]);
                }
            }
            break;
        case BUTTON:
            /* Just draw a string and change background for now */
            draw_button(a);
            break;
        default:
            break;
    }
}

void inpthr_loop() {
    while(1) {
        tui_handle_input();
    }
}

void tui_loop() {
    /* Stuff that needs to happen after setup but before the loop starts */
    /* Even though calculate_min_size returns sSize, it's not used for the top level
     * frame because it comes preset.
     */
    widget_sizer(w_root);
    widget_positioner(w_root);

    while(1) {
        tui_draw(w_root);
    }
}
