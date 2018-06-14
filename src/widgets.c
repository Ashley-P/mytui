#include <stdlib.h>
#include "widgets.h"
#include "draw.h"

sButton * tui_button(const int px, const int py, const int width,
                     const int height, wchar_t *text, void (*fp)()) {
    psButton ptr = (psButton)malloc(sizeof(sButton));
    ptr->px = px;
    ptr->py = py;
    ptr->width = width;
    ptr->height = height;
    ptr->text = text;
    if (fp != NULL)
        ptr->fp = fp;


    return ptr;
}
