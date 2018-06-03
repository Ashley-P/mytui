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


### Constants

### Structs

### Functions

#### Synopsis

    int inittui(const int nScreenWidth, const int nScreenHeight);
    wchar_t* allocwcarray(const int nScreenWidth, const int nScreenHeight);
    

#### Description

    inittui() creates a new console process and a screen buffer for it. stderr
    is redirected to an output file and an array is created to fit the desired
    screen size. Then the screen buffer is linked to the console and the screen
    size is changed.

    allocwcarray() returns a pointer to a chunk of memory that has been
    acquired through calloc.


---
## The \<utils.h\> Header

The \<utils.h\> header file contains declarations for some utility functions such as error handling

### Datatypes

### Variables

### Constants

### Structs

### Functions

#### Synopsis
    
    int winerr(const char *msg);


#### Description

    winerr() puts a formatted error message from the windows functions into a file
