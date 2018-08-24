# Mytui Documentation

## Description

mytui is an effort by the author to create a text based user interface using the windows console.

## License

There isn't one yet

---
## The \<tui.h\> Header

The \<tui.h\> header file contains all the declarations for the functions and datatypes

### Datatypes

### Variables

The variables here are for internal use by the library

    HANDLE h_console                handle for the console
    HANDLE h_stderr                 handle for stderr which is redirected to a file
    HANDLE h_inpthr                 handle for the thread that reads the input buffer
    CHAR_INFO *ci_screen            CHAR_INFO array that gets passed to the console
    CHAR_INFO **tui_current_screen  A pointer to the current screen being used
    int i_bufsize                   size of *wc_screen in elements
    int sn_screenwidth              global screenx
    int sn_screenheight             global screeny
    sWidget *w_root                 root widget struct
    COORD c_screensize              initialises the buffer size
    SMALL_RECT sr_screensize        initialises the window size


### Constants

### Structs

### Functions

#### Synopsis

    int init_tui(const int n_screenwidth, const int n_screenheight);
    void tui_root_frame();
    CHAR_INFO * alloc_ci_array(const int n_screenwidth, const int n_screenheight);
    void tui_handle_input();
    void tui_draw(sWidget *a);
    void tui_draw__(sWidget *a);
    void inpthr_loop();
    void tui_loop();
    

#### Description

    init_tui() creates a new console process and a screen buffer for it. stderr
    is redirected to an output file and an array is created to fit the desired
    screen size. Then the screen buffer is linked to the console and the screen
    size is changed.

    tui_root_frame() creates a special root frame and widget struct for all the
    user created widgets to be a child to.

    alloc_ci_array() returns a pointer to an array of CHAR_INFO structs.

    tui_handle_input() handles all the events for the program

    tui_draw() is where all the drawing to the buffer is handled

    tui_draw__() is a helper for tui_draw. It's purpose is to be called recursively
    over the tree created by the user.

    void widget_positioner sets the coords for each widget which tui_draw uses

    inpthr_loop() is the looping for h_inpthr it just calls tui_handle_input

    tui_loop() is the looping for the program, drawing and other functions are
    called sequentially


---
## The \<utils.h\> Header

The \<utils.h\> header file contains declarations for some utility functions such as error handling

### Datatypes

### Variables

### Constants
    
    TUI_ERROR   1
    TUI_WARNING 2
    TUI_OTHER   3

### Structs

### Functions

#### Synopsis
    
    void init_stderr
    void win_err(const char *msg);
    void tui_err(const char *msg, const int err_type, const int quit_prog);


#### Description

    init_stderr() creates a new file to log errors in. If the file already
    exists, it's overwritten.

    win_err() puts a formatted error message from the \<windows.h\> functions into a
    logging file.

    tui_err() is similar to win_err() in that it puts a formatted error/warning/other message into
    a logging file, and also offers the opportunity to quit the program all together.


---
## The \<draw.h\> Header

The \<draw.h\> header file contains all the declarations for the functions and datatypes

### Datatypes

### Variables

### Constants

### Structs

### Functions

#### Synopsis

    void reset_buf();
    void draw_box(int x, int y, const int x, const int y, const bool fill);
    void draw_str(const wchar_t *str, const size_t str_len, int x, int y);
    void draw_button(sWidget *a) {

#### Description
    
    reset_buf() Fills the buffer with spaces.
    Used after every draw to the screen

    draw_box() Draws a box of the desired x and y at the provided co-ordinates x and y.
    The character used is the hash '#'. The box can be filled or just left with the borders

    draw_str() Draws a string to the screen at the desired position.

    draw_button() Draws a button to the screen at the desired position


## The \<widgets.h\> Header

The \<widgets.h\> header contains all the widgets that the user of this library should be calling.

### Datatypes

    enum eType {
        FRAME  = 1 << 0,
        BUTTON = 1 << 1
    };

    typedef struct tSize {
        int x;
        int y;
    } sSize, sPos;

    typedef struct tFrame {
        void (*draw)();
        int numch;
        struct tWidget *children[MAX_CHILDREN];
        struct tWidget *grid[MAX_GRID_x][MAX_GRID_y];
        struct tSize *cols_size[MAX_GRID_COLS];
        struct tSize *rows_size[MAX_GRID_ROWS];
    } sFrame, *pFrame;

    typedef struct tButton {
        wchar_t *text;
        void (*draw)();
        void (*callback)();
    } sButton, *pButton;


    typedef struct tWidget {
        int px;
        int py;
        enum eType type; 
        sPos  pos;
        sSize size;
        struct tWidget *parent;
        union {
            struct tButton button;
            struct tFrame  frame;
        } widget;
    } sWidget, *pWidget;

### Variables

### Constants

    MAX_CHILDREN    16
    MAX_GRID_COLS   16
    MAX_GRID_ROWS   16

### Structs

### Functions

#### Synopsis
    
    sWidget * tui_frame(const sWidget *parent);
    sWidget * tui_button(const sWidget *parent, wchar_t *text, void (*callback)());
    void widget_sizer(sWidget *a);
    sSize add_sSize(sSize a, sSize b);
    sSize max_sSize(sSize a, sSize b);
    int parent_widget_type(sWidget *widget);
    void grid_set(sWidget *widget, int col, int row);

#### Description

    tui_frame creates an sWidget struct with the internal type of FRAME and returns a pointer to it.

    tui_button creates an sWidget struct with the internal type of BUTTON and returns a pointer to it.

    widget_sizer fills out the sSize struct in frames and other widgets that can have children

    add_sSize adds to sSize structs together and returns the resulting struct

    max_sSize compares two sSize structs and returns a struct with the largest of each variables
    
    parent_widget_type checks the type of the widget. If it is not of a type that can have children then it
    invokes tui_err and quits with program with an error.

    grid_set enters the position of the widget supplied into it's parent's grid array. Sends a warning if
    it overwrites something (aka the element is not NULL)
---
