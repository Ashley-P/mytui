#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"


static HANDLE errlog;
static DWORD dw_bytes_written;


void init_stderr() {
    errlog = CreateFile("error.log",
                        GENERIC_WRITE,
                        0,
                        NULL,
                        CREATE_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);
}


void win_err(const char *msg) {
    char buf[256];
    char err[256];
    memset(buf, '\0', 256);
    memset(err, '\0', 256);

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL,
                  GetLastError(),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  buf,
                  256,
                  NULL);
    /*
     * This is where it goes to a file
     * It has to be done like this to prevent 
     * artifacts being left in the error file
     */
    sprintf(err, "ERROR: %s.\t%s\r\n", msg, buf);
    char err2[strlen(err) + 1];
    err2[strlen(err) + 1] = '\0';
    strcpy(err2, err);
    WriteFile(errlog,
              err2,
              sizeof(err2) - 1,
              &dw_bytes_written,
              NULL);

    free(err2);
}

void tui_err(const char *msg, const int err_type, const int quit_prog) {
    char err[256];
    memset(err, '\0', 256);
    switch (err_type) {
        case TUI_ERROR:
            sprintf(err, "ERROR: %s\r\n", msg);
            break;
        case TUI_WARNING:
            sprintf(err, "WARNING: %s\r\n", msg);
            break;
        case TUI_OTHER:
            sprintf(err, "OTHER: %s\r\n", msg);
            break;
    }
    char err2[strlen(err) + 1];
    err2[strlen(err) + 1] = '\0';
    strcpy(err2, err);
    WriteFile(errlog,
              err2,
              sizeof(err2) - 1,
              &dw_bytes_written,
              NULL);
    
    if (quit_prog == 1)
        exit(-1);
}

/* Generating random strings */
wchar_t *rand_str() {
    static int flag = 0;
    if (!flag) {
        srand(time(NULL));
        flag = 1;
    }
    wchar_t charset[] = L"abcdefghijklmnopqrstuvwxyz";
    int strsize = 2 + rand() % 15;
    wchar_t *ptr = (wchar_t *)malloc(sizeof(wchar_t) * strsize);
    for(int i = 0; i < strsize - 1; i++) {
        *(ptr + i) = charset[rand() % 25];
    }
    *(ptr + strsize - 1) = L'\0';

    return ptr;
}

