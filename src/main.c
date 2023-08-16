#include "zraygui.h"


Layout* root;
Widget *button, *label;

void my_callback(Vector2 mousePos) {
    SetWidgetVisible(label, true);
}

int main(void) {
    int w = 250;
    int h = 100;
    InitWindow(w,h,"Hello world zraygui");

    root = CreateLayout(NULL, L_NONE, WHITE);
    button = CreateButton("Press me!");
    label = CreateLabel("Hello, World!");

    int buttonWidth = 75;
    int buttonHeight = 25;
    SetWidgetBounds(button, (Rectangle){(w - buttonWidth) / 2, (h - buttonHeight) / 4, buttonWidth, buttonHeight});

    Rectangle labelRect = GetWidgetBounds(label);
    SetWidgetBounds(label, (Rectangle){(w - labelRect.width) / 2, ((h - labelRect.height) / 2) + 15, labelRect.width, labelRect.height});
    SetWidgetVisible(label, false);

    AddWidget(root, button);
    AddWidget(root, label);
    OnWidgetClick(button, my_callback); 
    
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(WHITE);
            RenderWindow(root);
        EndDrawing();
    }
    return 0;
}
