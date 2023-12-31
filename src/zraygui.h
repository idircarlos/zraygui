#ifndef ZRAYGUI_H
#define ZRAYGUI_H

#define ZRAYGUI_VERSION  "0.1-dev"

#ifndef ZRAYGUIAPI
    #define ZRAYGUIAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif

#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "../include/raygui.h"

#define DA_INIT_CAP 10

#define da_append(da, item)                                                          \
    do {                                                                             \
        if ((da)->count >= (da)->capacity) {                                         \
            (da)->capacity = (da)->capacity == 0 ? DA_INIT_CAP : (da)->capacity*2;   \
            (da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items)); \
            assert((da)->items != NULL && "Buy more RAM lol");                       \
        }                                                                            \
        (da)->items[(da)->count++] = (item);                                         \
    } while (0)

#define da_remove(da, item)                                                   \
    do {                                                                      \
        for (size_t i = 0; i < (da)->count; i++) {                            \
            if ((da)->items[i] == (item)) {                                   \
                for (size_t j = i; j < (da)->count - 1; j++) {                \
                    (da)->items[j] = (da)->items[j + 1];                      \
                }                                                             \
                (da)->count--;                                                \
                break;                                                        \
            }                                                                 \
        }                                                                     \
    } while (0)

#define ARRAY_SIZE(arr) sizeof(arr)/sizeof(char*)
#define DEBUG_RECTANGLE(rect) printf("Rectangle[%1.f, %1.f, %1.f, %1.f]\n", rect.x, rect.y, rect.width, rect.height);
#define DEBUG_WIDGET(w) printf("Widget[%d, %s, act = %d, visi = %d]\n", w->type, w->label, w->active, w->visible);

#define CHECK_WIDGET_TYPE(w,t) if ((!w) || (w->type) != t) return


typedef struct _component Component;    // Generic empty struct used to contain the pointer to the specific widget (button, label...)
typedef struct _layout_list LayoutList;
typedef struct _widget Widget;
typedef struct _menu_item_list MenuItemList;

typedef enum {
    L_NONE = 0,
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
    W_TEXTBOX,
    W_MENUITEM,
    W_MENUBAR,
    W_LISTVIEW
} WidgetType;

typedef struct Layout {
    Widget *widget;
    Rectangle rect;
    LayoutType type;
    Color color;
    struct Layout *parent;
    LayoutList *childs;
} Layout;

// ME_NONE < ME_HOVER < ME_CLICK < ME_DOWN < ME_RELEASE
typedef enum MouseEvent {
    ME_NONE = 0,
    ME_HOVER,
    ME_CLICK,
    ME_DOWN,
    ME_RELEASE
} MouseEvent;

typedef struct MouseListeners {
    void (*OnClick)(Vector2 mousePos);
    void (*OnHover)(Vector2 mousePos);
    void (*OnPressed)(Vector2 mousePos);
    void (*OnReleased)(Vector2 mousePos);
} MouseListeners;

struct _widget {
    Layout *parent;
    Rectangle rect;
    char *label;
    WidgetType type;
    Component *component;
    bool visible;
    bool active;
    MouseEvent widgetStatus;
    MouseListeners mouseListener;
};

struct _layout_list {
    Layout **items;
    size_t count;
    size_t capacity;
};

typedef struct Button {
    // Empty
} Button;

typedef struct Label {
    Color color;
} Label;

typedef struct CheckBox {
    bool checked;
} CheckBox;

typedef struct Radio {
    bool checked;
} Radio;

typedef struct ComboBox {
    char **options;
    size_t size;
    int index;
    bool opened;
} ComboBox;

typedef struct GroupBox {
    // Empty
} GroupBox;

typedef struct Separator {
    // Empty
} Separator;

typedef struct TextBox {
    char text [1024];
    bool active;
} TextBox;

typedef struct MenuItem {
    Widget *super;
    struct MenuItem *parent;
    MenuItemList *childs;
    bool isLeaf;
    size_t level;
    bool expanded;  // If this item is a root of a (sub)menu then it is true if its options are visibles, false otherwise
} MenuItem;

