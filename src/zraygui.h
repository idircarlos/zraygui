#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "../include/raygui.h"

#define DA_INIT_CAP 10

#define da_append(da, item)                                                          \
    do {                                                                             \
        if ((da)->count >= (da)->capacity) {                                         \
            (da)->capacity = (da)->capacity == 0 ? DA_INIT_CAP : (da)->capacity*2;   \
            (da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items)); \
            assert((da)->items != NULL && "Buy more RAM lol");                   \
        }                                                                            \
                                                                                     \
        (da)->items[(da)->count++] = (item);                                         \
    } while (0)

#define ARRAY_SIZE(arr) sizeof(arr)/sizeof(char*)

typedef struct _component Component;    // Generic empty struct used to contain the pointer to the specific widget (button, label...)
typedef struct _layout_list LayoutList;
typedef struct _widget Widget;

typedef enum {
    L_NONE,
    L_HORZ,
    L_VERT
} LayoutType;

typedef enum {
    W_BUTTON = 0,
    W_LABEL,
    W_CHECKBOX,
    W_RADIO,
    W_COMBOBOX,
    W_GROUPBOX,
    W_SEPARATOR,
    W_TEXTBOX
} WidgetType;

typedef struct Layout {
    Widget *widget;
    Rectangle rect;
    LayoutType type;
    Color color;
    struct Layout *parent;
    LayoutList *childs;
} Layout;

struct _widget {
    Layout *parent;
    Rectangle rect;
    WidgetType type;
    Component *component;
    bool visible;
    void (*onClick)(Vector2 mousePos);
};

struct _layout_list {
    Layout **items;
    size_t count;
    size_t capacity;
};

typedef struct Button {
    char *text;
} Button;

typedef struct Label {
    char *text;
} Label;

typedef struct CheckBox {
    char *text;
    bool checked;
} CheckBox;

typedef struct Radio {
    char *text;
    bool checked;
} Radio;

typedef struct ComboBox {
    char **options;
    size_t size;
    int index;
    bool opened;
} ComboBox;

typedef struct GroupBox {
    char *title;
} GroupBox;

typedef struct Separator {
    // Empty
} Separator;

typedef struct TextBox {
    char text [1024];
    bool active;
} TextBox;

LayoutList *CreateLayoutList() {
    LayoutList *list = (LayoutList*)malloc(sizeof(LayoutList));
    list->items = (Layout*)malloc(DA_INIT_CAP*sizeof(Layout));
    list->capacity = DA_INIT_CAP;
    list->count = 0;
    return list;
}

Rectangle GetWidgetBounds(Widget *widget) {
    return widget->rect;
}

void SetWidgetBounds(Widget *widget, Rectangle rect) {
    widget->rect = rect;
}

int GetWidgetX(Widget *widget) {
    return widget->rect.x;
}

int GetWidgetY(Widget *widget) {
    return widget->rect.y;
}

int GetWidgetWidth(Widget *widget) {
    return widget->rect.width;
}

int GetWidgetHeight(Widget *widget) {
    return widget->rect.height;
}

Vector2 GetWidgetPos(Widget *widget) {
    return (Vector2){widget->rect.x, widget->rect.y};
}

void SetWidgetPos(Widget *widget, Vector2 pos) {
    widget->rect.x = pos.x;
    widget->rect.y = pos.y;
}

static void UpdateLayout(Layout *parent) {
    int offsetx = parent->rect.width  / parent->childs->count;
    int offsety = parent->rect.height / parent->childs->count;
    for (size_t i = 0; i < parent->childs->count; i++) {
        Layout *children = parent->childs->items[i];
        switch (parent->type) {
            case L_HORZ:
                children->rect.x = parent->rect.x + (i*offsetx);
                children->rect.y = parent->rect.y;
                children->rect.width = offsetx;
                children->rect.height = parent->rect.height;
                if (parent->widget) SetWidgetBounds(parent->widget, parent->rect);
                break;

            case L_VERT:
                children->rect.x = parent->rect.x;
                children->rect.y = parent->rect.y + (i*offsety);
                children->rect.width = parent->rect.width;
                children->rect.height = offsety;
                if (parent->widget) SetWidgetBounds(parent->widget, parent->rect);
                break;
            case L_NONE:
                break;
            default:
                assert(0 && "Unreachable");
        }
        UpdateLayout(children);
    }
}

static void AddToLayout(Layout *parent, Layout *children) {
    if (!parent) return;
    da_append(parent->childs, children);
    children->parent = parent;
    UpdateLayout(parent);
}

