#include "tui.h"
/* All the includes are in the header so the user doesn't have to include 
 * everything themselves
 */


void initTui(){}

HANDLE createRoot (const int nScreenWidth, const int nScreenHeight) {
    /*HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, // Desired access
                                                0,                            // Share mode
                                                NULL,                         // Security attributes
                                                CONSOLE_TEXTMODE_BUFFER,
                                                NULL);
    
    // TODO: replace with better error handling
    if (!hConsole)
        fprintf(stderr, "Create Root Error"); 

    // Change the internal buffer size
    //SetConsoleActiveScreenBuffer(hConsole);

    return hConsole;*/
}

wchar_t * allocwArray(const int nScreenWidth, const int nScreenHeight) {
    wchar_t *ptr = (wchar_t *)calloc(nScreenWidth * nScreenHeight, sizeof(wchar_t));
    if (ptr == NULL) {
        printf("failed");
        exit(-1);
    }
    return ptr;
}
/*
wchar_t * allocwArray(const int nScreenWidth, const int nScreenHeight) {
    return (wchar_t *)calloc(nScreenWidth * nScreenHeight, sizeof(wchar_t));
}
*/
