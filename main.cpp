#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXT_SIZE 1000

typedef struct Line {
    char* data;
    struct Line* next;
} Line;

// creates a new line of text
Line* createLine() {
    Line* newLine = (Line*)malloc(sizeof(Line));
    newLine->data = (char*)malloc(MAX_TEXT_SIZE);
    newLine->data[0] = '\0';
    newLine->next = NULL;
    return newLine;
}

// appends text to the current line
void appendText(Line* currentLine, const char* text) {
    strcat(currentLine->data, text);
}

// starts a new linne
Line* startNewLine(Line* currentLine) {
    Line* newLine = createLine();
    currentLine->next = newLine;
    return newLine;
}

// prints the text
void printText(Line* text) {
    Line* current = text;
    while (current != NULL) {
        printf("%s\n", current->data);
        current = current->next;
    }
}

//saves the tsxt
void saveTextToFile(Line* text, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file != NULL) {
        Line* current = text;
        while (current != NULL) {
            fputs(current->data, file);
            fputc('\n', file); // Add a newline after each line
            current = current->next;
        }
        fclose(file);
        printf("Text has been saved successfully\n");
    } else {
        printf("Error opening file for saving\n");
    }
}

// serxhes in the text
void searchInText(Line* text, const char* substring) {
    int lineIndex = 0;
    Line* current = text;
    while (current != NULL) {
        char* position = strstr(current->data, substring);
        while (position != NULL) {
            int symbolIndex = position - current->data;
            printf("Text is present in this position: %d %d\n", lineIndex, symbolIndex);
            position = strstr(position + 1, substring);
        }
        current = current->next;
        lineIndex++;
    }
}

// Function to insert text at a specific line and symbol index
void insertText(Line* text, int line, int index, const char* insertText) {
    Line* current = text;
    for (int i = 0; i < line; i++) {
        if (current->next == NULL) {
            printf("Line index %d not found\n", line);
            return;
        }
        current = current->next;
    }
    if (index < 0 || index > strlen(current->data)) {
        printf("Invalid symbol index %d\n", index);
        return;
    }
    char* newString = (char*)malloc(strlen(current->data) + strlen(insertText) + 1);
    strncpy(newString, current->data, index);
    newString[index] = '\0';
    strcat(newString, insertText);
    strcat(newString, current->data + index);
    strcpy(current->data, newString);
    free(newString);
}



int main() {
    int command;

    while (1) {
        printf("Choose the command:\n");
        printf("1| Enter text to append:\n");
        printf("2| Start a new line:\n");
        printf("3| Use files to saving the information\n");
        printf("4| Use files to loading the information\n");
        printf("5| Print the current text:\n");
        printf("6| Insert text by line and symbol index\n");
        printf("7| Enter text to search:\n");
        printf("8| Clear the console\n");
        printf("9| Exit\n");

        scanf("%d", &command);

        switch (command) {
            case 1:
                printf(" Enter text to append: \n");
                break;

            case 2:
                printf("New line is started\n");
                break;

            case 3:
                printf("Enter the file name for saving: ");
                char saveFileName[MAX_TEXT_SIZE];
                scanf(" %[^\n]", saveFileName);
                saveTextToFile(text, saveFileName);
                break;
            case 4:
                printf("");
                break;

            case 5:
                printf("Current text:\n");
                printText(text);
                break;

            case 6:
                printf("Type in format -- LINE INDEX: ");
                int line, index;
                scanf("%d %d", &line, &index);
                printf("Enter text to insert: ");
                char insertBuffer[MAX_TEXT_SIZE];
                scanf(" %[^\n]", insertBuffer);
                insertText(text, line, index, insertBuffer);
                break;


            case 7:
                printf("Enter text to search: ");
                char searchString[MAX_TEXT_SIZE];
                scanf(" %[^\n]", searchString);
                searchInText(text, searchString);
                break;


            case 8:
                printf("Console cleared\n");
                break;

            case 9:
                exit(0);

            default:
                printf("The command is not implemented.\n");
                break;
        }

    }

    return 0;
}
