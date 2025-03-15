#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dshlib.h"

int main() {
    command_list_t clist = {0};  // Initialize command list
    char cmd_line[256];          // Buffer for command line input

    while (1) {
        // Get command line input from the user
        printf("dsh> ");
        if (fgets(cmd_line, sizeof(cmd_line), stdin) == NULL) {
            continue;  // Exit on EOF
        }
        cmd_line[strcspn(cmd_line, "\n")] = '\0';  // Remove trailing newline

        // Exit the shell if the user enters "exit"
        if (strcmp(cmd_line, "exit") == 0) {
            printf("Exiting shell...\n");
            break;
        }

        // Prevent processing of empty command
        if (strlen(cmd_line) == 0) {
            continue;
        }

        // Parse the command line into the command list
        if (build_cmd_list(cmd_line, &clist) != OK) {
            printf("Error: Too many commands or invalid input.\n");
            continue;  // Continue without exiting
        }

        // Print the parsed command list
        print_cmd_list(&clist);
    }

    return 0;
}