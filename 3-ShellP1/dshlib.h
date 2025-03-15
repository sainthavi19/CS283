#ifndef DSHLIB_H
#define DSHLIB_H

#include <stdlib.h>
#include <string.h>

#define SH_CMD_MAX 1024
#define CMD_MAX 16
#define EXE_MAX 128
#define ARG_MAX 512
#define PIPE_STRING "|"
#define EXIT_CMD "exit"
#define SH_PROMPT "dsh> "
#define OK 0
#define WARN_NO_CMDS 1
#define ERR_TOO_MANY_COMMANDS 2
#define ERR_CMD_OR_ARGS_TOO_BIG 3

typedef struct {
    char exe[EXE_MAX];
    char *args[ARG_MAX];
} command_t;

typedef struct {
    command_t commands[CMD_MAX];
    int num;
} command_list_t;

int build_cmd_list(char *cmd_line, command_list_t *clist);
void print_cmd_list(command_list_t *clist);

#endif
