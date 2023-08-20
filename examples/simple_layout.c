#include "../src/zraygui.h"

int main(void) {
    int w = 900;
    int h = 600;
    InitWindow(w, h, "Simple Layout - zraygui");

    Layout* root = CreateLayout(NULL, L_HORZ, WHITE);

    Layout* left = CreateLayout(root, L_NONE, RED);
    Layout* middle = CreateLayout(root, L_NONE, GREEN);
    Layout* right = CreateLayout(root, L_NONE, BLUE);

    Widget *leftLabel = CreateLabel("This label is in the left layout");
    Widget *middleLabel = CreateLabel("This label is in the middle layout");
    Widget *rightLabel = CreateLabel("This label is in the right layout");

    SetWidgetPos(leftLabel, (Vector2){(GetLayoutWidth(left) - GetWidgetWidth(leftLabel)) / 2, (GetLayoutHeight(left) - GetWidgetHeight(leftLabel)) / 2});
    SetWidgetPos(middleLabel, (Vector2){(GetLayoutWidth(middle) - GetWidgetWidth(middleLabel)) / 2, (GetLayoutHeight(middle) - GetWidgetHeight(middleLabel)) / 2});
    SetWidgetPos(rightLabel, (Vector2){(GetLayoutWidth(right) - GetWidgetWidth(rightLabel)) / 2, (GetLayoutHeight(right) - GetWidgetHeight(rightLabel)) / 2});

    SetLabelColor(leftLabel, WHITE);
    SetLabelColor(middleLabel, WHITE);
    SetLabelColor(rightLabel, WHITE);

    AddWidget(left, leftLabel);
    AddWidget(middle, middleLabel);
    AddWidget(right, rightLabel);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(WHITE);
            RenderWindow(root);
        EndDrawing();
    }

    return 0;
}