struct _menu_item_list {
    MenuItem **items;
    size_t count;
    size_t capacity;
};

typedef struct MenuBar {
    MenuItem **items;
    size_t count;
    size_t capacity;
    float currentWidth;
} MenuBar;

typedef struct ListView {
    char **items;
    size_t count;
    size_t capacity;
} ListView;

ZRAYGUIAPI Rectangle GetLayoutBounds(Layout *layout);
ZRAYGUIAPI Vector2 GetLayoutPos(Layout *layout);
ZRAYGUIAPI int GetLayoutX(Layout *layout);
ZRAYGUIAPI int GetLayoutY(Layout *layout);
ZRAYGUIAPI int GetLayoutWidth(Layout *layout);
ZRAYGUIAPI int GetLayoutHeigth(Layout *layout);
ZRAYGUIAPI Rectangle GetWidgetBounds(Widget *widget);
ZRAYGUIAPI void SetWidgetLabel(Widget *widget, char *label);
ZRAYGUIAPI char* GetTextBoxText(Widget *textBoxWidget);
ZRAYGUIAPI void SetTextBoxText(Widget *textBoxWidget, char *text);
ZRAYGUIAPI char* GetComboBoxOption(Widget *comboBoxWidget);
ZRAYGUIAPI void SetComboBoxOption(Widget *comboBoxWidget, int index);
ZRAYGUIAPI void ClearListView(Widget *listViewWidget);
ZRAYGUIAPI void SetWidgetBounds(Widget *widget, Rectangle rect);
ZRAYGUIAPI int GetWidgetX(Widget *widget);
ZRAYGUIAPI int GetWidgetY(Widget *widget);
ZRAYGUIAPI int GetWidgetWidth(Widget *widget);
ZRAYGUIAPI int GetWidgetHeigth(Widget *widget);
ZRAYGUIAPI Vector2 GetWidgetPos(Widget *widget);
ZRAYGUIAPI void SetLabelColor(Widget *labelWidget, Color color);

ZRAYGUIAPI Layout *CreateLayout(Layout *parent, LayoutType type, Color c);
ZRAYGUIAPI Widget *CreateButton(char *text);
ZRAYGUIAPI Widget *CreateLabel(char *text);
ZRAYGUIAPI Widget *CreateCheckBox(char *text);
ZRAYGUIAPI Widget *CreateRadio(char *text);
ZRAYGUIAPI Widget *CreateComboBox(Rectangle rect, char **options, size_t numOptions);
ZRAYGUIAPI Widget *CreateGroupBox(Rectangle rect, char *title);
ZRAYGUIAPI Widget *CreateSeparator(int x, int y, int width);
ZRAYGUIAPI Widget *CreateTextBox(Rectangle rect);
ZRAYGUIAPI Widget *CreateMenuBar();
ZRAYGUIAPI Widget *CreateMenuItem(Widget *parentMenuItemWidget, char *text);
ZRAYGUIAPI Widget *CreateListView(Rectangle rect);

ZRAYGUIAPI void DestroyLayout(Layout *layout);
ZRAYGUIAPI void DestroyWidget(Widget *widget);

ZRAYGUIAPI void AddWidget(Layout *layout, Widget *widget);
ZRAYGUIAPI void AddItemToMenuBar(Widget *menuBarWidget, Widget *menuItemWidget);
ZRAYGUIAPI void AddItemToListView(Widget *listViewWidget, char *item);

ZRAYGUIAPI void RemoveWidget(Layout *layout, Widget *widget);
ZRAYGUIAPI void RemoveItemFromMenuBar(Widget *menuBarWidget, Widget *menuItemWidget);
ZRAYGUIAPI void RemoveItemFromListView(Widget *listViewWidget, size_t index);

ZRAYGUIAPI void RenderWindow(Layout *rootLayout);
ZRAYGUIAPI void SetWidgetVisible(Widget *widget, bool visible);

