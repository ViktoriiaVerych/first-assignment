#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10000

typedef struct Line {
    char* data;
    struct Line* next;
} Line;


//creates a new line of text
Line* createLine() {
    Line* newLine = (Line*)malloc(sizeof(Line));
    newLine->data = (char*)malloc(MAX_SIZE);
    newLine->data[0] = '\0';
    newLine->next = NULL;
    return newLine;
}

//appends text
void appendText(Line* currentLine, const char* text) {
    strcat(currentLine->data, text);
}

// starts a new linne
Line* startNewLine(Line* currentLine) {
    Line* newLine = createLine();
    currentLine->next = newLine;
    return newLine;
}

// prints
void print(Line* text) {
    Line* current = text;
    while (current != NULL) {
        printf("%s\n", current->data);
        current = current->next;
    }
}

// saves the tsxt
void saveTextToFile(Line* text, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file != NULL) {
        Line* current = text;
        while (current != NULL) {
            fputs(current->data, file);
            current = current->next;
        }
        fclose(file);
        printf("Text has been saved\n");
    } else {
        printf("Error\n");
    }
}

//loads the text
Line* loadTextFromFile(const char* filename) {
    Line* text = createLine();
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error\n");
        return text;
    }
    char buffer[MAX_SIZE];
    while (fgets(buffer, MAX_SIZE, file) != NULL) {
        appendText(text, buffer);
    }
    fclose(file);
    printf("Text has been loaded\n");
    return text;
}

//serxhes in the text
void searchInText(Line* text, const char* substring) {
    int lineIndex = 0;
    Line* current = text;
    while (current != NULL) {
        char* position = strstr(current->data, substring);
        while (position != NULL) {
            int symbolIndex = position - current->data;
            printf("Position: %d %d\n", lineIndex, symbolIndex);
            position = strstr(position + 1, substring);
        }
        current = current->next;
        lineIndex++;
    }
}

// inserts
void insertText(Line* text, int lineIndex, int symbolIndex, const char* insertText) {
    Line* current = text;
    for (int i = 0; i < lineIndex; i++) {
        if (current->next == NULL) {
            printf("Line index %d not found\n", lineIndex);
            return;
        }
        current = current->next;
    }
    if (symbolIndex < 0 || symbolIndex > strlen(current->data)) {
        printf("Invalid symbol index %d\n", symbolIndex);
        return;
    }
    char* newText = (char*)malloc(strlen(current->data) + strlen(insertText) + 1);
    strncpy(newText, current->data, symbolIndex);
    newText[symbolIndex] = '\0';
    strcat(newText, insertText);
    strcat(newText, current->data + symbolIndex);
    strcpy(current->data, newText);
    free(newText);
}

int main() {
    int command;
    Line* text = createLine();

    while (1) {
        printf("Choose the command:\n");
        printf("1| Append text symbols to the end\n");
        printf("2| Start the new line\n");
        printf("3| Use files to saving the information\n");
        printf("4| Use files to loading the information\n");
        printf("5| Print the current text to console\n");
        printf("6| Insert the text by line and symbol index\n");
        printf("7| Search\n");
        printf("8| Clear the console\n");

        scanf("%d", &command);

        switch (command) {
            case 1:
                printf("Enter text to append: ");
                char appendBuffer[MAX_SIZE];
                scanf(" %[^\n]", appendBuffer);
                appendText(text, appendBuffer);
                break;

            case 2:
                text = startNewLine(text);
                printf("New line is started\n");
                break;

            case 3:
                printf("Enter the file name for saving: ");
                char saveFileName[MAX_SIZE];
                scanf(" %[^\n]", saveFileName);
                saveTextToFile(text, saveFileName);
                break;

            case 4:
                printf("Enter the file name for loading: ");
                char loadFileName[MAX_SIZE];
                scanf(" %[^\n]", loadFileName);
                free(text);  // free the current text
                text = loadTextFromFile(loadFileName);
                break;

            case 5:
                printf("Current text:\n");
                print(text);
                break;

            case 6:
                printf("Choose line and index: ");
                int lineIndex, symbolIndex;
                scanf("%d %d", &lineIndex, &symbolIndex);
                printf("Enter text to insert: ");
                char insertBuffer[MAX_SIZE];
                scanf(" %[^\n]", insertBuffer);
                insertText(text, lineIndex, symbolIndex, insertBuffer);
                break;

            case 7:
                printf("Enter text to search: ");
                char search[MAX_SIZE];
                scanf(" %[^\n]", search);
                searchInText(text, search);
                break;

            case 8:
                system("clear");
                break;

            default:
                printf("The command is not implemented.\n");
                break;
        }
    }

    return 0;
}
