#include "../src/zraygui.h"

void exit_program(Vector2 mousePos) {
    exit(0);
}

int main(void) {
    int w = 750;
    int h = 600;
    InitWindow(w,h,"Menu Bar - zraygui");

    Layout *root = CreateLayout(NULL, L_NONE, WHITE);

    Widget *menuBar = CreateMenuBar();

    Widget *fileItem = CreateMenuItem(NULL, "File");
        Widget *openItem = CreateMenuItem(fileItem, "Open");
        Widget *saveItem = CreateMenuItem(fileItem, "Save");
        Widget *saveAsItem = CreateMenuItem(fileItem, "Save as...");
            Widget *saveAsTxtItem = CreateMenuItem(saveAsItem, "TXT");
            Widget *saveAsAudioItem = CreateMenuItem(saveAsItem, "Audio...");
                Widget *saveAsAudioMp3Item = CreateMenuItem(saveAsAudioItem, "MP3");
                Widget *saveAsAudioWavItem = CreateMenuItem(saveAsAudioItem, "WAV");
                Widget *saveAsAudioOggItem = CreateMenuItem(saveAsAudioItem, "OGG");
            Widget *saveAsPdfItem = CreateMenuItem(saveAsItem, "PDF");
        Widget *exitItem = CreateMenuItem(fileItem, "Exit");

    Widget *editItem = CreateMenuItem(NULL, "Edit");
        Widget *copyItem = CreateMenuItem(editItem, "Copy");
        Widget *pasteItem = CreateMenuItem(editItem, "Paste");
        Widget *undoItem = CreateMenuItem(editItem, "Undo");
        Widget *redoItem = CreateMenuItem(editItem, "Redo");

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

    OnWidgetClick(exitItem, exit_program);      // Add an onClick listener. When the exit button of the menu bar is clicked, the program will finish.
    
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(WHITE);
            RenderWindow(root);
        EndDrawing();
    }

    return 0;
}
