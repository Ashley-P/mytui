#ifndef UTILS_H_
#define UTILS_H_

#include "widgets.h"

#define TUI_ERROR 1
#define TUI_WARNING 2
#define TUI_OTHER 3

typedef struct tStack {
    unsigned int capacity;
    int top;
    sWidget **arr;
} sStack;

void init_stderr();
void win_err(const char *msg);
void tui_err(const int err_type, const int quit_prog, const char *msg, ...);
wchar_t * reverse_eType(enum eType type);
void init_time();
wchar_t *rand_str();
sStack *create_stack(unsigned int capacity);
int is_stack_full(sStack *stack);
int is_stack_empty(sStack *stack);
void stack_push(sStack *stack, sWidget *a);
sWidget * stack_pop(sStack *stack);
void word_wrap(wchar_t **text, const size_t len, int *xsize, int *ysize);

#endif
