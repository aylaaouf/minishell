/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 13:15:51 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/08 13:17:40 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_child_handler(int sig)
{
	(void)sig;
	exit(130);
}

int	is_quoted(const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = ft_strlen(s);
	return ((len >= 2 && s[0] == '\'' && s[len - 1] == '\'') || (len >= 2
			&& s[0] == '"' && s[len - 1] == '"'));
}

char	*strip_quotes(t_gc *gc, const char *s)
{
	if (is_quoted(s))
		return (gc_substr(gc, s, 1, ft_strlen(s) - 2));
	return (gc_strdup(gc, s));
}

char	*get_env_value_heredoc(char *key, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return ("");
}

char	*expand_status(t_gc *gc, char *result, size_t *i)
{
	char	*status;

	status = ft_itoa_gc(gc, g_last_exit_status);
	result = gc_strjoin_free_a(gc, result, status);
	(*i)++;
	return (result);
}
