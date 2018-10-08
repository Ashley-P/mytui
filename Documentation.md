# Mytui Documentation

## Description

mytui is an effort by the author to create a text based user interface using the windows console.

## License

There isn't one yet

---
## The \<const.h\> Header

The \<const.h\> header file contains all the declarations for the functions and datatypes

### Variables


    extern int sn_screenwidth;
    extern int sn_screenheight;
    extern int i_bufsize;
    extern HANDLE h_stdin;
    extern CHAR_INFO **tui_current_screen;

    extern struct tWidget *w_root;
--
## The \<tui.h\> Header

The \<tui.h\> header file contains all the declarations for the functions and datatypes

### Functions

    int tui_init(const int n_screenwidth, const int n_screenheight);
    void find_widget(sStack *stack, sWidget *a, const int x, const int y);
    void inpthr_loop();
    void tui_loop();

---
## The \<tui.c\> Header

The \<tui.c\> header file contains all the declarations for the functions and datatypes

### Variables

The variables here are for internal use by the library

    HANDLE h_console                handle for the console
    CHAR_INFO *ci_screen            CHAR_INFO array that gets passed to the console
    HANDLE h_inpthr                 handle for the thread that reads the input buffer
    HANDLE h_stdin
    CHAR_INFO **tui_current_screen  A pointer to the current screen being used
    int sn_screenwidth              global screenx
    int sn_screenheight             global screeny
    int i_bufsize                   size of *wc_screen in elements
    sWidget *w_root                 root widget struct
    COORD c_screensize              initialises the buffer size
    SMALL_RECT sr_screensize        initialises the window size


### Functions

#### Synopsis

    CHAR_INFO * alloc_ci_array(const int n_screenwidth, const int n_screenheight);
    int init_tui(const int n_screenwidth, const int n_screenheight);
    void find_widget(sStack *stack, sWidget *a, int x, int y);
    void tui_draw_helper(sWidget *a);
    void tui_draw(sWidget *a);
    void inpthr_loop();
    void tui_loop();
    

#### Description

    alloc_ci_array() returns a pointer to an array of CHAR_INFO structs.

    init_tui() creates a new console process and a screen buffer for it. stderr
    is redirected to an output file and an array is created to fit the desired
    screen size. Then the screen buffer is linked to the console and the screen
    size is changed.

    find_widget() finds a widget given (Ideally) the top level widget and an x and y coordinate.

    tui_draw_helper() is a helper for tui_draw. It's purpose is to be called recursively
    over the tree created by the user.

    tui_draw() is where all the drawing to the buffer is handled.

    inpthr_loop() is the looping for h_inpthr it just calls tui_handle_input.

    tui_loop() is the looping for the program, drawing and other functions are
    called sequentially.

---
## The \<utils.h\> Header

The \<utils.h\> header file contains declarations for some utility functions such as error handling

### Functions

    void init_stderr();
    void win_err(const char *msg);
    void tui_err(const int err_type, const int quit_prog, const char *msg, ...);
    wchar_t * reverse_eType(enum eType type);
    wchar_t *rand_str();
    sStack *create_stack(unsigned int capacity);
    int is_stack_full(sStack *stack);
    int is_stack_empty(sStack *stack);
    void stack_push(sStack *stack, sWidget *a);
    sWidget * stack_pop(sStack *stack);

---
## The \<utils.c\> Source

The \<utils.c\> source file contains declarations for some utility functions such as error handling

### Constants/Defines
    
    TUI_ERROR   1
    TUI_WARNING 2
    TUI_OTHER   3

### Structs

    typedef struct tStack {
        unsigned int capacity;
        int top;
        sWidget **arr;
    } sStack;

### Variables

    HANDLE errlog               // Handle for the error file
    DWORD dw_bytes_written      // Required for the WriteFile function

### Functions

#### Synopsis
    
    void init_stderr();
    void win_err(const char *msg);
    void tui_err(const char *msg, const int err_type, const int quit_prog, ...);
    wchar_t * reverse_eType(enum eType type);
    int rand_int(int min, int max);
    wchar_t *rand_str();
    sStack *create_stack(unsigned int capacity);
    int is_stack_full(sStack *stack);
    int is_stack_empty(sStack *stack);
    void stack_push(sStack, sWidget *a);
    sSwidget *stack_pop(sStack *stack);

