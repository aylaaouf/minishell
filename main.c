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

/*int	main(int argc, char *argv[], char **env)*/
/*{*/
/*	char	*input;*/
/*	int		i;*/
/**/
/*	(void)argc;*/
/*	(void)argv;*/
/**/
/*	t_env *my_env = env_init(env);*/
/*	while (1)*/
/*	{*/
/*		i = 0;*/
/*		input = readline("marvel$ ");*/
/*		if (!input)*/
/*		{*/
/*			printf("exit\n");*/
/*			break ;*/
/*		}*/
/*		if (!strcmp(input, "env"))*/
/*			print_env(my_env);*/
/*		if (input)*/
/*			add_history(input);*/
/*	}*/
/*	free(input);*/
/*	return (0);*/
/*}*/
#include "minishell.h"

void add_env_node(t_env **env,char *key,char *value)
{
    t_env *new_node = new_env_node(key, value);
    if (!new_node)
        return;
    if (*env == NULL)
    {
        *env = new_node;
        return;
    }
    t_env *current = *env;
    while (current->next)
        current = current->next;
    current->next = new_node;
}

t_env *create_sample_env()
{
    t_env *env = NULL;
    add_env_node(&env, "USER", "aiman");
    add_env_node(&env, "HOME", "/home/aiman");
    add_env_node(&env, "SHELL", "/bin/bash");
    add_env_node(&env, "EMPTY", "");
    return env;
}

void print_tokens(t_token *tokens)
{
    while (tokens)
    {
        printf("[TOKEN_%s] -> %s\n", 
               tokens->type == TOKEN_WORD ? "WORD" : "OTHER",
               tokens->value);
        tokens = tokens->next;
    }
}

int main(void)
{
    char *input = "echo Hello $USER, your home is $HOME and shell is $SHELL. Also $EMPTY and $NONEXISTENT";

    t_token *tokens = tokenize(input);
    printf("Before Quote Management:\n");
    print_tokens(tokens);
    quote_management(tokens);
    printf("\nAfter Quote Management:\n");
    print_tokens(tokens);
    t_env *env = create_sample_env();
    expander(tokens, env);
    printf("\nAfter Variable Expansion:\n");
    print_tokens(tokens);
    return 0;
}