ZRAYGUIAPI void SetWidgetMouseListeners(Widget *widget, MouseListeners listeners);
ZRAYGUIAPI void SetWidgetOnClick(Widget *widget, void (*callback)(Vector2 mousePos));
ZRAYGUIAPI void SetWidgetOnPressed(Widget *widget, void (*callback)(Vector2 mousePos));
ZRAYGUIAPI void SetWidgetOnHover(Widget *widget, void (*callback)(Vector2 mousePos));
ZRAYGUIAPI void SetWidgetOnReleased(Widget *widget, void (*callback)(Vector2 mousePos));



#endif // ZRAYGUI_H

/***********************************************************************************
*
*   ZRAYGUI IMPLEMENTATION
*
************************************************************************************/

#if defined(ZRAYGUI_IMPLEMENTATION)

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//----------------------------------------------------------------------------------
// Module specific Functions Declaration
//----------------------------------------------------------------------------------

static LayoutList *CreateLayoutList();
static void UpdateLayout(Layout *parent);
static void AddToLayout(Layout *parent, Layout *children);

static Widget *BuildWidget(WidgetType wtype, Component *component);

static void RenderLayout(Layout *layout);
static void RenderWidget(Widget *widget);
static void RenderListView(Widget *widget);
static void RenderMenuBar(Widget *widget);
static void RenderMenuItem(Widget *widget);
static void RenderTextBox(Widget *widget);
static void RenderSeparator(Widget *widget);
static void RenderGroupBox(Widget *widget);
static void RenderComboBox(Widget *widget);
static void RenderRadio(Widget *widget);
static void RenderCheckBox(Widget *widget);
static void RenderLabel(Widget *widget);
static void RenderButton(Widget *widget);

static void DestroySuperWidget(Widget *widget);
static void DestroyButton(Widget *buttonWidget);
static void DestroyLabel(Widget *labelWidget);
static void DestroyCheckBox(Widget *checkBoxWidget);
static void DestroyRadio(Widget *radioWidget);
static void DestroyComboBox(Widget *comboBoxWidget);
static void DestroyGroupBox(Widget *groupBoxWidget);
static void DestroySeparator(Widget *separatorWidget);
static void DestroyTextBox(Widget *textBoxWidget);
static void DestroyMenuBar(Widget *menuBarWidget);
static void DestroyMenuItem(Widget *menuItemWidget);
static void DestroyListView(Widget *listViewWidget);

static void DrawTextCentered(char *text, int fontSize, Rectangle rect);
static void CheckMouse(Widget *widget);

//----------------------------------------------------------------------------------
// Gui Controls Functions Definition
//----------------------------------------------------------------------------------

Rectangle GetLayoutBounds(Layout *layout) {
    return layout->rect;
}

Vector2 GetLayoutPos(Layout *layout) {
    return (Vector2){layout->rect.x, layout->rect.y};
}

int GetLayoutX(Layout *layout) {
    return layout->rect.x;
}

int GetLayoutY(Layout *layout) {
    return layout->rect.y;
}

int GetLayoutWidth(Layout *layout) {
    return layout->rect.width;
}

int GetLayoutHeight(Layout *layout) {
    return layout->rect.height;
}

Rectangle GetWidgetBounds(Widget *widget) {
    return widget->rect;
}

void SetWidgetLabel(Widget *widget, char *label) {
    widget->label = label;
}

char* GetTextBoxText(Widget *textBoxWidget) {
    if (textBoxWidget->type != W_TEXTBOX) return NULL;
    TextBox *textbox = (TextBox*)textBoxWidget->component;
    return textbox->text;
}

void SetTextBoxText(Widget *textBoxWidget, char *text) {
    if (textBoxWidget->type != W_TEXTBOX) return;
    TextBox *textbox = (TextBox*)textBoxWidget->component;
    strcpy(textbox->text,text);
}

char* GetComboBoxOption(Widget *comboBoxWidget) {
    if (comboBoxWidget->type != W_COMBOBOX) return NULL;
    ComboBox *combobox = (ComboBox*)comboBoxWidget->component;
    return combobox->options[combobox->index];
}

void SetComboBoxOption(Widget *comboBoxWidget, int index) {
    if (comboBoxWidget->type != W_COMBOBOX) return;
    ComboBox *combobox = (ComboBox*)comboBoxWidget->component;
    combobox->index = index;
}

