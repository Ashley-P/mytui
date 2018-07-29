#ifndef WIDGETS_H_
#define WIDGETS_H_

enum {
    FRAME  = 1,
    BUTTON = 2
} eType;

typedef struct tWidget {
    enum eType type; 
    int px;
    int py;
    int min_width;
    int min_height;
    struct tWidget *parent;
    union {
        struct tButton *button;
        struct tFrame  *frame;
    } widget;
} sWidget;

typedef struct tFrame {
    struct tWidget *children[16];
    void (*draw)();     // Drawing function
} sFrame;

typedef struct tButton {
    wchar_t *text;
    void (*draw)();     // Drawing function
    void (*callback)(); // Callback for the button
} sButton;


sFrame * tui_frame(sFrame *parent);

sButton * tui_button(sFrame *parent, wchar_t *text, void (*callback)());

#endif
