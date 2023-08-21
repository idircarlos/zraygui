#define ZRAYGUI_IMPLEMENTATION
#include "../src/zraygui.h"

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600

#define EMPTY_STRING ""

Widget *firstNameBox;
Widget *lastNameBox;
Widget *ageBox;
Widget *gendreBox;
Widget *resetButton;
Widget *submitButton;

Widget *listView;

Widget *importButton;
Widget *exportButton;
Widget *clearButton;
Widget *exitButton;

void reset_form(Vector2 mousePos) {
    (void) mousePos;
    SetTextBoxText(firstNameBox, EMPTY_STRING);
    SetTextBoxText(lastNameBox, EMPTY_STRING);
    SetTextBoxText(ageBox, EMPTY_STRING);
    SetComboBoxOption(gendreBox, 0);
}

void submit_form(Vector2 mousePos) {
    (void) mousePos;
    char *item = (char*)malloc(128);
    sprintf(item,"%s, %s. %s. %s", GetTextBoxText(lastNameBox), GetTextBoxText(firstNameBox), GetTextBoxText(ageBox), GetComboBoxOption(gendreBox));
    AddItemToListView(listView, item);
}

void import_table(Vector2 mousePos) {
    (void) mousePos;
    // Implement the import of the table into a file
}

void export_table(Vector2 mousePos) {
    (void) mousePos;
    // Implement the export of the table into a file
}

void clear_table(Vector2 mousePos) {
    (void) mousePos;
    ClearListView(listView);
}

void exit_program(Vector2 mousePos) {
    (void) mousePos;
    exit(0);
}

void BuildFormLayout(Layout *formLayout) {
    Widget *firstNameLabel = CreateLabel("First name");
    Widget *lastNameLabel = CreateLabel("Last name");
    Widget *ageLabel = CreateLabel("Age");
    Widget *gendreLabel = CreateLabel("Gendre");

    firstNameBox = CreateTextBox((Rectangle){125,50,250,20});
    lastNameBox = CreateTextBox((Rectangle){125,100,250,20});
    ageBox = CreateTextBox((Rectangle){125,150,250,20});

    int gendreSize = 2;
    char **gendres = (char**)malloc(gendreSize*sizeof(char*));
    gendres[0] = strdup("Masculine");
    gendres[1] = strdup("Femenine");
    
    gendreBox = CreateComboBox((Rectangle){125,200,250,20}, gendres, gendreSize);

    Widget *formBox = CreateGroupBox((Rectangle){10,10,SCREEN_WIDTH/2 - 20,SCREEN_HEIGHT/2 - 20}, "Form");

    resetButton = CreateButton("Reset");
    submitButton = CreateButton("Submit data");

    AddWidget(formLayout, formBox);

    SetWidgetPos(resetButton, (Vector2){(GetWidgetWidth(formBox) - GetWidgetWidth(submitButton)) / 4, (GetWidgetHeight(formBox)) - ((GetWidgetHeight(formBox) - GetWidgetHeight(resetButton)) / 8 + 7)});
    SetWidgetPos(submitButton, (Vector2){GetWidgetWidth(formBox) - GetWidgetWidth(resetButton) - ((GetWidgetWidth(formBox) - GetWidgetWidth(resetButton)) / 4), (GetWidgetHeight(formBox)) - ((GetWidgetHeight(formBox) - GetWidgetHeight(submitButton)) / 8 + 7)});

    SetWidgetPos(firstNameLabel, (Vector2){50, 50 + 5});
    SetWidgetPos(lastNameLabel, (Vector2){50, 100 + 5});
    SetWidgetPos(ageLabel, (Vector2){50, 150 + 5});
    SetWidgetPos(gendreLabel, (Vector2){50, 200 + 5});

    AddWidget(formLayout, resetButton);
    AddWidget(formLayout, submitButton);

    AddWidget(formLayout, firstNameLabel);
    AddWidget(formLayout, lastNameLabel);
    AddWidget(formLayout, ageLabel);
    AddWidget(formLayout, gendreLabel);

    AddWidget(formLayout, firstNameBox);
    AddWidget(formLayout, lastNameBox);
    AddWidget(formLayout, ageBox);
    AddWidget(formLayout, gendreBox);

    OnWidgetClick(resetButton, reset_form);
}

void BuildResultLayout(Layout *resultLayout) {
    Widget *resultBox = CreateGroupBox((Rectangle){10,10,SCREEN_WIDTH - SCREEN_WIDTH/2 - 20,SCREEN_HEIGHT - 20}, "Result");
    listView = CreateListView((Rectangle){25,25,400,550});

    AddWidget(resultLayout, resultBox);
    AddWidget(resultLayout, listView);

    OnWidgetClick(submitButton, submit_form);
}

void BuildConsoleLayout(Layout *consoleLayout) {
    Widget *consoleBox = CreateGroupBox((Rectangle){10,10,SCREEN_WIDTH/2 - 20,SCREEN_HEIGHT/2 - 20}, "Console");
    importButton = CreateButton("Import table");
    exportButton = CreateButton("Export table");
    clearButton = CreateButton("Clear table");
    exitButton = CreateButton("Exit");

    SetWidgetPos(importButton, (Vector2){(GetWidgetWidth(consoleBox) - GetWidgetWidth(importButton)) / 4, ((GetWidgetHeight(consoleBox) - GetWidgetHeight(importButton)) / 4)});
    SetWidgetPos(exportButton, (Vector2){GetWidgetWidth(consoleBox) - GetWidgetWidth(exportButton) - ((GetWidgetWidth(consoleBox) - GetWidgetWidth(exportButton)) / 4), ((GetWidgetHeight(consoleBox) - GetWidgetHeight(exportButton)) / 4)});

    SetWidgetPos(clearButton, (Vector2){(GetWidgetWidth(consoleBox) - GetWidgetWidth(clearButton)) / 4, (GetWidgetHeight(consoleBox)) - ((GetWidgetHeight(consoleBox) - GetWidgetHeight(clearButton)) / 4)});
    SetWidgetPos(exitButton, (Vector2){GetWidgetWidth(consoleBox) - GetWidgetWidth(exitButton) - ((GetWidgetWidth(consoleBox) - GetWidgetWidth(exitButton)) / 4), (GetWidgetHeight(consoleBox)) - ((GetWidgetHeight(consoleBox) - GetWidgetHeight(exitButton)) / 4)});

    AddWidget(consoleLayout, consoleBox);
    AddWidget(consoleLayout, importButton);
    AddWidget(consoleLayout, exportButton);
    AddWidget(consoleLayout, clearButton);
    AddWidget(consoleLayout, exitButton);

    OnWidgetClick(clearButton, clear_table);
    OnWidgetClick(exitButton, exit_program);
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Form Data - zraygui");

    Layout *root = CreateLayout(NULL, L_HORZ, BLANK);
    Layout *left = CreateLayout(root, L_VERT, BLANK);
        Layout *formLayout = CreateLayout(left, L_NONE, BLANK);
        Layout *consoleLayout = CreateLayout(left, L_NONE, BLANK);
    Layout *right = CreateLayout(root, L_NONE, BLANK);

    BuildFormLayout(formLayout);
    BuildResultLayout(right);
    BuildConsoleLayout(consoleLayout);
    
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(WHITE);
            RenderWindow(root);
        EndDrawing();
    }

    return 0;
}