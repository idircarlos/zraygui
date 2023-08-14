#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

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
    W_RADIO
} WidgetType;

typedef struct Layout {
    Widget *widget;
    Rectangle rect;
    LayoutType orient;
    Color color;
    struct Layout *parent;
    LayoutList *childs;
} Layout;

struct _widget {
    Layout *parent;
    Rectangle rect;
    WidgetType type;
    void *component;
    void (*onClick)(Vector2 mousePos);
};

struct _layout_list {
    Layout **items;
    size_t count;
    size_t capacity;
};

typedef struct Button {
    char *text;
    Color color;
} Button;

typedef struct Label {
    char *text;
} Label;

typedef struct Checkbox {
    char *text;
    bool checked;
} Checkbox;

typedef struct Radio {
    char *text;
    bool checked;
} Radio;

void SetWidgetBounds(Widget *widget, Rectangle rect) {
    widget->rect.x = rect.x;
    widget->rect.y = rect.y;
}

void UpdateLayout(Layout *parent) {
    if (parent->widget) SetWidgetBounds(parent->widget, parent->rect);
    int offsetx = parent->rect.width  / parent->childs->count;
    int offsety = parent->rect.height / parent->childs->count;
    for (size_t i = 0; i < parent->childs->count; i++) {
        Layout *children = parent->childs->items[i];
        switch (parent->orient) {
            case L_HORZ:
                children->rect.x = parent->rect.x + (i*offsetx);
                children->rect.y = parent->rect.y;
                children->rect.width = offsetx;
                children->rect.height = parent->rect.height;
                break;

            case L_VERT:
                children->rect.x = parent->rect.x;
                children->rect.y = parent->rect.y + (i*offsety);
                children->rect.width = parent->rect.width;
                children->rect.height = offsety;
                break;

            default:
                assert(0 && "Unreachable");
        }
        UpdateLayout(children);
    }
     
}

void AddToLayout(Layout *parent, Layout *children) {
    if (!parent) return;
    da_append(parent->childs, children);
    children->parent = parent;
    UpdateLayout(parent);
}

Layout *CreateLayout(Layout *parent, LayoutType orient, Color c) {
    Rectangle rect = {0};
    if (!parent) {
        rect.x = 0;
        rect.y = 0;
        rect.width = GetScreenWidth();
        rect.height = GetScreenHeight();
    }
    Layout *layout = (Layout*)malloc(sizeof(Layout));
    layout->rect = rect;
    layout->orient = orient;
    layout->childs = (LayoutList*)malloc(sizeof(LayoutList));
    layout->color = c;
    AddToLayout(parent, layout);
    return layout;
}

void AddWidget(Layout *layout, Widget *widget) {
    Layout *l = CreateLayout(layout, L_HORZ, WHITE);
    l->widget = widget;
    widget->parent = l;
    SetWidgetBounds(widget, l->rect);
}

Widget *BuildWidget(WidgetType wtype, void *component) {
    Widget *widget = (Widget*)malloc(sizeof(Widget));
    widget->parent = NULL;
    widget->type = wtype;
    widget->component = component;
    widget->rect = (Rectangle){0};
    widget->onClick = NULL;
    return widget;
}

Widget *CreateButton(char *text, Color c) {
    Button *button = (Button*)malloc(sizeof(Button));
    button->color = c;
    button->text = text;
    Widget *widget = BuildWidget(W_BUTTON, button);
    widget->rect = (Rectangle){0, 0, 90, 30};
    return widget;
}

Widget *CreateLabel(char *text) {
    Label *label = (Label*)malloc(sizeof(Label));
    label->text = text;
    Widget *widget = BuildWidget(W_LABEL, label);
    widget->rect = (Rectangle){0, 0, 90, 30};
    return widget;
}

Widget *CreateCheckbox(char *text) {
    Checkbox *checkbox = (Checkbox*)malloc(sizeof(Checkbox));
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

void DrawTextCentered(char *text, int fontSize, Rectangle rect) {
    int size = MeasureText(text, fontSize);
    DrawText(text, rect.x + (rect.width/2) - (size/2),rect.y + rect.height/2, fontSize, BLACK);
}

bool CheckMouse(Widget *widget) {
    if (!widget) return false;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), widget->rect)) {
        return true;
    }
    return false;
}

