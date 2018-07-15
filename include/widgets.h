#ifndef WIDGETS_H_
#define WIDGETS_H_


typedef struct tWidget {
    int type;
    struct tWidget *master;
    union {
        struct tButton *button;
        struct tFrame  *frame;
    } widget;
} sWidget;

typedef struct tFrame {
    int px;
    int py;
    int width;
    int height;
    sWidget *children[16];
} sFrame;

typedef struct tButton {
    int px;
    int py;
    int width;
    int height;
    wchar_t *text;
    void (*draw)();     // Drawing function
    void (*callback)(); // Callback for the button
} sButton;


sButton * tui_button(const int px, const int py, const int width, const int height, wchar_t *text, void (*fp)());

sFrame * tui_frame(const sFrame *master);

#endif