Layout *CreateLayout(Layout *parent, LayoutType type, Color c) {
    Rectangle rect = {0};
    if (!parent) {
        rect.x = 0;
        rect.y = 0;
        rect.width = GetScreenWidth();
        rect.height = GetScreenHeight();
    }
    Layout *layout = (Layout*)malloc(sizeof(Layout));
    layout->rect = rect;
    layout->type = type;
    layout->childs = CreateLayoutList();
    layout->color = c;
    layout->widget = NULL;
    layout->parent = NULL;
    AddToLayout(parent, layout);
    return layout;
}

void AddWidget(Layout *layout, Widget *widget) {
    Layout *l = CreateLayout(layout, L_NONE, WHITE);
    l->widget = widget;
    widget->rect = (Rectangle) {
        .x = widget->rect.x + layout->rect.x,
        .y = widget->rect.y + layout->rect.y,
        .width = widget->rect.width,
        .height = widget->rect.height
    };
    widget->parent = l;
}

static Widget *BuildWidget(WidgetType wtype, Component *component) {
    Widget *widget = (Widget*)malloc(sizeof(Widget));
    widget->parent = NULL;
    widget->type = wtype;
    widget->component = component;
    widget->rect = (Rectangle){0};
    widget->onClick = NULL;
    widget->visible = true;
    return widget;
}

Widget *CreateButton(char *text) {
    Button *button = (Button*)malloc(sizeof(Button));
    button->text = text;
    Widget *widget = BuildWidget(W_BUTTON, button);
    widget->rect = (Rectangle){0, 0, 75, 25};
    return widget;
}

Widget *CreateLabel(char *text) {
    Label *label = (Label*)malloc(sizeof(Label));
    label->text = malloc(1024*sizeof(char));
    strcpy(label->text, text);
    Widget *widget = BuildWidget(W_LABEL, label);
    widget->rect = (Rectangle){
        .x = 0,
        .y = 0,
        .width = GetTextWidth(text),
        .height = 20
    };
    int width = GetTextWidth(text);
    int size = MeasureText(text, (float)GuiGetStyle(DEFAULT, TEXT_SIZE));
    printf("%d, %d\n", width, size);
    
    widget->rect.width = size;

    return widget;
}

Widget *CreateCheckBox(char *text) {
    CheckBox *checkbox = (CheckBox*)malloc(sizeof(CheckBox));
    checkbox->checked = false;
    checkbox->text = text;
    Widget *widget = BuildWidget(W_CHECKBOX, checkbox);
    widget->rect = (Rectangle){0, 0, 20, 20};
    return widget;
}

Widget *CreateRadio(char *text) {
    Radio *radio = (Radio*)malloc(sizeof(Radio));
    radio->checked = false;
    radio->text = text;
    Widget *widget = BuildWidget(W_RADIO, radio);
    widget->rect = (Rectangle){0, 0, 15, 15};
    return widget;
}

Widget *CreateComboBox(size_t size, char **options) {
    ComboBox *combobox = (ComboBox*)malloc(sizeof(ComboBox));
    combobox->opened = false;
    combobox->index = 0;
    combobox->size = size;
    combobox->options = options;
    Widget *widget = BuildWidget(W_COMBOBOX, combobox);
    widget->rect = (Rectangle){0, 0, 100, 25};
    return widget;
}

Widget *CreateGroupBox(Rectangle rect, char *title) {
    GroupBox *groupbox = (GroupBox*)malloc(sizeof(GroupBox));
    groupbox->title = title;
    Widget *widget = BuildWidget(W_GROUPBOX, groupbox);
    widget->rect = rect;
    return widget;
}

Widget *CreateSeparator(int x, int y, int width) {
    Separator *separator = (Separator*)malloc(sizeof(Separator));
    Widget *widget = BuildWidget(W_SEPARATOR, separator);
    widget->rect = (Rectangle){x,y,width,0};
    return widget;
}

Widget *CreateTextBox(Rectangle rect) {
    TextBox *textbox = (TextBox*)malloc(sizeof(TextBox));
    textbox->active = false;
    memset(textbox->text, 0, ARRAY_SIZE(textbox->text));
    Widget *widget = BuildWidget(W_TEXTBOX, textbox);
    widget->rect = rect;
    return widget;
}

static void DrawTextCentered(char *text, int fontSize, Rectangle rect) {
    int size = MeasureText(text, fontSize);
    DrawText(text, rect.x + (rect.width/2) - (size/2),rect.y + rect.height/2, fontSize, BLACK);
}