void RenderButton(Widget *widget) {
    Button *button = (Button*)widget->component;
    GuiButton(widget->rect, button->text);
}

void RenderLabel(Widget *widget) {
    Label *label = (Label*)widget->component;
    GuiLabel(widget->rect, label->text);
}

void RenderCheckbox(Widget *widget) {
    Checkbox *checkbox = (Checkbox*)widget->component;
    GuiCheckBox(widget->rect, checkbox->text, &checkbox->checked);
}

void RenderRadio(Widget *widget) {
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

void RenderWidget(Widget *widget) {
    if (!widget) return;
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
        default:
            assert(0 && "Unreachable");
    }
}

void RenderLayout(Layout *layout) {
    DrawRectangleRec(layout->rect, layout->color);
    RenderWidget(layout->widget);
    LayoutList *childs = layout->childs;
    assert(childs != NULL);
    for (size_t i = 0; i < childs->count; i++) {
        RenderLayout(childs->items[i]);
    }
}

void CheckMouseAll(Layout *layout) {
    LayoutList *childs = layout->childs;
    for (size_t i = 0; i < childs->count; i++) {
        if (CheckMouse(childs->items[i]->widget)) {
            if (childs->items[i]->widget->onClick) childs->items[i]->widget->onClick(GetMousePosition());
        }
        CheckMouseAll(childs->items[i]);
    }
}


void OnWidgetClick(Widget *widget, void (*callback)(Vector2 mousePos)) {
    widget->onClick = callback;
}

void my_callback(Vector2 mousePos) {
    printf("Mouse Pos: (%d,%d)\n", (int)mousePos.x, (int)mousePos.y);
}

int main(void) {
    int w = 1000;
    int h = 700;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(w,h,"zgui");
    Layout* root = CreateLayout(NULL,L_HORZ, WHITE);
    Layout* child1 = CreateLayout(root,L_VERT, GetColor(0x5a7b91ff));
    Layout* child1_1 = CreateLayout(child1,L_VERT, GetColor(0x5a7b91ff));
    Layout* child1_2 = CreateLayout(child1,L_VERT, MAGENTA);
    Layout* child2 = CreateLayout(root,L_HORZ, GREEN);
    Layout* child3 = CreateLayout(root,L_VERT, BLUE);
    
    Layout* child4 = CreateLayout(child3,L_VERT, GetColor(0xd10d94ff));
    Layout* child5 = CreateLayout(child3,L_HORZ, ORANGE);
    Layout* child6 = CreateLayout(child3,L_VERT, GetColor(0x07dbbfff));

    Widget* button = CreateButton("My button", YELLOW);
    Widget* label = CreateLabel("My label");
    Widget* label2 = CreateLabel("My label 2");
    Widget* testLabel = CreateLabel("Test label");
    Widget* checkbox1 = CreateCheckbox("Checkbox 1");
    Widget* checkbox2 = CreateCheckbox("Checkbox 2");
    Widget* radio1 = CreateRadio("Option A");
    Widget* radio2 = CreateRadio("Option B");
    Widget* radio3 = CreateRadio("Option C");

    AddWidget(child5, button);
    AddWidget(child5, label);
    AddWidget(child5, label2);
    AddWidget(child4, checkbox1);
    AddWidget(child4, checkbox2);

    AddWidget(child6, radio1);
    AddWidget(child6, radio2);
    AddWidget(child6, radio3);
    AddWidget(child6, testLabel);

    OnWidgetClick(button, my_callback);

    while (!WindowShouldClose()) {
        root->rect.width = GetRenderWidth();
        root->rect.height = GetRenderHeight();
        UpdateLayout(root);
        BeginDrawing();
            ClearBackground(WHITE);
            CheckMouseAll(root);
            RenderLayout(root);
        EndDrawing();
    }
    return 0;
}