#### Description

    init_stderr() creates a new file to log errors in. If the file already
    exists, it's overwritten.

    win_err() puts a formatted error message from the \<windows.h\> functions into a
    logging file.

    tui_err() is similar to win_err() in that it puts a formatted error/warning/other message into
    a logging file, and also offers the opportunity to quit the program all together.

    reverse_eType() checks the input and returns a wchar_t string corresponding to the type.
    Used with tui_err to provide more descriptive errors.

    rand_int() creates a non-biased int utilizing the rand() function.
    
    rand_str() creates a random string of lower case alphabet characters with a random length
    between 1 and 15 characters.

    create_stack() creates a stack for use in searching the widget tree.

    is_stack_full() checks if the stack is full.

    is_stack_empty() checks if the stack is empty.
    
    stack_push appends a pointer to a widget to the top of the array.

    stack_pop returns the pointer at the top of the list.

---
## The \<draw.h\> Header

The \<draw.h\> header file contains all the function declarations that are exposed to other files

### Constants/Defines

    SHOW_MARGIN 0 // Debug Variable 
    DRAW_MARGIN draw_box(a->pos.x, a->pos.y, a->rsize.x, a->rsize.y, 0, 0x00);

    H HORIZONTAL
    V VERTICAL
    HORIZONTAL 1
    VERTICAL 2

### Functions

    void reset_buf();
    void draw_frame(const sWidget *a, const bool fill);
    void draw_button(const sWidget *a);
    void draw_label(const sWidget *a);
    void draw_checkbox(const sWidget *a);
    void draw_radiobutton(const sWidget *a);

---
## The \<draw.c\> Source

The \<draw.c\> source file contains all the function implementations

### Functions

#### Synopsis

    void reset_buf();
    void draw_line(const int x, const int y, const int len, const int direction, const int colour);
    void draw_box(int x, int y, const int width, const int height, const bool fill, int colour);
    void draw_str(const wchar_t *str, const size_t len, const size_t str_len, int x, int y);
    void draw_border_padding_content(sWidget *a, int colour);
    void draw_frame(sWidget *a, const int fill);
    void draw_button(const sWidget *a);
    void draw_label(const sWidget *a);
    void draw_checkbox(const sWidget *a);
    void draw_radiobutton(const sWidget *a);

#### Description
    
    reset_buf() Fills the buffer with spaces.
    Used after every draw to the screen.

    draw_line() Draws a non-diagonal line

    draw_box() Draws a box of the desired x and y at the provided co-ordinates x and y.
    The character used is the hash '#'. The box can be filled or just left with the borders.

    draw_str() Draws a string to the screen at the desired position.

    draw_border_padding_content() Fills out the colour for the border, padding and content for the supplied widget.

    draw_frame() Draws a frame to the screen at the desired location. Also reads the state
    of the frame and changes the colour accordingly.

    draw_button() Draws a button to the screen at the desired position. Also reads the state
    of the button and changes the colour accordingly.

    draw_label() Draws a label to the screen at the desired position.

    draw_checkbox() Draws a checkbox widget to the screen at the desired position. Anchoring of
    the label decides whether the label is on the left or right.

    draw_radiobutton() Draws a radiobutton widget to the screen at the desired position. Anchoring of
    the label decides whether the label is on the left or right.

---
## The \<widgets.h\> Header

The \<widgets.h\> header file contains all the struct definitions and functions that are exposed to other files

### Functions

    sWidget * tui_frame(sWidget *parent, wchar_t *text);
    sWidget * tui_button(sWidget *parent, wchar_t *text, void (*callback)());
    sWidget * tui_label(sWidget *parent, wchar_t *text);
    sWidget * tui_checkbox(sWidget *parent, wchar_t *text);
    sWidget * tui_radiobutton(sWidget *parent, wchar_t *text);
    sRadiobuttonLink * tui_radiobutton_link();
    void tui_root_frame();
    void widget_sizer(sWidget *a);
    void widget_span_sizer(sWidget *a);
    void widget_positioner(sWidget *a);
    void redraw_widgets(sWidget *a);
    sSize add_sSize(const sSize a, const sSize b);
    void checkbox_add(sWidget *a, sWidget *b);
    void radiobutton_link(sRadiobuttonLink *link, int count, ...); 
    void grid_set(sWidget *widget, const int col, const int row);

