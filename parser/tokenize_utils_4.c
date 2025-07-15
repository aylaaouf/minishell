/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils_4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:00:54 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/15 17:21:57 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_standalone_quotes(char *line, int i, t_quote_params *params)
{
	char			quote;
	int				start;
	char			*content;
	t_token_type	q_type;

	quote = line[i++];
	start = i;
	while (line[i] && line[i] != quote)
		i++;
	if (line[i] == quote)
	{
		content = gc_strndup(params->gc, &line[start], i - start);
		if (quote == '\'')
			q_type = TOKEN_SQUOTE;
		else
			q_type = TOKEN_DQUOTE;
		add_token(params->tokens, new_token(q_type, content, params->gc,
				params->has_space_before));
		return (i + 1);
	}
	return (i);
}

int	check_if_standalone_quote(char *line, int i, char **joined)
{
	size_t	j;
	char	quote;

	if (ft_strlen(*joined) == 0)
	{
		j = i + 1;
		quote = line[i];
		while (line[j] && line[j] != quote)
			j++;
		if (line[j] == quote && (j + 1 >= ft_strlen(line) || ft_isspace(line[j
						+ 1]) || is_operator_char(line[j + 1])))
			return (1);
	}
	return (0);
}
