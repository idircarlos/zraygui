# zraygui - Retained GUI based on raylib's raygui

#### This software is unfinished!! Also, there is a lot of spaghetti code that I will be fixing soon.

**zraygui** is an attempt of transforming the well-known **raygui** library, which currently implements an immediate mode GUI (ImGUI) using the **raylib** framework, into a **retained mode GUI**. This transition from immediate mode to retained mode brings about a significant shift in how user interfaces are managed and drawn.

## What is raylib and raygui?

* **raylib** is a popular and lightweight C library designed for making 2D and 3D games. It provides a wealth of features for graphics, audio, and input handling while keeping things simple and accessible.
* **raygui** is a companion library for raylib, focusing on creating graphical user interfaces (GUIs). It employs an **immediate mode GUI** paradigm, where each frame, GUI elements are drawn and handled individually. This approach is efficient and straightforward for simple interfaces but can become complex for larger projects.

## Transitioning to Retained Mode with zraygui

**zraygui** takes the fundamental concept of **raygui** and evolves it into a **retained mode GUI** based of Layouts/Panels and Widgets. In retained mode, GUI elements persist beyond the frame they were created in. This approach allows for a more structured and organized GUI construction, particularly for complex applications.

### Features Check List:

 - [ ] Implement almost every Gui control of raygui.
 - [x] Implement Radio Buttons.
 - [x] Implement Menu bar with sub menus.

### Key Benefits of zraygui:

1. **Dynamic Content:** Retained mode accommodates dynamic content changes seamlessly. Adding, removing, or altering elements during runtime is more natural and less error-prone.
2. **Widget Listeners:** This retained mode brings the benefit of having different kind of listeners such as OnClick or OnHover.
3. **Menu Bar**: One of the motivation of this project was to bring an easy way to create a Menu Bar with multiples indented Sub Menus.
4. **Radio Buttons:** With an immediate GUI it is impossible (with out saving data in the API source) to implement Radio Buttons. Since zraygui is based on layouts/panels and widgets, we can implement this easily!

## Getting Started with zraygui

To begin using **zraygui** in your raylib-based project, follow these steps:

1. Copy the *zraygui.h* file from the src folder and paste it in your project workspace.
2. Define the **ZRAYGUI_IMPLEMENTATION** and include the header file in your sources.
3. Start creating your beautiful Raylib GUI applications with **zraygui**!

## Example

```c
#define ZRAYGUI_IMPLEMENTATION
#include "../src/zraygui.h"

Layout* root;
Widget *button, *label;

void my_callback(Vector2 mousePos) {
    SetWidgetVisible(label, true);
}

int main(void) {
    int w = 250;
    int h = 100;
    InitWindow(w,h,"Hello World - zraygui");

    root = CreateLayout(NULL, L_NONE, WHITE);
    button = CreateButton("Press me!");
    label = CreateLabel("Hello world from zraygui!");

    SetWidgetPos(button, (Vector2){(w - GetWidgetWidth(button)) / 2, (h - GetWidgetHeight(button)) / 4});
    SetWidgetPos(label, (Vector2){(w - GetWidgetWidth(label)) / 2, ((h - GetWidgetHeight(label)) / 2) + 15});
    
    SetWidgetVisible(label, false);

    AddWidget(root, button);
    AddWidget(root, label);
    
    SetWidgetOnClick(button, my_callback);

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(WHITE);
            RenderWindow(root);
        EndDrawing();
    }

    return 0;
}
```

## Contributing to zraygui

zraygui is an open-source project that welcomes contributions from the community. Whether you're interested in improving documentation, adding new features, or fixing bugs, your efforts are valued.
