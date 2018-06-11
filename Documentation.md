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

    HANDLE h_console            handle for the console
    HANDLE h_stderr             handle for stderr which is redirected to a file
    wchar_t *wc_screen          wide character array that gets written to the console
    DWORD dw_bytes_written      required variable for usage with windows
    COORD c_screensize          initialises the buffer size
    SMALL_RECT y                initialises the window size
    int i_bufsize               size of *wc_screen in elements


### Constants

### Structs

### Functions

#### Synopsis

    int init_tui(const int nScreenWidth, const int nScreenHeight);
    wchar_t* alloc_wc_array(const int nScreenWidth, const int nScreenHeight);
    void tui_handle_input();
    void tui_draw();
    void tui_loop();
    

#### Description

    init_tui() creates a new console process and a screen buffer for it. stderr
    is redirected to an output file and an array is created to fit the desired
    screen size. Then the screen buffer is linked to the console and the screen
    size is changed.

    alloc_wc_array() returns a pointer to a chunk of memory that has been
    acquired through calloc.

    tui_handle_input() handles all the events for the program

    tui_draw() is where all the drawing to the buffer is handled

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

    void reset_buf(wchar_t *buf);
    void draw_box(wchar_t *buf, int x, int y, const int width, const int height, const bool fill);
    void draw_str(wchar_t *buf, const wchar_t *str, const size_t str_len, int x, int y);


#### Description
    
    reset_buf() Fills the buffer with spaces and sets the last character to the null character.
    Used after every draw to the screen

    draw_box() Draws a box of the desired width and height at the provided co-ordinates x and y.
    The character used is the hash '#'. The box can be filled or just left with the borders

    draw_str() Draws a string to the screen at the desired position.


## The \<widgets.h\> Header

The \<widgets.h\> header contains all the widgets that the user of this library should be calling.

### Datatypes

### Variables

### Constants

### Structs

    typedef struct Button_ {
        int px;
        int py;
        int width;
        int height;
        char *text;
    } sButton, *psButton;

### Functions

#### Synopsis
    
    psButton * tui_button(const int px, const int py, const int width, const int height, const char *text);

#### Description

    tui_button() creates a Button_ struct and returns a pointer to it. (In the future it will also be added
    to an array/list of drawn objects/structs and to an array/list of all objects/structs.)


---
