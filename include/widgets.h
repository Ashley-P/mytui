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

enum eState {
    NONE  = 1 << 0,
    HOVER = 1 << 1,
    PRESS = 1 << 2
};


/* sSize exists so I can return 2 variables from calculate_min_size() */
typedef struct tSize {
    int x;
    int y;
} sSize, sPos;  // Two names just for less confusion because the same struct is used for coords

typedef struct tFrame {
    int numch;
    struct tWidget  *children[MAX_CHILDREN];
    struct tWidget  *grid[MAX_GRID_COLS][MAX_GRID_ROWS];
    int cols_size[MAX_GRID_COLS];
    int rows_size[MAX_GRID_ROWS];
} sFrame, *pFrame;

typedef struct tButton {
    wchar_t *text;
    void (*callback)(); // Callback for the button
} sButton, *pButton;

typedef struct tWidget {
    enum eType type; 
    enum eState state;
    sPos  pos;
    sSize size;
    int rowspan;
    int colspan;
    struct tWidget *parent;
    union {
        struct tButton button;
        struct tFrame  frame;
    } widget;
} sWidget, *pWidget;

sWidget * tui_frame(sWidget *parent);

sWidget * tui_button(sWidget *parent, wchar_t *text, void (*callback)());

void widget_sizer(sWidget *a);

void widget_positioner(sWidget *a);

sSize add_sSize(sSize a, sSize b);

sSize max_sSize(sSize a, sSize b);

void assign_to_parent(sWidget *child, sWidget *parent);

void grid_set(sWidget *widget, int col, int row);

#endif
