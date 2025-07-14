/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 12:05:21 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/09 22:52:51 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*handle_dollar_expansion(t_gc *gc, char *str, size_t *i, t_env *env)
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

char	*process_character(t_gc *gc, char *result, char *str, size_t *i)
{
	char	*char_str;

	char_str = gc_strndup(gc, &str[*i], 1);
	(*i)++;
	return (gc_strjoin_free_a(gc, result, char_str));
}

char	*get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return ("");
}

char	*extract_var_name(t_gc *gc, char *str, size_t *i)
{
	size_t	start;

	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (gc_strndup(gc, "?", 1));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	return (gc_strndup(gc, &str[start], *i - start));
}

char	**split_words(t_gc *gc, char *str)
{
	char	**result;
	size_t	count;
	char	*token;
	char	*copy;
	char	*saveptr;

	if (!str || !*str)
		return (NULL);
	result = NULL;
	count = 0;
	copy = gc_strdup(gc, str);
	token = strtok_r(copy, " \t\n", &saveptr);
	while (token)
	{
		result = gc_realloc(gc, result, sizeof(char *) * count,
				sizeof(char *) * (count + 2));
		result[count++] = gc_strdup(gc, token);
		token = strtok_r(NULL, " \t\n", &saveptr);
	}
	if (result)
		result[count] = NULL;
	return (result);
}

int	needs_word_splitting(char *str)
{
	return (str && (strchr(str, ' ') || strchr(str, '\t')));
}

t_token	*create_split_tokens(t_gc *gc, char **split, t_token *next)
{
	t_token	*first;
	t_token	*current;
	int		i;

	if (!split || !split[0])
		return (next);
	first = new_token(TOKEN_WORD, split[0], gc);
	current = first;
	i = 1;
	while (split[i])
	{
		current->next = new_token(TOKEN_WORD, split[i], gc);
		current = current->next;
		i++;
	}
	current->next = next;
	return (first);
}
