#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dshlib.h"


void trim_spaces(char *str) {
    char *end;

    while (*str == ' ') str++;

    end = str + strlen(str) - 1;
    while (end > str && *end == ' ') end--;

    *(end + 1) = '\0';
}

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    int num_cmds = 0;
    char *cmd = strtok(cmd_line, PIPE_STRING); // Split by pipe "|"
    for (int i = 0; i < CMD_MAX; i++) {
        clist->commands[i].exe[0] = '\0';  // Reset executable name
        for (int j = 0; j < ARG_MAX; j++) {
            clist->commands[i].args[j] = NULL; // Reset arguments (char * initialized to NULL)
        }
    }

    while (cmd != NULL) {

        char *exe = strtok(cmd, " ");
        if (exe == NULL) {
            cmd = strtok(NULL, PIPE_STRING);
            continue;
        }

        strncpy(clist->commands[num_cmds].exe, exe, EXE_MAX - 1);
        clist->commands[num_cmds].exe[EXE_MAX - 1] = '\0'; // Ensure null termination

        char *arg = strtok(NULL, " ");
        int arg_num = 0;

        while (arg != NULL) {
            clist->commands[num_cmds].args[arg_num++] = arg;
            arg = strtok(NULL, " ");
        }

        clist->commands[num_cmds].args[arg_num] = NULL;

        clist->num = ++num_cmds;
        cmd = strtok(NULL, PIPE_STRING); 
    }

    if (num_cmds > CMD_MAX) {
        return ERR_TOO_MANY_COMMANDS;
    }

    return OK;
}

void print_cmd_list(command_list_t *clist) {
    printf("PARSED COMMAND LINE - TOTAL COMMANDS %d\n", clist->num);
    for (int i = 0; i < clist->num; i++) {
        printf("<%d> %s", i + 1, clist->commands[i].exe);
        int j = 0;
        while (clist->commands[i].args[j] != NULL) {
            printf(" %s", clist->commands[i].args[j]);
            j++;
        }
        printf("\n");
    }
}
