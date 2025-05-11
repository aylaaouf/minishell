/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:15:16 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/05/11 11:07:59 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int main(int ac, char *av[], char **env)
{
    char *input;
    t_env *my_env = env_init(env);

	(void)ac;
	(void)av;
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
		if (!check_syntax(tokens))
        {
            free(input);
            continue;
        }
        t_command *commands = parse_tokens(tokens);
		print_commands(commands);
        free(input);
    }
    gc_clear();
    return 0;
}

