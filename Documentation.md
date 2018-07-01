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
    COORD c_screensize              initialises the buffer size
    SMALL_RECT sr_screensize        initialises the window size
    int i_bufsize                   size of *wc_screen in elements
    int sn_screenwidth              global screenwidth
    int sn_screenheight             global screenheight


### Constants

### Structs

### Functions

#### Synopsis

    int init_tui(const int n_screenwidth, const int n_screenheight);
    CHAR_INFO * alloc_ci_array(const int n_screenwidth, const int n_screenheight);
    void tui_handle_input();
    void tui_draw();
    void inpthr_loop();
    void tui_loop();
    

#### Description

    init_tui() creates a new console process and a screen buffer for it. stderr
    is redirected to an output file and an array is created to fit the desired
    screen size. Then the screen buffer is linked to the console and the screen
    size is changed.

    alloc_ci_array() returns a pointer to an array of CHAR_INFO structs.

    tui_handle_input() handles all the events for the program

    tui_draw() is where all the drawing to the buffer is handled

    inpthr_loop() is the looping for h_inpthr it just calls tui_handle_input

    tui_loop() is the looping for the program, drawing and other functions are
    called sequentially


---
## The \<utils.h\> Header

The \<utils.h\> header file contains declarations for some utility functions such as error handling

### Datatypes

### Variables

### Constants

### Structs

### Functions

#### Synopsis
    
    void init_stderr
    void win_err(const char *msg);
    void tui_err(const char *msg, const int quit_prog);


#### Description

    init_stderr() creates a new file to log errors in. If the file already
    exists, it's overwritten.

    win_err() puts a formatted error message from the \<windows.h\> functions into a
    logging file.

    tui_err() is similar to win_err() in that it puts a formatted error message into
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

    void reset_buf(CHAR_INFO *buf);
    void draw_box(CHAR_INFO *buf, int x, int y, const int width, const int height, const bool fill);
    void draw_str(CHAR_INFO *buf, const wchar_t *str, const size_t str_len, int x, int y);


#### Description
    
    reset_buf() Fills the buffer with spaces.
    Used after every draw to the screen

    draw_box() Draws a box of the desired width and height at the provided co-ordinates x and y.
    The character used is the hash '#'. The box can be filled or just left with the borders

    draw_str() Draws a string to the screen at the desired position.

    draw_button() Draws a button to the screen. Really it's just calling draw_box () and 
    draw_str() together.


## The \<widgets.h\> Header

The \<widgets.h\> header contains all the widgets that the user of this library should be calling.

### Datatypes

### Variables

### Constants

### Structs

    typedef struct ButtonT {
        int px;
        int py;
        int width;
        int height;
        wchar_t *text;
        void (*draw)();
        void (*callback)();
    } sButton, *psButton;

### Functions

#### Synopsis
    
    sButton * tui_button(const int px, const int py, const int width, const int height, wchar_t *text);

#### Description

    tui_button() creates a ButtonT struct and returns a pointer to it. (In the future it will also be added
    to an array/list of drawn objects/structs and to an array/list of all objects/structs.)


---
