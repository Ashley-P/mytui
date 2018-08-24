#ifndef WIDGETS_H_
#define WIDGETS_H_

#include <stdio.h>

#define MAX_CHILDREN    16
#define MAX_GRID_COLS   16
#define MAX_GRID_ROWS   16


enum eType {
    FRAME  = 1 << 0,
    BUTTON = 1 << 1
};

/* sSize exists so I can return 2 variables from calculate_min_size() */
typedef struct tSize {
    int x;
    int y;
} sSize, sPos;  // Two names just for less confusion because the same struct is used for coords

typedef struct tFrame {
    void (*draw)();     // Drawing function
    int numch;
    struct tWidget  *children[MAX_CHILDREN];
    struct tWidget  *grid[MAX_GRID_COLS][MAX_GRID_ROWS];
    int cols_size[MAX_GRID_COLS];
    int rows_size[MAX_GRID_ROWS];
} sFrame, *pFrame;

typedef struct tButton {
    wchar_t *text;
    void (*draw)();     // Drawing function
    void (*callback)(); // Callback for the button
} sButton, *pButton;

typedef struct tWidget {
    enum eType type; 
    sPos  pos;
    sSize size;
    struct tWidget *parent;
    union {
        struct tButton button;
        struct tFrame  frame;
    } widget;
} sWidget, *pWidget;

sWidget * tui_frame(sWidget *parent);

sWidget * tui_button(sWidget *parent, wchar_t *text, void (*callback)());

void widget_sizer(sWidget *a);

sSize add_sSize(sSize a, sSize b);

sSize max_sSize(sSize a, sSize b);

int parent_widget_type(sWidget *widget);

void grid_set(sWidget *widget, int col, int row);

#endif
