#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_HISTORY_SIZE 10
#define MAX_PATH_LENGTH 100

char commandHistory[MAX_HISTORY_SIZE][100];
int historyCount = 0;
char currentDirectory[MAX_PATH_LENGTH] = ".";

void addToHistory(const char *command) {
    if (historyCount < MAX_HISTORY_SIZE) {
        strcpy(commandHistory[historyCount], command);
        historyCount++;
    } else {
        for (int i = 0; i < MAX_HISTORY_SIZE - 1; ++i) {
            strcpy(commandHistory[i], commandHistory[i + 1]);
        }
        strcpy(commandHistory[MAX_HISTORY_SIZE - 1], command);
    }
}

void edit_file(const char *filename) {
    char buffer[1000];
    FILE *file = fopen(filename, "a+"); // Open file for appending

    if (file == NULL) {
        printf("Failed to open file.\n");
        return;
    }

    // Display file contents
    printf("File contents:\n");
    fseek(file, 0, SEEK_SET); // Move file pointer to the beginning
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    // Move file pointer to end for appending new content
    fseek(file, 0, SEEK_END);

    // Prompt user for input
    printf("\n--- Start typing (type 'exit' on a new line to exit) ---\n");
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // Check for "exit" command
        if (strcmp(buffer, "exit\n") == 0) {
            printf("Exiting text editor...\n");
            break; // Exit the loop and the text editor
        }
        
        // Write input to file
        fputs(buffer, file);
    }

    // Close file
    fclose(file);
}

void view_file(const char *filename) {
    char buffer[1000];
    FILE *file = fopen(filename, "r"); // Open file for reading

    if (file == NULL) {
        printf("Failed to open file.No such file.\n");
        return;
    }

    // Display file contents
    printf("File contents:\n");
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    // Close file
    fclose(file);
}

void clear_screen() {
    system("cls");
}

void display_history() {
    printf("Command history:\n");
    for (int i = 0; i < historyCount; ++i) {
        printf("%d: %s\n", i + 1, commandHistory[i]);
    }
}

void make_directory(const char *dirname) {
    char path[MAX_PATH_LENGTH];
    snprintf(path, sizeof(path), "%s/%s", currentDirectory, dirname);
    if (mkdir(path) == 0) {
        printf("Directory '%s' created successfully.\n", dirname);
    } else {
        printf("Failed to create directory '%s'.\n", dirname);
    }
}

void change_directory(const char *dirname) {
    char path[MAX_PATH_LENGTH];
    snprintf(path, sizeof(path), "%s/%s", currentDirectory, dirname);
    if (chdir(path) == 0) {
        strcpy(currentDirectory, path);
        printf("Changed directory to '%s'.\n", currentDirectory);
    } else {
        printf("Failed to change directory to '%s'.\n", dirname);
    }
}

int main() {
    char cmd[100];
    char filename[100];
    char input[100];

    // Print header title
    printf("HitlerOS version 0.1. Copyright claimed\n");

    // Main loop
    while (1) {
        printf("%s $ ", currentDirectory); // Print current directory
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%s", cmd);
        addToHistory(cmd);

        if (strcmp(cmd, "a") == 0) { // About
            printf("This is an experimental operating system\n");
        } else if (strcmp(cmd, "c") == 0) { // Cat
            printf("HitlerOS version: v0.1 Publisher: MD Oaes Made in Bangladesh\n");
        } else if (strcmp(cmd, "d") == 0) { // Date
            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            printf("Current date and time: %s", asctime(tm));
        } else if (strcmp(cmd, "e") == 0) { // Edit
            printf("Enter filename: ");
            scanf("%s", filename);
            getchar(); // Clear the newline character
            edit_file(filename);
        } else if (strcmp(cmd, "h") == 0) { // Help
            printf("List of available commands:\n");
            printf("a - About\n");
            printf("c - Cat\n");
            printf("d - Date\n");
            printf("e - Edit\n");
            printf("h - Help\n");
            printf("q - Quit\n");
            printf("s - View file contents\n");
            printf("cls - Clear screen\n");
            printf("histo - Command history\n");
            printf("echo - Echo text\n");
            printf("mkdirec - Make directory\n");
            printf("cd - Change directory\n");
            printf("calc - Calculator\n");
        } else if (strcmp(cmd, "q") == 0) { // Quit
            printf("Exiting...\n");
            return 0;
        } else if (strcmp(cmd, "s") == 0) { // View file contents
            printf("Enter filename: ");
            scanf("%s", filename);
            getchar(); // Clear the newline character
            view_file(filename);
        } else if (strcmp(cmd, "cls") == 0) { // Clear screen
            clear_screen();
        } else if (strcmp(cmd, "histo") == 0) { // Command history
            display_history();
        } else if (strcmp(cmd, "echo") == 0) { // Echo text
            printf("%s\n", input + strlen(cmd) + 1);
        } else if (strcmp(cmd, "mkdirec") == 0) { // Make directory
            printf("Enter directory name: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0; // Remove newline character
            make_directory(filename);
        } else if (strcmp(cmd, "cd") == 0) { // Change directory
            printf("Enter directory name: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0; // Remove newline character
            change_directory(filename);
        } else if (strcmp(cmd, "calc") == 0) {
            char expression[100];
            double result;
            char choice;

            do {
                printf("Enter expression: ");
                fgets(input, sizeof(input), stdin);
                sscanf(input, "%[^\n]", expression);

                // Evaluate expression based on operator
                double num1, num2;
                char op;
                sscanf(expression, "%lf %c %lf", &num1, &op, &num2);
                switch(op) {
                    case '+':
                        result = num1 + num2;
                        break;
                    case '-':
                        result = num1 - num2;
                        break;
                    case '*':
                        result = num1 * num2;
                        break;
                    case '/':
                        if(num2 != 0)
                            result = num1 / num2;
                        else {
                            printf("That is not possible by any computer, human or os.\n");
                            continue; // Skip to the next iteration
                        }
                        break;
                    default:
                        printf("Invalid operator.\n");
                        continue; // Skip to the next iteration
                }
                
                printf("Result: %lf\n", result);

                printf("Do you want to calculate again? (y/n): ");
                scanf(" %c", &choice);
                getchar();
            } while (choice == 'y');
        } else {
            printf("No such command in my OS. type 'h' for list of commands.\n");
        }
    }
    return 0;
}
