/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayelasef <ayelasef@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 13:18:21 by ayelasef          #+#    #+#             */
/*   Updated: 2025/07/08 13:37:20 by ayelasef         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_child_process(int pipefd, t_heredoc_data *data)
{
	char	*line;
	char	*processed;

	signal(SIGINT, heredoc_child_handler);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write_eof_warning(data->delim);
			break ;
		}
		if (ft_strcmp(line, data->delim) == 0)
		{
			free(line);
			break ;
		}
		processed = maybe_expand(data->gc, line, data->expand, data->env);
		write(pipefd, processed, ft_strlen(processed));
		write(pipefd, "\n", 1);
		free(line);
	}
	close(pipefd);
	exit(0);
}

char	*expand_variable(t_expand_data *data, size_t *i)
{
	size_t	start;
	char	*key;
	char	*val;

	start = *i;
	while (ft_isalnum(data->line[*i]) || data->line[*i] == '_')
		(*i)++;
	key = gc_strndup(data->gc, &data->line[start], *i - start);
	val = get_env_value_heredoc(key, data->env);
	data->result = gc_strjoin_free_a(data->gc, data->result, val);
	return (data->result);
}

char	*expand_line(t_gc *gc, char *line, t_env *env)
{
	t_expand_data	data;
	size_t			i;

	data.gc = gc;
	data.result = gc_strdup(gc, "");
	data.line = line;
	data.env = env;
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1])
		{
			i++;
			if (line[i] == '?')
				data.result = expand_status(gc, data.result, &i);
			else
				data.result = expand_variable(&data, &i);
		}
		else
		{
			data.result = ft_strjoin_char_gc(gc, data.result, line[i]);
			i++;
		}
	}
	return (data.result);
}

char	*maybe_expand(t_gc *gc, char *line, int expand, t_env *env)
{
	if (!expand)
		return (gc_strdup(gc, line));
	return (expand_line(gc, line, env));
}

void	write_eof_warning(char *delim)
{
	char	*msg1;
	char	*msg2;
	char	*msg3;

	msg1 = "minishell: warning: here-document ";
	msg2 = "delimited by end-of-file (wanted `";
	msg3 = "`)\n";
	write(2, msg1, ft_strlen(msg1));
	write(2, msg2, ft_strlen(msg2));
	write(2, delim, ft_strlen(delim));
	write(2, msg3, ft_strlen(msg3));
}
