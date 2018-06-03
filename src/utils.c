#include "utils.h"


int winerr(const char *msg) {
    char buf[256];
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL,
                  GetLastError(),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  buf,
                  256,
                  NULL);
    // This is where it goes to a file
    return 0;
}
