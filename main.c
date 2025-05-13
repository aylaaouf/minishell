/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:15:16 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/05/12 19:25:44 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void print_commands(t_command *cmd)
// {
//     int cmd_num = 1;
//     while (cmd)
//     {
//         printf("Command %d:\n", cmd_num++);
//         if (cmd->args)
//         {
//             for (int i = 0; cmd->args[i]; i++)
//                 printf("  [CMD] %s\n", cmd->args[i]);
//         }

//         if (cmd->redir)
//         {
//             t_redirection *r = cmd->redir;
//             while (r)
//             {
//                 printf("  [REDIR] %s %s\n", r->type, r->file);
//                 r = r->next;
//             }
//         }

//         cmd = cmd->next;
//         if (cmd) printf("  [PIPE]\n");
//     }
// }

void    builtins(char *input,t_env *env)
{
    if (!strncmp(input, "echo", 4))
        ft_echo(input, env);
}

int main(int ac, char *av[], char **env)
{
    char *input;
    t_env *my_env = env_init(env);

    (void)av;
	(void)ac;
    while (1)
    {
        input = readline("marvel$ ");
        if (!input)
        {
            printf("exit\n");
            break;
        }
        add_history(input);
        t_token *tokens = tokenize(input);
        quote_management(tokens);
        expander(tokens, my_env);
        builtins(input, my_env);
		if (!check_syntax(tokens))
        {
            free(input);
            continue;
        }
        // t_command *commands = parse_tokens(tokens);
		// print_commands(commands);
        free(input);
    }
    gc_clear();
    return 0;
}

