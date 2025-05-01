/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:15:16 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/05/01 12:14:57 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char *argv[], char **env)
{
	char	*input;
	char	**envp;
	int		i;

	(void)argc;
	(void)argv;
	envp = get_env(env);
	while (1)
	{
		i = 0;
		input = readline("marvel$ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (input)
			add_history(input);
		while (env[i] != NULL)
		{
			printf("%s\n", envp[i]);
			i++;
		}
	}
	/*free(input);*/
	return (0);
}
