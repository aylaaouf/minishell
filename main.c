/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:15:16 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/06/24 22:52:50 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     is_builtin(char *cmd)
{
    return (!strncmp(cmd, "echo", 4)
        || !strncmp(cmd, "cd", 2)
        || !strncmp(cmd, "pwd", 3)
        || !strncmp(cmd, "export", 6)
        || !strncmp(cmd, "unset", 5)
        || !strncmp(cmd, "env", 3)
        || !strncmp(cmd, "exit", 4));
}

void builtins(char *input, t_env *env, int last_exit_status)
{
    if (!strncmp(input, "echo", 4))
        ft_echo(input, env, last_exit_status);
    else if (!strncmp(input, "cd", 2))
        ft_cd(input, env);
    else if (!strncmp(input, "env", 3))
        print_env(env);
    else if (!strncmp(input, "pwd", 3))
        ft_pwd(input, env);
    else if (!strncmp(input, "unset", 5))
        ft_unset(input, env);
    else if (!strncmp(input, "export", 6))
        ft_export(input, env);
    else if (!strncmp(input, "exit", 4))
        ft_exit(input);
}

int main(int ac, char *av[], char **env)
{
    char *input;
	t_env *my_env = env_init(env);
	int last_exit_status = 0;
    (void)av;
	(void)ac;
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
    while (1)
    {
        input = readline("marvel$ ");
        if (!input)
        {
            printf("exit\n");
            last_exit_status = 0;
            exit(0);
        }
        add_history(input);
        t_token *tokens = tokenize(input);
        quote_management(tokens);
        if (!check_syntax(tokens))
        {
            free(input);
            continue;
		}
        t_command *commands = parse_tokens(tokens);
        process_heredocs(commands, my_env, last_exit_status);
		expander(tokens, my_env, last_exit_status);
        if (commands->next)
            execute_pipe(commands, my_env);
        else if (commands->args && is_builtin(commands->args[0]))
            builtins(input, my_env, last_exit_status);
        else
            shell(commands, my_env);

        free(input);
    }

    gc_clear();
    return 0;
}

