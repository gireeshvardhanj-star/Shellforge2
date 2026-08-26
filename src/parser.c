#include <stdio.h>
#include <string.h>

#include "parser.h"

int parser(token_list_t *tokens, pipeline_t *pipeline)
{
    if (tokens == NULL || pipeline == NULL)
        return -1;

    memset(pipeline, 0, sizeof(pipeline_t));

    if (tokens->count == 0)
        return -1;

    int command_index = 0;
    command_t *cmd = &pipeline->commands[command_index];

    for (int i = 0; i < tokens->count; i++)
    {
        token_t *token = &tokens->tokens[i];

        switch (token->type)
        {
            case TOKEN_WORD:
                if (cmd->argc >= MAX_ARGS - 1)
                {
                    fprintf(stderr, "Parser Error: too many arguments\n");
                    return -1;
                }

                cmd->argv[cmd->argc] = token->text;
                cmd->argc++;

                break;

            case TOKEN_INPUT:
                if (i + 1 >= tokens->count ||
                    tokens->tokens[i + 1].type != TOKEN_WORD)
                {
                    fprintf(stderr,
                            "Parser Error: expected input filename\n");
                    return -1;
                }

                strncpy(cmd->input,
                        tokens->tokens[++i].text,
                        MAX_FILENAME - 1);

                cmd->input[MAX_FILENAME - 1] = '\0';

                break;

            case TOKEN_OUTPUT:
                if (i + 1 >= tokens->count ||
                    tokens->tokens[i + 1].type != TOKEN_WORD)
                {
                    fprintf(stderr,
                            "Parser Error: expected output filename\n");
                    return -1;
                }

                strncpy(cmd->output,
                        tokens->tokens[++i].text,
                        MAX_FILENAME - 1);

                cmd->output[MAX_FILENAME - 1] = '\0';
                cmd->append = 0;

                break;

            case TOKEN_APPEND:
                if (i + 1 >= tokens->count ||
                    tokens->tokens[i + 1].type != TOKEN_WORD)
                {
                    fprintf(stderr,
                            "Parser Error: expected output filename\n");
                    return -1;
                }

                strncpy(cmd->output,
                        tokens->tokens[++i].text,
                        MAX_FILENAME - 1);

                cmd->output[MAX_FILENAME - 1] = '\0';
                cmd->append = 1;

                break;

            case TOKEN_BACKGROUND:
                cmd->background = 1;
                break;

            case TOKEN_PIPE:
                if (cmd->argc == 0)
                {
                    fprintf(stderr,
                            "Parser Error: empty command before pipe\n");
                    return -1;
                }

                if (command_index >= MAX_COMMANDS - 1)
                {
                    fprintf(stderr,
                            "Parser Error: too many commands\n");
                    return -1;
                }

                command_index++;

                cmd = &pipeline->commands[command_index];

                break;

            case TOKEN_END:
                i = tokens->count;
                break;

            default:
                fprintf(stderr, "Parser Error: unknown token\n");
                return -1;
        }
    }

    if (cmd->argc == 0)
    {
        if (command_index > 0)
        {
            fprintf(stderr,
                    "Parser Error: empty command after pipe\n");
            return -1;
        }

        return -1;
    }

    pipeline->command_count = command_index + 1;

    return 0;
}


void pipeline_print(const pipeline_t *pipeline)
{
    if (pipeline == NULL)
        return;

    printf("\n========== PIPELINE ==========\n");

    printf("Commands: %d\n", pipeline->command_count);

    for (int i = 0; i < pipeline->command_count; i++)
    {
        const command_t *cmd = &pipeline->commands[i];

        printf("\nCommand %d\n", i + 1);
        printf("argc       : %d\n", cmd->argc);

        printf("argv       : ");

        for (int j = 0; j < cmd->argc; j++)
        {
            printf("[%s] ", cmd->argv[j]);
        }

        printf("\n");

        printf("input      : %s\n",
               cmd->input[0] ? cmd->input : "(none)");

        printf("output     : %s\n",
               cmd->output[0] ? cmd->output : "(none)");

        printf("append     : %d\n", cmd->append);
        printf("background : %d\n", cmd->background);
    }

    printf("==============================\n");
}
