#include "dshlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// Function to allocate memory for the cmd_buff_t structure
cmd_buff_t *alloc_cmd_buff() {
    cmd_buff_t *cmd_buff = malloc(sizeof(cmd_buff_t));
    if (!cmd_buff) {
        perror("malloc failed for cmd_buff_t");
        exit(1);
    }
    cmd_buff->argc = 0;
    cmd_buff->_cmd_buffer = NULL;
    return cmd_buff;
}

// Function to free the cmd_buff_t structure
void free_cmd_buff(cmd_buff_t *cmd_buff) {
    if (cmd_buff) {
        if (cmd_buff->_cmd_buffer) {
            free(cmd_buff->_cmd_buffer);
        }
        free(cmd_buff);
    }
}

// Function to clear the cmd_buff_t structure
void clear_cmd_buff(cmd_buff_t *cmd_buff) {
    cmd_buff->argc = 0;
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv));
    if (cmd_buff->_cmd_buffer) {
        memset(cmd_buff->_cmd_buffer, 0, strlen(cmd_buff->_cmd_buffer));
    }
}

// Function to trim leading and trailing spaces
void trim_spaces(char *str) {
    char *start = str;
    char *end = str + strlen(str) - 1;

    // Trim leading spaces
    while (*start == ' ' || *start == '\t') {
        start++;
    }

    // Trim trailing spaces
    while (*end == ' ' || *end == '\t') {
        end--;
    }

    // Null-terminate the string after trimming
    *(end + 1) = '\0';

    // Shift the string to the start
    if (start != str) {
        memmove(str, start, (end - start + 1));
    }
}

// Function to parse the command buffer and handle quoted strings
void build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    trim_spaces(cmd_line);
    char *token = strtok(cmd_line, " ");

    while (token != NULL) {
        // Check if the token is part of a quoted string
        if (token[0] == '"' || token[0] == '\'') {
            // Remove the quotes from the string
            token[strlen(token) - 1] = '\0';
            token++;
        }

        cmd_buff->argv[cmd_buff->argc++] = token;
        token = strtok(NULL, " ");
    }
}

// Function to execute the built-in 'cd' command
int exec_cd(cmd_buff_t *cmd_buff) {
    if (cmd_buff->argc == 1) {
        // No argument provided, do nothing
        return 0;
    }

    // Change directory to the directory specified in the argument
    if (chdir(cmd_buff->argv[1]) != 0) {
        perror("cd failed");
        return 1;
    }
    return 0;
}

// Function to execute a built-in command
int exec_built_in_cmd(cmd_buff_t *cmd_buff) {
    if (strcmp(cmd_buff->argv[0], "cd") == 0) {
        return exec_cd(cmd_buff);
    } else if (strcmp(cmd_buff->argv[0], "exit") == 0) {
        exit(0);  // Exit the shell
    }
    return -1;  // Not a built-in command
}

// Function to execute an external command using fork/execvp
int exec_external_cmd(cmd_buff_t *cmd_buff) {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        if (execvp(cmd_buff->argv[0], cmd_buff->argv) == -1) {
            perror("execvp failed");
            exit(1);  // Exit the child process if execvp fails
        }
    } else if (pid > 0) {
        // Parent process
        int status;
        waitpid(pid, &status, 0);  // Wait for the child process to finish
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
    } else {
        perror("fork failed");
        return 1;
    }
    return 0;
}

// Main execution loop
void exec_local_cmd_loop() {
    char *cmd_line = NULL;
    size_t len = 0;
    cmd_buff_t *cmd_buff = alloc_cmd_buff();

    while (1) {
        printf("%s", SH_PROMPT);
        if (getline(&cmd_line, &len, stdin) == -1) {
            perror("getline failed");
            break;
        }

        build_cmd_buff(cmd_line, cmd_buff);

        // Check if it's a built-in command first
        if (exec_built_in_cmd(cmd_buff) == -1) {
            // Otherwise, execute it as an external command
            exec_external_cmd(cmd_buff);
        }

        clear_cmd_buff(cmd_buff);  // Clear the cmd_buff for the next command
    }

    free(cmd_line);
    free_cmd_buff(cmd_buff);
}
