/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:06:43 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/08 15:57:20 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_dollar_variable(char *line, int i, char **joined, t_gc *gc)
{
	int		var_start;
	char	*var;
	char	*full;

	var_start = i;
	while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
		i++;
	if (i > var_start)
	{
		var = gc_strndup(gc, &line[var_start], i - var_start);
		full = gc_strjoin_free_a(gc, gc_strdup(gc, "$"), var);
		*joined = gc_strjoin_free_a(gc, *joined, full);
	}
	else
		*joined = ft_strjoin_char_gc(gc, *joined, '$');
	return (i);
}

int	process_single_quote_content(char *line, int i, char **joined, t_gc *gc)
{
	int		start;
	char	*quoted;

	start = i;
	while (line[i] && line[i] != '\'')
		i++;
	if (line[i] == '\'')
	{
		quoted = gc_strndup(gc, &line[start], i - start);
		*joined = gc_strjoin_free_a(gc, *joined, quoted);
		i++;
	}
	else
		*joined = ft_strjoin_char_gc(gc, *joined, line[start - 1]);
	return (i);
}

int	handle_single_quote(char *line, int i, char **joined, t_gc *gc)
{
	return (process_single_quote_content(line, i, joined, gc));
}

int	handle_double_quote(char *line, int i, char **joined, t_gc *gc)
{
	int		start;
	char	*quoted;

	start = i;
	while (line[i] && line[i] != '"')
		i++;
	if (line[i] == '"')
	{
		quoted = gc_strndup(gc, &line[start], i - start);
		*joined = gc_strjoin_free_a(gc, *joined, quoted);
		i++;
	}
	else
		*joined = ft_strjoin_char_gc(gc, *joined, line[start - 1]);
	return (i);
}

int	handle_dollar_question(char *line, int i, char **joined, t_gc *gc)
{
	int	start;

	start = i;
	*joined = gc_strjoin_free_a(gc, *joined, gc_strndup(gc, &line[start], 2));
	return (i + 2);
}
