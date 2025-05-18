/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:15:16 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/05/18 19:11:07 by aylaaouf         ###   ########.fr       */
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

void    builtins(char *input,t_env *env)
{
    if (!strncmp(input, "echo", 4))
        ft_echo(input, env);
    else if (!strncmp(input, "cd", 2))
        ft_cd(input, env);
    else if (!strncmp(input, "env", 3))
        print_env(env);
    else if (!strncmp(input, "pwd", 3))
        ft_pwd(input, env);
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
		if (!check_syntax(tokens))
        {
            free(input);
            continue;
        }
        t_command *commands = parse_tokens(tokens);
        if (is_builtin(commands->args[0]))
            builtins(input, my_env);
        else
            shell(commands, my_env);
        free(input);
    }
    gc_clear();
    return 0;
}