static bool CheckMouse(Widget *widget) {
    if (!widget) return false;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), widget->rect)) {
        return true;
    }
    return false;
}

static void RenderButton(Widget *widget) {
    Button *button = (Button*)widget->component;
    GuiButton(widget->rect, button->text);
}

static void RenderLabel(Widget *widget) {
    Label *label = (Label*)widget->component;
    DrawText(label->text, widget->rect.x, widget->rect.y, GuiGetStyle(LABEL, TEXT_SIZE), GetColor(GuiGetStyle(LABEL, TEXT)));
}

static void RenderCheckbox(Widget *widget) {
    CheckBox *checkbox = (CheckBox*)widget->component;
    GuiCheckBox(widget->rect, checkbox->text, &checkbox->checked);
}

static void RenderRadio(Widget *widget) {
    Radio *radio = (Radio*)widget->component;
    Layout *parent = widget->parent->parent;
    GuiRadioButton(widget->rect, radio->text, &radio->checked);
    if (radio->checked) {
        for (size_t i = 0; i < parent->childs->count; i++) {
            Widget *w = parent->childs->items[i]->widget;
            if (w->type == W_RADIO && w != widget) {
                Radio *r = (Radio*)w->component;
                r->checked = false;
            }
        }
    }
}

static void RenderComboBox(Widget *widget) {
    ComboBox *combobox = (ComboBox*)widget->component;
    Layout *parent = widget->parent->parent;
    if(GuiDropdownBox(widget->rect, combobox->options, combobox->size, &combobox->index, combobox->opened)) combobox->opened = !combobox->opened;
}

static void RenderGroupBox(Widget *widget) {
    GroupBox *groupbox = (GroupBox*)widget->component;
    Layout *parent = widget->parent->parent;
    GuiGroupBox(widget->rect, groupbox->title);
}

static void RenderSeparator(Widget *widget) {
    Separator *separator = (Separator*)widget->component;
    Layout *parent = widget->parent->parent;
    GuiLine(widget->rect, NULL);
}

static void RenderTextBox(Widget *widget) {
    TextBox *textbox = (TextBox*)widget->component;
    Layout *parent = widget->parent->parent;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), widget->rect)) {
            textbox->active = true;
        }
        else {
            textbox->active = false;    
        }
    }
    GuiTextBox(widget->rect, textbox->text, ARRAY_SIZE(textbox->text), textbox->active);
}

static void RenderWidget(Widget *widget) {
    if (!widget || !widget->visible) return;
    switch (widget->type) {
        case W_BUTTON:
            RenderButton(widget);
            break;
        case W_LABEL:
            RenderLabel(widget);
            break;
        case W_CHECKBOX:
            RenderCheckbox(widget);
            break;
        case W_RADIO:
            RenderRadio(widget);
            break;
        case W_COMBOBOX:
            RenderComboBox(widget);
            break;
        case W_GROUPBOX:
            RenderGroupBox(widget);
            break;
        case W_SEPARATOR:
            RenderSeparator(widget);
            break;
        case W_TEXTBOX:
            RenderTextBox(widget);
            break;
        default:
            assert(0 && "Unreachable");
    }
}

static void CheckMouseAll(Layout *layout) {
    LayoutList *childs = layout->childs;
    for (size_t i = 0; i < childs->count; i++) {
        if (CheckMouse(childs->items[i]->widget)) {
            if (childs->items[i]->widget->onClick) childs->items[i]->widget->onClick(GetMousePosition());
        }
        CheckMouseAll(childs->items[i]);
    }
}

static void RenderLayout(Layout *layout) {
    if(layout->widget && layout->widget->onClick && CheckMouse(layout->widget)) {
        layout->widget->onClick(GetMousePosition());
    }
    DrawRectangleRec(layout->rect, layout->color);
    RenderWidget(layout->widget);
    LayoutList *childs = layout->childs;
    for (size_t i = 0; i < childs->count; i++) {
        RenderLayout(childs->items[i]);
    }
}

void RenderWindow(Layout *rootLayout) {
    rootLayout->rect.width = GetScreenWidth();
    rootLayout->rect.height = GetScreenHeight();
    UpdateLayout(rootLayout);
    RenderLayout(rootLayout);
}

void OnWidgetClick(Widget *widget, void (*callback)(Vector2 mousePos)) {
    widget->onClick = callback;
}

void SetWidgetVisible(Widget *widget, bool visible) {
    widget->visible = visible;
}