void ClearListView(Widget *listViewWidget) {
    if (listViewWidget->type != W_LISTVIEW) return;
    ListView *listview = (ListView*)listViewWidget->component;
    listview->count = 0;
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

void SetLabelColor(Widget *labelWidget, Color color) {
    if (labelWidget->type != W_LABEL) return;
    Label *label = (Label*)labelWidget->component;
    label->color = color;
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

void AddItemToMenuBar(Widget *menuBarWidget, Widget *menuItemWidget) {
    if (menuBarWidget->type != W_MENUBAR || menuItemWidget->type != W_MENUITEM) return;
    MenuBar *menubar = (MenuBar*)menuBarWidget->component;
    MenuItem *menuitem = (MenuItem*)menuItemWidget->component;
    AddWidget(menuBarWidget->parent->parent, menuItemWidget);
    da_append(menubar, menuitem);
    if (menuitem->parent == NULL) {
        menuItemWidget->rect.x = menubar->currentWidth;
        menubar->currentWidth += menuItemWidget->rect.width;
    }
    size_t count = menuitem->childs->count;

    for (size_t i = 0; i < count; i++) {
        MenuItem *childItem = menuitem->childs->items[i];
        Widget *childWidget = childItem->super;
        childItem->level = menuitem->level + 1;
        if (childItem->level > 0) { // If is not the first level
            //printf("soy %s y mi parent es %p\n",menuitem->text, menuitem->parent);
            childWidget->rect.x = menuItemWidget->rect.x + (menuitem->parent == NULL ? 0 : menuItemWidget->rect.width);
            childWidget->rect.y = menuitem->parent == NULL ? menuItemWidget->rect.y + (i+1)*20 : (i*20) + menuItemWidget->rect.y;
            childWidget->rect.width = 100; 
        }
        AddItemToMenuBar(menuBarWidget, childWidget);
    }
}

void AddItemToListView(Widget *listViewWidget, char *item) {
    if (listViewWidget->type != W_LISTVIEW) return;
    ListView *listview = (ListView*)listViewWidget->component;
    da_append(listview, item);
}

void RemoveWidget(Layout *layout, Widget *widget) {
    LayoutList *childs = layout->childs;
    Layout *parent = widget->parent;
    da_remove(childs, parent);
}

void RemoveItemFromMenuBar(Widget *menuBarWidget, Widget *menuItemWidget) {
    if (menuBarWidget->type != W_MENUBAR || menuItemWidget->type != W_MENUITEM) return;
    MenuBar *menubar = (MenuBar*)menuBarWidget->component;
    MenuItem *menuitem = (MenuItem*)menuItemWidget->component;
    RemoveWidget(menuBarWidget->parent->parent, menuItemWidget);
    da_remove(menubar, menuitem);
    // TODO: Re-calculate the new positions of the rest of the widgets.
}

void RemoveItemFromListView(Widget *listViewWidget, size_t index) {
    if (listViewWidget->type != W_LISTVIEW) return;
    ListView *listview = (ListView*)listViewWidget->component;
    if (index < 0 || index >= listview->count) return;
    da_remove(listview, listview->items[index]);
}

void DestroyWidget(Widget *widget) {
    printf("lol\n");
    if (!widget) return;
    switch (widget->type) {
        case W_BUTTON:
            DestroyButton(widget);
            break;
        case W_LABEL:
            printf("here\n");
            DestroyLabel(widget);
            break;
        case W_CHECKBOX:
            DestroyCheckBox(widget);
            break;
        case W_RADIO:
            DestroyRadio(widget);
            break;
        case W_COMBOBOX:
            DestroyComboBox(widget);
            break;
        case W_GROUPBOX:
            DestroyGroupBox(widget);
            break;
        case W_SEPARATOR:
            DestroySeparator(widget);
            break;
        case W_TEXTBOX:
            DestroyTextBox(widget);
            break;
        case W_MENUITEM:
            DestroyMenuItem(widget);
            break;
        case W_MENUBAR:
            DestroyMenuBar(widget);
            break;
        case W_LISTVIEW:
            DestroyListView(widget);
            break;
        default:
            assert(0 && "Unreachable");
    }
    DestroySuperWidget(widget);
}

Widget *CreateButton(char *text) {
    Button *button = (Button*)malloc(sizeof(Button));
    Widget *widget = BuildWidget(W_BUTTON, (Component*)button);
    widget->rect = (Rectangle){0, 0, 75, 25};
    widget->label = text;
    return widget;
}

Widget *CreateLabel(char *text) {
    Label *label = (Label*)malloc(sizeof(Label));
    label->color = GetColor(GuiGetStyle(LABEL, TEXT));
    Widget *widget = BuildWidget(W_LABEL, (Component*)label);
    widget->rect = (Rectangle){
        .x = 0,
        .y = 0,
        .width = GetTextWidth(text),
        .height = 20
    };
    widget->label = malloc(1024*sizeof(char));
    strcpy(widget->label, text);
    int width = GetTextWidth(text);
    int size = MeasureText(text, (float)GuiGetStyle(DEFAULT, TEXT_SIZE));
    widget->rect.width = size;
    return widget;
}

Widget *CreateCheckBox(char *text) {
    CheckBox *checkbox = (CheckBox*)malloc(sizeof(CheckBox));
    checkbox->checked = false;
    Widget *widget = BuildWidget(W_CHECKBOX, (Component*)checkbox);
    widget->rect = (Rectangle){0, 0, 20, 20};
    widget->label = text;
    return widget;
}

Widget *CreateRadio(char *text) {
    Radio *radio = (Radio*)malloc(sizeof(Radio));
    radio->checked = false;
    Widget *widget = BuildWidget(W_RADIO, (Component*)radio);
    widget->rect = (Rectangle){0, 0, 15, 15};
    widget->label = text;
    return widget;
}

Widget *CreateComboBox(Rectangle rect, char **options, size_t numOptions) {
    ComboBox *combobox = (ComboBox*)malloc(sizeof(ComboBox));
    combobox->opened = false;
    combobox->index = 0;
    combobox->size = numOptions;
    combobox->options = options;
    Widget *widget = BuildWidget(W_COMBOBOX, (Component*)combobox);
    widget->rect = rect;
    return widget;
}

Widget *CreateGroupBox(Rectangle rect, char *title) {
    GroupBox *groupbox = (GroupBox*)malloc(sizeof(GroupBox));
    Widget *widget = BuildWidget(W_GROUPBOX, (Component*)groupbox);
    widget->rect = rect;
    widget->label = strdup(title);
    return widget;
}

Widget *CreateSeparator(int x, int y, int width) {
    Separator *separator = (Separator*)malloc(sizeof(Separator));
    Widget *widget = BuildWidget(W_SEPARATOR, (Component*)separator);
    widget->rect = (Rectangle){x,y,width,0};
    return widget;
}

Widget *CreateTextBox(Rectangle rect) {
    TextBox *textbox = (TextBox*)malloc(sizeof(TextBox));
    textbox->active = false;
    memset(textbox->text, 0, ARRAY_SIZE(textbox->text));
    Widget *widget = BuildWidget(W_TEXTBOX, (Component*)textbox);
    widget->rect = rect;
    return widget;
}

Widget *CreateMenuBar() {
    MenuBar *menubar = (MenuBar*)malloc(sizeof(MenuBar));
    menubar->items = (MenuItem**)malloc(sizeof(MenuItem*));
    menubar->count = 0;
    menubar->capacity = 0;
    menubar->currentWidth = 0.0f;
    Widget *widget = BuildWidget(W_MENUBAR, (Component*)menubar);
    widget->rect = (Rectangle){0,0,GetScreenWidth(),40};
    return widget;
}

Widget *CreateMenuItem(Widget *parentMenuItemWidget, char *text) {
    MenuItem *parentMenuItem = (parentMenuItemWidget && parentMenuItemWidget->type == W_MENUITEM) ? (MenuItem*)parentMenuItemWidget->component : NULL;
    MenuItem *menuitem = (MenuItem*)malloc(sizeof(MenuItem));
    menuitem->parent = parentMenuItem; 
    menuitem->isLeaf = true;
    menuitem->level = 0;
    menuitem->expanded = false;
    MenuItemList *childsList = (MenuItemList*)malloc(sizeof(MenuItemList));
    childsList->items = (MenuItem**)malloc(sizeof(MenuItem*));
    childsList->count = 0;
    childsList->capacity = 0;
    menuitem->childs = childsList;
    Widget *widget = BuildWidget(W_MENUITEM, (Component*)menuitem);
    menuitem->super = widget;
    int size = GetTextWidth(text);
    widget->rect = (Rectangle){0,0,size*2,20};
    widget->label = text;
    if (parentMenuItem != NULL) {
        parentMenuItem->isLeaf = false; // Now the parent widget is not a leaf and should be a dropdownbox
        parentMenuItemWidget->active = false;
        da_append(parentMenuItem->childs, menuitem);
        widget->rect = (Rectangle) {
            .x = parentMenuItemWidget->rect.x,
            .y = parentMenuItemWidget->rect.y,
            .width = parentMenuItemWidget->rect.width,
            .height = parentMenuItemWidget->rect.height
        };
    }
    return widget;
}

Widget *CreateListView(Rectangle rect) {
    ListView *listview = (ListView*)malloc(sizeof(ListView));
    listview->items = (char**)malloc(sizeof(char*));
    listview->count = 0;
    listview->capacity = 0;
    Widget *widget = BuildWidget(W_LISTVIEW, (Component*)listview);
    widget->rect = rect;
    return widget;
}

// Does not destroy widget
void DestroyLayout(Layout *layout) {
    if(!layout) return;
    Layout *parent = layout->parent;
    if (parent) {
        LayoutList *childs = parent->childs;
        da_remove(childs, layout);
        for (size_t i = 0; i < layout->childs->count; i++) {
            DestroyLayout(layout->childs->items[i]);
        }
        free(layout->childs->items);
        free(layout->childs);
        free(layout);
    }
}

void DestroyButton(Widget *buttonWidget) {
    CHECK_WIDGET_TYPE(buttonWidget, W_BUTTON);
    Button *button = (Button*)buttonWidget->component;
}

void DestroyLabel(Widget *labelWidget) {
    CHECK_WIDGET_TYPE(labelWidget, W_LABEL);
    Label *label = (Label*)labelWidget->component;
}

void DestroyCheckBox(Widget *checkBoxWidget) {
    CHECK_WIDGET_TYPE(checkBoxWidget, W_CHECKBOX);
    CheckBox *checkbox = (CheckBox*)checkBoxWidget->component;
}

void DestroyRadio(Widget *radioWidget) {
    CHECK_WIDGET_TYPE(radioWidget, W_RADIO);
    Radio *radio = (Radio*)radioWidget->component;
}

void DestroyComboBox(Widget *comboBoxWidget) {
    CHECK_WIDGET_TYPE(comboBoxWidget, W_COMBOBOX);
    ComboBox *combobox = (ComboBox*)comboBoxWidget->component;
    for (size_t i = 0; i < combobox->size; i++) {
        free(combobox->options[i]);
    }
    free(combobox->options);
}

void DestroyGroupBox(Widget *groupBoxWidget) {
    CHECK_WIDGET_TYPE(groupBoxWidget, W_GROUPBOX);
    GroupBox *groupbox = (GroupBox*)groupBoxWidget->component;
}

void DestroySeparator(Widget *separatorWidget) {
    CHECK_WIDGET_TYPE(separatorWidget, W_SEPARATOR);
    Separator *separator = (Separator*)separatorWidget->component;
}

void DestroyTextBox(Widget *textBoxWidget) {
    CHECK_WIDGET_TYPE(textBoxWidget, W_TEXTBOX);
    TextBox *textbox = (TextBox*)textBoxWidget->component;
}

void DestroyMenuBar(Widget *menuBarWidget) {
    CHECK_WIDGET_TYPE(menuBarWidget, W_MENUBAR);
    MenuBar *menubar = (MenuBar*)menuBarWidget->component;
    free(menubar->items);
}

void DestroyMenuItem(Widget *menuItemWidget) {
    CHECK_WIDGET_TYPE(menuItemWidget, W_MENUITEM);
    MenuItem *menuitem = (MenuItem*)menuItemWidget->component;
    MenuItemList *childs = menuitem->childs;
    free(childs->items);
    free(childs);
}

void DestroyListView(Widget *listViewWidget) {
    CHECK_WIDGET_TYPE(listViewWidget, W_LISTVIEW);
    ListView *listview = (ListView*)listViewWidget->component;
    free(listview->items);
}

void RenderWindow(Layout *rootLayout) {
    rootLayout->rect.width = GetScreenWidth();
    rootLayout->rect.height = GetScreenHeight();
    UpdateLayout(rootLayout);
    RenderLayout(rootLayout);
}

void SetWidgetMouseListeners(Widget *widget, MouseListeners listeners) {
    widget->mouseListener = listeners;
}

void SetWidgetOnClick(Widget *widget, void (*callback)(Vector2 mousePos)) {
    widget->mouseListener.OnClick = callback;
}

void SetWidgetOnPressed(Widget *widget, void (*callback)(Vector2 mousePos)) {
    widget->mouseListener.OnPressed = callback;
}

void SetWidgetOnHover(Widget *widget, void (*callback)(Vector2 mousePos)) {
    widget->mouseListener.OnHover = callback;
}

void SetWidgetOnReleased(Widget *widget, void (*callback)(Vector2 mousePos)) {
    widget->mouseListener.OnReleased = callback;
}

void SetWidgetVisible(Widget *widget, bool visible) {
    widget->visible = visible;
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------

static Widget *BuildWidget(WidgetType wtype, Component *component) {
    Widget *widget = (Widget*)malloc(sizeof(Widget));
    widget->parent = NULL;
    widget->type = wtype;
    widget->component = component;
    widget->rect = (Rectangle){0};
    widget->active = true;
    widget->visible = true;
    widget->label = NULL;
    widget->mouseListener = (MouseListeners){0};
    widget->widgetStatus = ME_NONE;
    return widget;
}

static void DrawTextCentered(char *text, int fontSize, Rectangle rect) {
    int size = MeasureText(text, fontSize);
    DrawText(text, rect.x + (rect.width/2) - (size/2),rect.y + rect.height/2, fontSize, BLACK);
}

static void RenderButton(Widget *widget) {
    Button *button = (Button*)widget->component;
    GuiButton(widget->rect, widget->label);
}

static void RenderLabel(Widget *widget) {
    Label *label = (Label*)widget->component;
    DrawText(widget->label, widget->rect.x, widget->rect.y, GuiGetStyle(LABEL, TEXT_SIZE), label->color);
}

static void RenderCheckBox(Widget *widget) {
    CheckBox *checkbox = (CheckBox*)widget->component;
    GuiCheckBox(widget->rect, widget->label, &checkbox->checked);
}

static void RenderRadio(Widget *widget) {
    Radio *radio = (Radio*)widget->component;
    Layout *parent = widget->parent->parent;
    GuiRadioButton(widget->rect, widget->label, &radio->checked);
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
    GuiGroupBox(widget->rect, widget->label);
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

static void RenderMenuItem(Widget *widget) {
    MenuItem *menuitem = (MenuItem*)widget->component;
    Layout *parent = widget->parent->parent;
    size_t childCount = menuitem->childs->count;
    if (childCount > 0) {
        DrawCircle(widget->rect.x + widget->rect.width - 7, widget->rect.y + widget->rect.height/2, 3, BLACK);
        Rectangle rect = {
            .x = widget->rect.x + (menuitem->parent == NULL ? 0 : widget->rect.width),
            .y = widget->rect.y + (menuitem->parent == NULL ? 20 : 0),
            .width = 100, // TODO: Change this
            .height = childCount * 20
        };
        DrawRectangleLinesEx(rect, 2, GREEN);
        if (CheckCollisionPointRec(GetMousePosition(), widget->rect)) {
            if (menuitem->parent == NULL || (menuitem->parent && menuitem->parent->expanded)) {
                menuitem->expanded = true;
                for (size_t i = 0; i < childCount; i++) {   // But if we find that one of our child is expanded, then we should keep this menu expanded as well
                    menuitem->childs->items[i]->super->active = true;   // also, set all child widgets to active, so the listeners can handle events
                }
            }
        }
        else if (!CheckCollisionPointRec(GetMousePosition(), rect)) {
            menuitem->expanded = false; // By default, we do not expand if we are not in the widget rectangle
            for (size_t i = 0; i < childCount; i++) {   // But if we find that one of our child is expanded, then we should keep this menu expanded as well
                if (menuitem->childs->items[i]->expanded) {
                    menuitem->expanded = true;
                }
            }
            if (!menuitem->expanded) {
                for (size_t i = 0; i < childCount; i++) {   
                    menuitem->childs->items[i]->super->active = false;   // now deactivete widgets that are not expanded
                }
            }
        }
    }
    MenuItem *parentMenuItem = menuitem->parent;
    if (menuitem->parent == NULL) {
        GuiLabelButton(widget->rect, widget->label);
    }
    else if (parentMenuItem && parentMenuItem->expanded) {
        GuiButton(widget->rect, widget->label);
    }
}

static void RenderMenuBar(Widget *widget) {
    MenuBar *menubar = (MenuBar*)widget->component;
    Layout *parent = widget->parent->parent;
    GuiLine(widget->rect, NULL);
}

static void RenderListView(Widget *widget) {
    ListView *listview = (ListView*)widget->component;
    Layout *parent = widget->parent->parent;
    int a = 0;
    int b = 0;
    int c = 0;
    GuiListViewEx(widget->rect, listview->items, listview->count, &a, &b, &c);
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
            RenderCheckBox(widget);
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
        case W_MENUITEM:
            RenderMenuItem(widget);
            break;
        case W_MENUBAR:
            RenderMenuBar(widget);
            break;
        case W_LISTVIEW:
            RenderListView(widget);
            break;
        default:
            assert(0 && "Unreachable");
    }
}

static void DestroySuperWidget(Widget *widget) {
    free(widget->component);
    Layout *wrapper = widget->parent;
    Layout *parent = wrapper->parent;
    da_remove(parent->childs, wrapper);
    free(wrapper);
}

static LayoutList *CreateLayoutList() {
    LayoutList *list = (LayoutList*)malloc(sizeof(LayoutList));
    list->items = (Layout**)malloc(sizeof(Layout*));
    list->capacity = 0;
    list->count = 0;
    return list;
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

static void RenderLayout(Layout *layout) {
    CheckMouse(layout->widget);
    DrawRectangleRec(layout->rect, layout->color);
    RenderWidget(layout->widget);
    LayoutList *childs = layout->childs;
    for (size_t i = 0; i < childs->count; i++) {
        RenderLayout(childs->items[i]);
    }
}

static void CheckMouse(Widget *widget) {
    if (!widget || !widget->active) return;
    bool pressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    bool down = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    bool released = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
    bool inside = CheckCollisionPointRec(GetMousePosition(), widget->rect);
    
    MouseEvent me;
    MouseEvent prevMe = widget->widgetStatus;
    
    if (pressed && inside) me = ME_CLICK; 
    else if (down && inside) {
        if (prevMe == ME_CLICK || prevMe == ME_DOWN) me = ME_DOWN;
        else me = ME_NONE;
    } 
    else if (released && prevMe == ME_DOWN) me = ME_RELEASE;
    else if (inside) me = ME_HOVER;
    else me = ME_NONE;

    if (me == ME_CLICK && widget->mouseListener.OnClick) widget->mouseListener.OnClick(GetMousePosition());
    else if (me == ME_HOVER && widget->mouseListener.OnHover) widget->mouseListener.OnHover(GetMousePosition());
    else if (me == ME_DOWN && widget->mouseListener.OnPressed) widget->mouseListener.OnPressed(GetMousePosition());
    else if (me == ME_RELEASE && widget->mouseListener.OnReleased) widget->mouseListener.OnReleased(GetMousePosition());

    widget->widgetStatus = me;  // TODO: Change this to make OnReleased work
}

#endif // ZRAYGUI_IMPLEMENTATION