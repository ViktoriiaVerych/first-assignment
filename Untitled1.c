#include <stdio.h>
#include <stdlib.h>
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

int main() {
    int command;

    while (1) {
        printf("Choose the command:\n");
        printf("1| Append text symbols to the end\n");
        printf("2| Start a new line\n");
        printf("3| Use files to saving the information\n");
        printf("4| Use files to loading the information\n");
        printf("5| Print the current text to console\n");
        printf("6| Insert text by line and symbol index\n");
        printf("7| Search for a substring\n");
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
                printf("");
                break;

            case 4:
                printf("");
                break;

            case 5:
                printf("");
                break;

            case 6:
                printf("");
                break;

            case 7:
                printf("");
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

