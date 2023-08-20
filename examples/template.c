#include "../src/zraygui.h"

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600

int main(void) {

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Template - zraygui");

    Layout *root = CreateLayout(NULL, L_NONE, BLANK);   // Create the surronding layout, which will cover the entire window

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(WHITE);
            RenderWindow(root); // Render recursively every widget inside the root layout (and other root child layouts)
        EndDrawing();
    }

    return 0;
}