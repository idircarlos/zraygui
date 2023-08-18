#include "zraygui.h"

Layout* root;
Widget *button, *label;

void my_callback(Vector2 mousePos) {
    SetWidgetVisible(label, true);
}

int main(void) {
    int w = 750;
    int h = 600;
    InitWindow(w,h,"Hello world zraygui");

    root = CreateLayout(NULL, L_NONE, WHITE);
    button = CreateButton("Press me!");
    label = CreateLabel("Hello, World!");

    SetWidgetPos(button, (Vector2){(w - GetWidgetWidth(button)) / 2, (h - GetWidgetHeight(button)) / 4});
    SetWidgetPos(label, (Vector2){(w - GetWidgetWidth(label)) / 2, ((h - GetWidgetHeight(label)) / 2) + 15});
    
    SetWidgetVisible(label, false);

    AddWidget(root, button);
    AddWidget(root, label);
    OnWidgetClick(button, my_callback);
    Widget *menuBar = CreateMenuBar();
    Widget *fileItem = CreateMenuItem(NULL, "File");
    Widget *editItem = CreateMenuItem(fileItem, "Edit");
    Widget *toolsItem = CreateMenuItem(NULL, "Tools");
    Widget *propertiesItem = CreateMenuItem(NULL, "Properties");
    Widget *optionsItem = CreateMenuItem(NULL, "Options");
    Widget *helpItem = CreateMenuItem(NULL, "Help");

    AddWidget(root, menuBar);
    AddItemToMenuBar(menuBar, fileItem);
    AddItemToMenuBar(menuBar, editItem);
    AddItemToMenuBar(menuBar, toolsItem);
    AddItemToMenuBar(menuBar, propertiesItem);
    AddItemToMenuBar(menuBar, optionsItem);
    AddItemToMenuBar(menuBar, helpItem);
    char *nose [3] = {"Prueba","Dos","Ole"};
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(WHITE);
            RenderWindow(root);
        EndDrawing();
    }
    return 0;
}
