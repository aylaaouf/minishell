/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:06:04 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/08 15:22:39 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_dollar_expansion(t_gc *gc, char *str, size_t *i, t_env *env)
{
	char	*key;
	char	*status_str;
	char	*val;

	(*i)++;
	if (str[*i] == '\0')
		return (gc_strdup(gc, "$"));
	if (str[*i] != '?' && !ft_isalnum(str[*i]) && str[*i] != '_')
	{
		key = gc_strndup(gc, &str[*i], 1);
		(*i)++;
		return (gc_strjoin_free_a(gc, gc_strdup(gc, "$"), key));
	}
	key = extract_var_name(gc, str, i);
	if (key[0] == '?')
	{
		status_str = ft_itoa_gc(gc, g_last_exit_status);
		return (status_str);
	}
	else
	{
		val = get_env_value(env, key);
		return (gc_strdup(gc, val));
	}
}

static char	*process_character(t_gc *gc, char *result, char *str, size_t *i)
{
	char	*char_str;

	char_str = gc_strndup(gc, &str[*i], 1);
	(*i)++;
	return (gc_strjoin_free_a(gc, result, char_str));
}

static char	*handle_quotes(char *str, size_t *i, char *quote)
{
	if (!*quote && (str[*i] == '"' || str[*i] == '\''))
	{
		*quote = str[*i];
		(*i)++;
		return ("continue");
	}
	else if (*quote && str[*i] == *quote)
	{
		*quote = 0;
		(*i)++;
		return ("continue");
	}
	return (NULL);
}

static char	*expand_token_value_no_squote(t_gc *gc, char *str, t_env *env)
{
	size_t	i;
	char	*result;
	char	*expansion;

	i = 0;
	result = gc_strdup(gc, "");
	while (str[i])
	{
		if (str[i] == '$')
		{
			expansion = handle_dollar_expansion(gc, str, &i, env);
			result = gc_strjoin_free_a(gc, result, expansion);
		}
		else
			result = process_character(gc, result, str, &i);
	}
	return (result);
}

char	*expand_token_value(t_gc *gc, char *str, t_env *env)
{
	size_t	i;
	char	*result;
	char	quote;
	char	*expansion;

	i = 0;
	result = gc_strdup(gc, "");
	quote = 0;
	while (str[i])
	{
		if (handle_quotes(str, &i, &quote))
			continue ;
		if (str[i] == '$' && quote != '\'')
		{
			expansion = handle_dollar_expansion(gc, str, &i, env);
			result = gc_strjoin_free_a(gc, result, expansion);
		}
		else
			result = process_character(gc, result, str, &i);
	}
	return (result);
}

static int	should_expand_token(t_token *token)
{
	if (token->type == TOKEN_SQUOTE)
		return (0);
	if ((token->type == TOKEN_WORD || token->type == TOKEN_DQUOTE)
		&& strchr(token->value, '$'))
		return (1);
	return (0);
}

void	expander(t_gc *gc, t_token *tokens, t_env *env)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_HEREDOC)
		{
			tokens = tokens->next->next;
			continue ;
		}
		if (should_expand_token(tokens))
		{
			if (tokens->type == TOKEN_DQUOTE)
				tokens->value = expand_token_value(gc, tokens->value, env);
			else
				tokens->value = expand_token_value_no_squote(gc,
						tokens->value, env);
		}
		tokens = tokens->next;
	}
}
