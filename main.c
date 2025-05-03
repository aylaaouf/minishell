/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:15:16 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/05/01 12:37:27 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_env(t_env *env)
{
    while (env)
    {
        if (env->value)
            printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
}

int	main(int argc, char *argv[], char **env)
{
	char	*input;
	int		i;

	(void)argc;
	(void)argv;

	t_env *my_env = env_init(env);
    print_env(my_env);
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
	}
	/*free(input);*/
	return (0);
}
