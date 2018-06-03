#include "tui.h"
/*
 * All the includes are in the header so the user doesn't have to include 
 * everything themselves
 */

// Variables : Everything is static because it shouldn't be used outside this file (could change)
static HANDLE h_console; // Handle for the console
static HANDLE h_stderr; //TODO redirect to a file
static wchar_t *wc_screen; // Buffer to write characters to
static DWORD dw_bytes_written = 0; // Required by windows.h


// Needed for initialisation
static COORD c_screensize;
static SMALL_RECT y;


int inittui(const int n_screenwidth, const int n_screenheight) {
    FreeConsole();
    AllocConsole();

    h_console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
                                                0,
                                                NULL,
                                                CONSOLE_TEXTMODE_BUFFER,
                                                NULL);
    
    h_stderr = GetStdHandle(STD_ERROR_HANDLE);
    wc_screen = allocwcarray(n_screenwidth, n_screenheight);
    
    /*
     * Slightly hacky way of setting up the console.
     * Sets the console window size to be very small, then scales it back
     * up to the size of the buffer
     */
    c_screensize = (COORD) {(short)n_screenwidth, (short)n_screenheight};
    y = (SMALL_RECT) {0, 0, (short)n_screenwidth - 1, (short)n_screenheight - 1};

    SetConsoleWindowInfo(h_console, TRUE, &((SMALL_RECT) {0, 0, 1, 1}));
    SetConsoleScreenBufferSize(h_console, c_screensize);
    SetConsoleActiveScreenBuffer(h_console);
    SetConsoleWindowInfo(h_console, TRUE, &y);
    SetConsoleMode(h_console, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

    // Just a test
    while(1) {
        *wc_screen = L'A';
        *(wc_screen + sizeof(wchar_t)) = L'B';
        *(wc_screen + (n_screenwidth * n_screenheight) - 1) = L'0';
        *(wc_screen + (n_screenwidth * n_screenheight)) = L'\0';
        WriteConsoleOutputCharacterW(h_console,
                                     wc_screen,
                                     n_screenwidth * n_screenheight,
                                     (COORD) {0, 0},
                                     &dw_bytes_written);

    }

    return 0;
}

wchar_t * allocwcarray(const int n_screenwidth, const int n_screenheight) {
    wchar_t *ptr = (wchar_t *)calloc(n_screenwidth * n_screenheight, sizeof(wchar_t));

    // TODO: replace with better error handling
    if (ptr == NULL) {
        printf("Calloc error");
        exit(-1);
    }
    return ptr;
}
