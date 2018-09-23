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
#define NW   N | W
#define NE   N | E
#define SW   S | W
#define SE   S | E
#define NS   N | S
#define EW   E | W
#define NSE  N | S | E
#define NSW  N | S | W
#define SEW  S | E | W
#define NEW  N | E | W
#define NSEW N | S | E | W

#define C CENTRE
#define N NORTH
#define S SOUTH
#define E EAST
#define W WEST

/* Might not use bitwise operations on the widget types so just keeping them sequential */
enum eType {
    FRAME     = /*1 <<*/ 1,
    BUTTON    = /*1 <<*/ 2,
    LABEL     = /*1 <<*/ 3,
    CHECKBOX  = /*1 <<*/ 4
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

typedef struct tSize {
    int x;
    int y;
} sSize, sPos;  // Two names just for less confusion because the same struct is used for coords

typedef struct tLabel {
    wchar_t *text;
    size_t  len;
    enum eAnchor anchor; /* This anchor is used when tLabel is used inside another widget */
} sLabel;

typedef struct tFrame {
    int numch;
    struct tLabel label;
    struct tWidget  *children[MAX_CHILDREN];
    struct tWidget  *grid[MAX_GRID_COLS][MAX_GRID_ROWS];
    int cols_size[MAX_GRID_COLS];
    int rows_size[MAX_GRID_ROWS];
} sFrame, *pFrame;

typedef struct tButton {
    struct tLabel label;
    void (*callback)(); // Callback for the button
} sButton, *pButton;

typedef struct tCheckbox {
    struct tLabel label;
    unsigned char active;
} sCheckbox;

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
        struct tButton   button;
        struct tFrame    frame;
        struct tLabel    label;
        struct tCheckbox cbox;
    } widget;
} sWidget, *pWidget;

sWidget * tui_frame(sWidget *parent, wchar_t *text);
sWidget * tui_button(sWidget *parent, wchar_t *text, void (*callback)());
sWidget * tui_label(sWidget *parent, wchar_t *text);
sWidget * tui_checkbox(sWidget *parent, wchar_t *text);
void tui_root_frame();
void widget_sizer(sWidget *a);
void widget_span_sizer(sWidget *a);
void widget_positioner(sWidget *a);
void redraw_widgets(sWidget *a);
sSize add_sSize(const sSize a, const sSize b);
void grid_set(sWidget *widget, const int col, const int row);

#endif
