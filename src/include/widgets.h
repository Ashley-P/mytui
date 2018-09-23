#ifndef WIDGETS_H_
#define WIDGETS_H_

#include <stdio.h>
#include <stddef.h>
#include "const.h"

/* For the grid system */
#define MAX_CHILDREN    16
#define MAX_GRID_COLS   16
#define MAX_GRID_ROWS   16

/* For eAnchor */
#define C CENTRE
#define N NORTH
#define S SOUTH
#define E EAST
#define W WEST

enum eType {
    FRAME  = 1 << 0,
    BUTTON = 1 << 1
};

enum eState {
    NONE     = 0,
    DISABLED = 1 << 0,
    HOVER    = 1 << 1,
    PRESS    = 1 << 2
};

enum eAnchor {
    CENTRE = 0,
    NORTH  = 1 << 0,
    SOUTH  = 1 << 1,
    EAST   = 1 << 2,
    WEST   = 1 << 3
};

typedef struct tLabel {
    wchar_t *text;
    size_t  len;
    enum eAnchor anchor;
} sLabel;

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
    sLabel label;
    void (*callback)(); // Callback for the button
} sButton, *pButton;

typedef struct tWidget {
    enum eType   type; 
    enum eState  state;
    enum eAnchor anchor;
    sPos  pos;
    sPos  gridpos;
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
void tui_root_frame();
void widget_sizer(sWidget *a);
void widget_span_sizer(sWidget *a);
void widget_positioner(sWidget *a);
void redraw_widgets(sWidget *a);
sSize add_sSize(const sSize a, const sSize b);
void grid_set(sWidget *widget, const int col, const int row);

#endif
