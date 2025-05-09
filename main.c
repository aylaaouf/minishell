/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:15:16 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/05/08 19:16:58 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*void	ft_echo(char *input)*/
/*{*/
/*	int	i;*/
/**/
/*	i = 5;*/
/*	// while (args[2][i])*/
/*	// {*/
/*	// 	if (args[2][i] == '-' && args[2][i + 1] == 'n')*/
/*	// }*/
/*	while (input[i])*/
/*	{*/
/*		// if (input[i] ==  '-' && input[i + 1] == 'n')*/
/*		// 	printf("%s", input[i]);*/
/*		// else*/
/*		printf("%c", input[i]);*/
/*		i++;*/
/*	}*/
/*	printf("\n");*/
/*}*/
/**/
/*int	main(int argc, char *argv[], char **env)*/
/*{*/
/*	char	*input;*/
/*	int		i;*/
/**/
/*	(void)argc;*/
/*	(void)argv;*/
/*	t_env *my_env = env_init(env);*/
/*	while (1)*/
/*	{*/
/*		i = 0;*/
/*		input = readline("marvel$ ");*/
/*		if (!input)*/
/*		{*/
/*			printf("exit\n");*/
/*			break ;*/
/*		}*/
/*		if (!strcmp(input, "env"))*/
/*			print_env(my_env);*/
/*		if (!strncmp(input, "echo", 4))*/
/*			ft_echo(input);*/
/*		if (input)*/
/*			add_history(input);*/
/*	}*/
/*	free(input);*/
/*	return (0);*/
/*}*/
void print_commands(t_command *cmd)
{
    int cmd_num = 1;
    while (cmd)
    {
        printf("Command %d:\n", cmd_num++);
        if (cmd->args)
        {
            for (int i = 0; cmd->args[i]; i++)
                printf("  [CMD] %s\n", cmd->args[i]);
        }

        if (cmd->redir)
        {
            t_redirection *r = cmd->redir;
            while (r)
            {
                printf("  [REDIR] %s %s\n", r->type, r->file);
                r = r->next;
            }
        }

        cmd = cmd->next;
        if (cmd) printf("  [PIPE]\n");
    }
}

// Mock tokens for testing
int main(void)
{
    // Create tokens manually
    t_token tokens[8];

    tokens[0] = (t_token){TOKEN_WORD, "echo", &tokens[1]};
    tokens[1] = (t_token){TOKEN_WORD, "hello", &tokens[2]};
    tokens[2] = (t_token){TOKEN_OUTPUT, ">", &tokens[3]};
    tokens[3] = (t_token){TOKEN_WORD, "file.txt", &tokens[4]};
    tokens[4] = (t_token){TOKEN_PIPE, "|", &tokens[5]};
    tokens[5] = (t_token){TOKEN_WORD, "cat", &tokens[6]};
    tokens[6] = (t_token){TOKEN_INPUT, "<", &tokens[7]};
    tokens[7] = (t_token){TOKEN_WORD, "file.txt", NULL};

    // Parse tokens into commands
    t_command *commands = parse_tokens(tokens);

    // Print the parsed commands
    print_commands(commands);

    return 0;
}
