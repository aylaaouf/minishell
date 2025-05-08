/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:15:16 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/05/07 16:46:46 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (input)
			add_history(input);
	}
	free(input);
	return (0);
}
