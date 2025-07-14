/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:15:16 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/07/10 13:05:27 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_last_exit_status = 0;

static void	handle_builtin_execution(t_gc *gc, t_command *commands,
		t_env **my_env)
{
	handle_redirection(commands, -1);
	builtins(gc, commands->args, my_env);
	dup2(commands->saved_stdin, STDIN_FILENO);
	dup2(commands->saved_stdout, STDOUT_FILENO);
	close(commands->saved_stdin);
	close(commands->saved_stdout);
}

void print_args(t_command *commands)
{
    t_command *current = commands;
    
    while (current != NULL)
    {
        if (current->args != NULL)
        {
            for (int i = 0; current->args[i] != NULL; i++)
            {
                printf("%s", current->args[i]);  // No spaces between args
            }
            printf("\n");
        }
        current = current->next;
    }
}

static void	process_input(t_gc *gc, char *input, t_env *my_env)
{
	t_token		*tokens;
	t_command	*commands;

	add_history(input);
	tokens = tokenize(input, gc);
	if (!check_syntax(tokens))
		return ;
	expander(gc, tokens, my_env);
	quote_management(gc, tokens);
	commands = parse_tokens(gc, tokens);
	if (process_heredocs(gc, commands, my_env) == -1)
	{
		g_last_exit_status = 130;
		return ;
	}
	if (commands->next)
		execute_pipe(gc, commands, my_env);
	else if (commands->args && is_builtin(commands->args[0]))
		handle_builtin_execution(gc, commands, &my_env);
	else
		shell(gc, commands, my_env);
}

static void	handle_eof(t_gc *gc)
{
	printf("exit\n");
	gc_clear(gc);
	g_last_exit_status = 0;
	exit(0);
}

int	main(int ac, char *av[], char **env)
{
	t_gc	gc;
	char	*input;
	t_env	*my_env;

	ft_memset(&gc, 0, sizeof(gc));
	my_env = env_init(env, &gc);
	(void)av;
	(void)ac;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		input = readline("marvel$ ");
		if (!input)
			handle_eof(&gc);
		process_input(&gc, input, my_env);
		free(input);
	}
	gc_clear(&gc);
	return (0);
}
