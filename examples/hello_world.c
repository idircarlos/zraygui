#include "../src/zraygui.h"


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

    SetWidgetPos(button, (Vector2){(w - GetWidgetWidth(button)) / 2, (h - GetWidgetHeight(button)) / 4});
    SetWidgetPos(label, (Vector2){(w - GetWidgetWidth(label)) / 2, ((h - GetWidgetHeight(label)) / 2) + 15});
    
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