#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"

int is_builtin(command_t *cmd)
{
    if (cmd == NULL || cmd->argc == 0)
        return 0;

    if (strcmp(cmd->argv[0], "cd") == 0)
        return 1;

    if (strcmp(cmd->argv[0], "pwd") == 0)
        return 1;

    if (strcmp(cmd->argv[0], "echo") == 0)
        return 1;

    if (strcmp(cmd->argv[0], "exit") == 0)
        return 1;

    return 0;
}


int execute_builtin(command_t *cmd)
{
    if (cmd == NULL || cmd->argc == 0)
        return -1;


    /* cd */
    if (strcmp(cmd->argv[0], "cd") == 0)
    {
        const char *path;

        if (cmd->argc >= 2)
            path = cmd->argv[1];
        else
            path = getenv("HOME");

        if (path == NULL)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return -1;
        }

        if (chdir(path) != 0)
        {
            perror("cd");
            return -1;
        }

        return 0;
    }


    /* pwd */
    if (strcmp(cmd->argv[0], "pwd") == 0)
    {
        char cwd[4096];

        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("pwd");
            return -1;
        }

        printf("%s\n", cwd);

        return 0;
    }


    /* echo */
    if (strcmp(cmd->argv[0], "echo") == 0)
    {
        for (int i = 1; i < cmd->argc; i++)
        {
            printf("%s", cmd->argv[i]);

            if (i < cmd->argc - 1)
                printf(" ");
        }

        printf("\n");

        return 0;
    }


    /* exit */
    if (strcmp(cmd->argv[0], "exit") == 0)
    {
        exit(0);
    }


    return -1;
}
