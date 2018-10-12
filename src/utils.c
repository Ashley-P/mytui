#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
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

void tui_err(const int err_type, const int quit_prog, const char *msg, ...) {
    char buf[256];
    char buf2[256];
    memset(buf2, '\0', 256);

    va_list arg;
    va_start(arg, msg);
    vsnprintf(buf, (size_t) 256, msg, arg);
    va_end(arg);

    switch (err_type) {
        case TUI_ERROR:
            sprintf(buf2, "ERROR: %s\r\n", buf);
            break;
        case TUI_WARNING:
            sprintf(buf2, "WARNING: %s\r\n", buf);
            break;
        case TUI_OTHER:
            sprintf(buf2, "OTHER: %s\r\n", buf);
            break;
        default:
            break;
    }

    /* Using strlen works because sizeof(char) is 1 */
    WriteFile(errlog,
              buf2,
              strlen(buf2),
              &dw_bytes_written,
              NULL);
}

wchar_t * reverse_eType(enum eType type) {
    switch (type) {
        case FRAME:       return L"FRAME";
        case BUTTON:      return L"BUTTON";
        case LABEL:       return L"LABEL";
        case CHECKBOX:    return L"CHECKBOX";
        case RADIOBUTTON: return L"RADIOBUTTON";
        case CANVAS:      return L"CANVAS";
        case FIELD:       return L"RADIOBUTTON";
        default:          return L"UNKNOWN";
    }
}

int rand_int(int min, int max) {
    /* Provides non modulus biased integers */
    int x;
    static int flag = 0;
    if (!flag) {
        srand(time(NULL));
        flag = 1;
    }
    do {
        x = 1 + rand() / ((RAND_MAX + 1u) / (max + 1));
    } while (x > max || x < min); 
    return x;
}

wchar_t *rand_str() {
    /* Generating random strings */
    int strsize = rand_int(6, 15);
    wchar_t *ptr = (wchar_t *)calloc(sizeof(wchar_t), strsize);
    for(int i = 0; i < strsize - 1; i++) {
        *(ptr + i) = L"abcdefghijklmnopqrstuvwxyz"[rand_int(0, 25)];
    }
    *(ptr + strsize - 1) = L'\0';

    return ptr;
}

sStack *create_stack(unsigned int capacity) {
    sStack *stack = (sStack *)malloc(sizeof(sStack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->arr = (sWidget **)calloc(stack->capacity, sizeof(sWidget *));
    return stack;
}

int is_stack_full(sStack *stack) {
    return (stack->top == stack->capacity - 1);
}

int is_stack_empty(sStack *stack) {
    return stack->top == -1;
}

void stack_push(sStack *stack, sWidget *a) {
    int temp = is_stack_full(stack);
    if (temp) {
        tui_err(TUI_WARNING, 0, "Error in function stack_push. Stack Full");
        return;
    }
    stack->arr[++stack->top] = a;
}

sWidget * stack_pop(sStack *stack) {
    if (is_stack_empty(stack)) {
        tui_err(TUI_WARNING, 0, "Error in function stack_pop. Stack Empty");
        return (sWidget *) -1;
    }
    return stack->arr[stack->top--];
} 