### Constants/Defines

    MAX_CHILDREN    16
    MAX_GRID_COLS   16
    MAX_GRID_ROWS   16

    NW   N | W
    NE   N | E
    SW   S | W
    SE   S | E
    NS   N | S
    EW   E | W
    NSE  N | S | E
    NSW  N | S | W
    SEW  S | E | W
    NEW  N | E | W
    NSEW N | S | E | W

    C CENTRE
    N NORTH
    S SOUTH
    E EAST
    W WEST
    
### Enums

    enum eType {
        FRAME       = 1,
        BUTTON      = 2,
        LABEL       = 3,
        CHECKBOX    = 4,
        RADIOBUTTON = 5
    };

    enum eState {
        NONE     = 0,
        DISABLED = 1 << 0,
        HOVER    = 1 << 1,
        PRESS    = 1 << 2
    };

    enum eAnchor {
        NORTH = 1 << 0,
        SOUTH = 1 << 1,
        EAST  = 1 << 2,
        WEST  = 1 << 3
    };

### Structs

    typedef struct tSize {
        int x;
        int y;
    } sSize, sPos;

    typedef struct tLabel {
        wchar_t *text;
        size_t  len;
        enum eAnchor anchor;
    } sLabel;

    typedef struct tFrame {
        size_t len;
        struct tLabel label;
        struct tWidget *children[MAX_CHILDREN];
        struct tWidget *grid[MAX_GRID_x][MAX_GRID_y];
        int *cols_size[MAX_GRID_COLS];
        int *rows_size[MAX_GRID_ROWS];
    } sFrame, *pFrame;

    typedef struct tButton {
        struct tLabel label;
        void (*callback)();
    } sButton, *pButton;

    typedef struct tCheckbox {
        struct tLabel label;
        struct tWidget *children[MAX_CHILDREN];
        struct tWidget *parent;
        size_t len;
        unsigned char active;
    } sCheckbox;

    typedef struct tRadiobuttonLink {
        struct tRadiobuttonNode *children[MAX_CHILDREN];
        size_t len;
        unsigned char active;
        sRadiobutton *old;
    } sRadiobuttonLink;

    typedef struct tRadiobutton {
        struct tLabel label;
        unsigned char active;
        struct tRadiobuttonLink *parent;
    } sRadiobutton;

    typedef struct tWidget {
        enum eType   type; 
        enum eState  state;
        enum eAnchor anchor;
        sPos  pos;
        sPos  cpos;
        sPos  gridpos;
        sSize rsize;
        sSize usize;
        sSize msize;
        sSize bsize;
        sSize psize;
        sSize csize;
        int rowspan;
        int colspan;
        struct tWidget *parent;
        union {
            struct tButton      button;
            struct tFrame       frame;
            struct tLabel       label;
            struct tCheckbox    cbox;
            struct tRadiobutton rbutton;
        } widget;
    } sWidget, *pWidget;

---
## The \<widgets.c\> Source

The \<widgets.c\> source contains all the functions including internal ones for use in the file

### Functions

#### Synopsis
    
    void assign_to_parent(sWidget *child, sWidget *parent);
    sWidget * init_sWidget(sWidget *parent);
    sWidget * tui_frame(sWidget *parent, wchar_t *text);
    void tui_root_frame();
    sWidget * tui_button(sWidget *parent, wchar_t *text, void (*callback)());
    sWidget * tui_label(sWidget *parent, wchar_t *text);
    sWidget * tui_checkbox(sWidget *parent, wchar_t *text);
    sWidget * tui_radiobutton(sWidget *parent, wchar_t *text);
    sRadiobuttonLink * tui_radiobutton_link();
    void calc_rsize(sWidget *a);
    void tui_root_frame();
    void widget_sizer(sWidget *a);
    void widget_span_sizer(sWidget *a);
    void widget_anchorer_helper(sWidget *a, int posdx, int posdy);
    void widget_anchorer(sWidget *a);
    void widget_positioner(sWidget *a)
    void redraw_widgets(sWidget *a);
    sSize add_sSize(const sSize a, const sSize b);
    sSize max_sSize(const sSize a, const sSize b);
    void checkbox_add(sWidget *a, sWidget *b);
    void radiobutton_link(sRadiobuttonLink *link, int count, ...); 
    void grid_set(sWidget *widget, int col, int row);

