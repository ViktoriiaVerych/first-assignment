#include <stdio.h>
#include <stdlib.h>

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



