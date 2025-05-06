/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 19:15:16 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/05/03 20:52:40 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*void print_env(t_env *env)*/
/*{*/
/*    while (env)*/
/*    {*/
/*        if (env->value)*/
/*            printf("%s=%s\n", env->key, env->value);*/
/*        env = env->next;*/
/*    }*/
/*}*/
/**/
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

void print_token_type(t_token_type type)
{
    if (type == TOKEN_IDINTTFE)
		printf("TOKEN_WORD");
    else if (type == TOKEN_PIPE)
		printf("TOKEN_PIPE");
    else if (type == TOKEN_INPUT) 
		printf("TOKEN_REDIR_IN");
    else if (type == TOKEN_OUTPUT)
		printf("TOKEN_REDIR_OUT");
    else if (type == TOKEN_APPEND) 
		printf("TOKEN_REDIR_APPEND");
    else if (type == TOKEN_HEREDOC)  
		printf("TOKEN_HEREDOC");
    else if (type == TOKEN_SQUOTE)   
		printf("TOKEN_SQUOTE");
    else if (type == TOKEN_DQUOTE)   
		printf("TOKEN_DQUOTE");
    else                             
		printf("UNKNOWN");
}

void print_tokens(t_token *tokens)
{
    while (tokens)
    {
        printf("[");
        print_token_type(tokens->type);
        printf("] -> \"%s\"\n", tokens->value);
        tokens = tokens->next;
    }
}

int main(void)
{
    char *input = "echo \"hello world\" | grep h > out.txt";
    t_token *tokens = tokenize(input);

    if (!tokens)
        return (1);

    print_tokens(tokens);

    return (0);
}

