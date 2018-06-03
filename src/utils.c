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


int winerr(const char *msg) {
    char buf[256];
    char err[256];
    char *err2;
    memset(buf, '\0', 256);
    memset(err, '\0', 256);

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL,
                  GetLastError(),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  buf,
                  256,
                  NULL);
    // This is where it goes to a file
    sprintf(err, "ERROR: %s.\t%s\r\n", msg, buf);
    err2 = (char *)malloc(strlen(err));
    strcpy(err2, err);
    WriteFile(errlog,
              err2,
              strlen(err),
              &dw_bytes_written,
              NULL);
              
    return 0;
}
