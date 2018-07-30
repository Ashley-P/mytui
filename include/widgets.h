#ifndef WIDGETS_H_
#define WIDGETS_H_

#define MAX_CHILDREN 16
enum eType {
    FRAME  = 1,
    BUTTON = 2
};

/* sMinsize exists so I can return 2 variables from calculate_min_size() */
typedef struct tMinSize {
    int width;
    int height;
} sMinSize;

typedef struct tWidget {
    enum eType type; 
    int px;
    int py;
    struct tMinSize;
    struct tWidget *parent;
    union {
        struct tButton *button;
        struct tFrame  *frame;
    } widget;
} sWidget;

typedef struct tFrame {
    struct sWidget *children[MAX_CHILDREN];
    void (*draw)();     // Drawing function
} sFrame;

typedef struct tButton {
    wchar_t *text;
    void (*draw)();     // Drawing function
    void (*callback)(); // Callback for the button
} sButton;


sFrame * tui_frame(sWidget *parent);

sButton * tui_button(sWidget *parent, wchar_t *text, void (*callback)());

void calculate_min_size(sWidget *widget);

#endif
