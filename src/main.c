#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/history.h>
#include <readline/readline.h>
#include "history.h"

int main(void)
{
    // Display welcome banner
    printf("=====================================\n");
    printf("          Shellforge\n");
    printf("    A Unix Style Shell written in C\n");
    printf("=====================================\n");

    // Initialize Readline history
    using_history();

    char *line;

    while (1)
    {
        // Read command from user
        line = readline("shellforge$ ");

        // Ctrl+D / EOF
        if (line == NULL)
        {
            printf("\nGoodbye!\n");
            break;
        }

        // Ignore empty commands
        if (strlen(line) == 0)
        {
            free(line);
            continue;
        }

        // Add command to history
        add_history(line);

        // Display entered command
        printf("YOU ENTERED : %s\n", line);

        // History command
        if (strcmp(line, "history") == 0)
        {
            print_history();
        }

        // Exit command
        else if (strcmp(line, "exit") == 0)
        {
            free(line);
            printf("Goodbye!\n");
            break;
        }

        // Free memory
        free(line);
    }

    return 0;
}