#### Description

    assign_to_parent() assigns the first argument (the child) to the second arguments child list.
    It has a check to see if the second widget is of the correct type.

    init_sWidget() initializes the basic widget struct. Usually called by the specific constructors 
    to facilitate easier struct creation.

    tui_frame() creates an sWidget struct with the internal type of FRAME and returns a pointer to it.
    
    tui_root_frame() creates the top level widget for the window, which has some special attributes.

    tui_button() creates an sWidget struct with the internal type of BUTTON and returns a pointer to it.

    tui_label() creates an sWidget struct with the internal type of LABEL and returns a pointer to it.

    tui_checkbox() creates an sWidget struct with the internal type of CHECKBOX and returns a pointer to it.

    tui_radiobutton() creates an sWidget struct with the internal type of RADIOBUTTON and returns a pointer to it.

    tui_radiobutton_link() creates an sRadiobuttonLink struct.

    calc_rsize() calculates rsize based on the other size structs. Called in widget_sizer to prevent copy pasting.

    widget_sizer() fills out the sSize struct in frames and other widgets that can have children

    widget_span_sizer() goes through the tree similar to widget_sizer but checks the colspan and rowspan
    of widgets and correctly sizes them.

    widget_anchorer_helper() propagates the changes in a->pos to it's children if there are any.

    widget_anchorer() Refines the position and size of the widget according to it's anchor variable

    widget_positioner() sets the coords for each widget which tui_draw uses
    
    redraw_widgets() Calls widget_sizer(), widget_span_sizer() and widget_positioner() in order.

    add_sSize() adds to sSize structs together and returns the resulting struct

    max_sSize() compares two sSize structs and returns a struct with the largest of each variables
    
    checkbox_add() adds the second checkbox to the first ones children array and sets the first checkbox
    as the second checkbox's parent.

    radiobutton_link() links the radio buttons together with the provide sRadiobutton struct.

    grid_set() enters the position of the widget supplied into it's parent's grid array. Sends a warning if
    it overwrites something (aka the element is not NULL)

---
## The \<input.h\> Header

The \<input.h\> header file contains all the declarations for the functions and datatypes

### Functions

    void tui_handle_input();
    int check_disable(sWidget *a);

---
## The \<input.c\> Source

The \<input.c\> source file contains all the definitions for the functions and datatypes

### Functions

#### Synopsis

    void mouse_hover(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *ev) {
    int check_disable(sWidget *a);
    void cbox_set_active(sWidget *a, int active);
    void cbox_check_parent_active(sWidget *a) {
    void frame_mouse_event(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *ev);
    void button_mouse_event(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *ev);
    void cbox_mouse_event(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *ev) {
    void rbutton_mouse_event(sWidget *a, sWidget **old, const MOUSE_EVENT_RECORD *ev) {
    void tui_handle_input();

#### Description

    mouse_hover() exists for some code reuse in the *_mouse_event functions.

    check_disable() is for some code reuse to propagate the effects of the DISABLED
    state without actually applying it to the widget.

    cbox_set_active() traverses the tree of checkboxes and sets them all to
    the same active variable.

    cbox_check_parent_active() goes up the tree from the child and checks if the
    parent active type needs to be set.

    frame_mouse_event() handles the mouse events when the widget is of type FRAME.

    button_mouse_event() handles the mouse events when the widget is of type BUTTON.

    cbox_mouse_event() handles the mouse events when the widget is of type CHECKBOX.

    rbutton_mouse_event() handles the mouse events when the widget is of type RADIOBUTTON.

    tui_handle_input() handles all the events for the program.
--- 
