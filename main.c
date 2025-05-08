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

void	ft_echo(char *input)
{
	int	i;
	
	i = 5;
	// while (args[2][i])
	// {
	// 	if (args[2][i] == '-' && args[2][i + 1] == 'n')
	// }
	while (input[i])
	{
		// if (input[i] ==  '-' && input[i + 1] == 'n')
		// 	printf("%s", input[i]);
		// else
		printf("%c", input[i]);
		i++;
	}
	printf("\n");
}

int	main(int argc, char *argv[], char **env)
{
	char	*input;
	int		i;

	(void)argc;
	(void)argv;
	t_env *my_env = env_init(env);
	while (1)
	{
		i = 0;
		input = readline("marvel$ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (!strcmp(input, "env"))
			print_env(my_env);
		if (!strncmp(input, "echo", 4))
			ft_echo(input);
		if (input)
			add_history(input);
	}
	free(input);
	return (0);
}
