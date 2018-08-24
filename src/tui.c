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

void tui_root_frame() {
    w_root = (sWidget *)malloc(sizeof(sWidget));

    w_root->type               = FRAME;
    w_root->pos.x              = 0;
    w_root->pos.y              = 0;
    w_root->parent             = NULL;
    w_root->minsize.x          = sn_screenwidth;
    w_root->minsize.y          = sn_screenheight;
    w_root->widget.frame.numch = 0;

    for(int i = 0; i < MAX_CHILDREN; i++)
        w_root->widget.frame.children[i] = NULL;

    for(int j = 0; j < MAX_GRID_COLS; j++) {
        for(int k = 0; k < MAX_GRID_COLS; k++) {
            w_root->widget.frame.grid[j][k] = NULL;
        }
    }
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
            draw_box(a->pos.x, a->pos.y, a->realsize.x, a->realsize.y, 0, 0x80);
            sFrame *af = &a->widget.frame;
            for(int i = 0; i < MAX_CHILDREN; i++) {
                if (af->children[i])
                    tui_draw__(af->children[i]);
            }
            break;
        case BUTTON:
            /* Just draw a string and change background for now */
            ; // Empty statement because labels cannot precede declarations
            sButton *ab = &a->widget.button;
            draw_button(ab->text, a->pos.x, a->pos.y, a->realsize.x, a->realsize.y);
            break;
        default:
            break;
    }
}

void widget_positioner(sWidget *a) {
    /*
     * Sets up realsize and pos structs in the widget recursively. Generally the top level widget of
     * the window should be given as the argument.
     * Called when the program starts and when windows are created or resized.
     * This relies on previous calls to set the cursor into position;
     */

    /* Struct for the position of the "cursor" that places everything down */
    static sSize s_cursor = {0, 0};

    switch (a->type) {
        case FRAME:
            /* w_root size is preset so we don't change it */
            if (a != w_root)
                a->pos = s_cursor;
            /* Right now realsize = minsize until more options are implemented */
            a->realsize = a->minsize;

            /* Cursor movement happens here, this is where extra movement due to margins would occur */
            s_cursor = add_sSize(s_cursor, (sSize) {1, 1});
            sSize s_temp = s_cursor;

            sFrame *af = &a->widget.frame;
            /* Iterating through the frame's children */
            for(int i = 0; i < MAX_GRID_COLS; i++) {
                for(int j = 0; j < MAX_GRID_ROWS; j++) {
                    if(af->grid[i][j])
                        widget_positioner(af->grid[i][j]);
                    s_cursor = add_sSize(s_cursor, (sSize) {0, af->rows_size[j] + 1});
                }
                /* Moving back to the top */
                s_cursor = s_temp;
                s_cursor = add_sSize(s_cursor, (sSize) {af->cols_size[i] + 1, 0});
            }

            s_cursor = add_sSize(a->pos, (sSize) {1, 1});
                
            break;
        case BUTTON:
            a->pos = s_cursor;
            /* Right now realsize = minsize until more options are implemented */
            a->realsize = a->minsize;
            break;
        default:
            tui_err("widget positioner default", 1, 0);
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
    calculate_min_size(w_root);
    widget_positioner(w_root);

    while(1) {
        tui_draw(w_root);
    }
}
