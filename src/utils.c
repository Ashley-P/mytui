#include <stdio.h>
#include <windows.h>
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

void tui_err(const char *msg, const int quit_prog) {
    char err[256];
    memset(err, '\0', 256);
    sprintf(err, "ERROR: %s\r\n", msg);
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



