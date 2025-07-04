/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:15:16 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/06/28 07:38:11 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_last_exit_status = 0;

int is_builtin(char *cmd)
{
    return (!strncmp(cmd, "echo", 4)
        || !strncmp(cmd, "cd", 2)
        || !strncmp(cmd, "pwd", 3)
        || !strncmp(cmd, "export", 6)
        || !strncmp(cmd, "unset", 5)
        || !strncmp(cmd, "env", 3)
        || !strncmp(cmd, "exit", 4));
}

void builtins(t_gc *gc, char **args, t_env *env)
{
    if (!ft_strcmp(args[0], "echo"))
        ft_echo(args);
    else if (!ft_strcmp(args[0], "cd"))
        ft_cd(gc, args, env);
    else if (!ft_strcmp(args[0], "env"))
        print_env(env);
    else if (!ft_strcmp(args[0], "pwd"))
        ft_pwd(args, env);
    else if (!ft_strcmp(args[0], "unset"))
        ft_unset(args, env);
    else if (!ft_strcmp(args[0], "export"))
        ft_export(gc, args, env);
    else if (!ft_strcmp(args[0], "exit"))
        ft_exit(args);
}

int main(int ac, char *av[], char **env)
{
    t_gc gc = {0};
    char *input;
    t_env *my_env = env_init(env, &gc);

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
            g_last_exit_status = 0;
            exit(0);
        }
        add_history(input);
        t_token *tokens = tokenize(input, &gc);
        quote_management(&gc, tokens);
        if (!check_syntax(tokens))
        {
            free(input);
            continue;
        }
        expander(&gc, tokens, my_env);
        t_command *commands = parse_tokens(&gc, tokens);
        if (process_heredocs(&gc, commands, my_env) == -1)
        {
            g_last_exit_status = 130;
            free(input);
            continue;
        }
        if (commands->next)
            execute_pipe(&gc, commands, my_env);
        else if (commands->args && is_builtin(commands->args[0]))
        {
            handle_redirection(commands, -1);
            builtins(&gc, commands->args, my_env);
            dup2(commands->saved_stdin, STDIN_FILENO);
            dup2(commands->saved_stdout, STDOUT_FILENO);
            close(commands->saved_stdin);
            close(commands->saved_stdout);
        }
        else
            shell(&gc, commands, my_env);
        free(input);
    }
    gc_clear(&gc);
    return (0);
}
