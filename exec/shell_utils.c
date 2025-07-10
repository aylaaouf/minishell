/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aylaaouf <aylaaouf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:48:40 by aylaaouf          #+#    #+#             */
/*   Updated: 2025/07/10 11:24:15 by aylaaouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strjoin_env(char *s1, char *s2, t_gc *gc)
{
	size_t	i;
	size_t	j;
	size_t	len1;
	size_t	len2;
	char	*array;

	if (!s1)
		return (NULL);
	if (!s2)
		return (s1);
	i = -1;
	j = 0;
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	array = gc_malloc(gc, len1 + len2 + 2);
	if (!array)
		return (NULL);
	while (++i < len1)
		array[i] = s1[i];
	array[i++] = '=';
	while (j < len2)
		array[i++] = s2[j++];
	array[i] = '\0';
	return (array);
}

char	*ft_strjoin_free(t_gc *gc, char *s1, char *s2)
{
	char	*arr;

	arr = ft_strjoin(gc, s1, s2);
	return (arr);
}

int	handle_stat_error(t_command *cmd)
{
	if (cmd->args[0][0] == '/' || (cmd->args[0][0] == '.'
			&& cmd->args[0][1] == '/'))
		write(2, "No such file or directory\n", 27);
	else
		write(2, "command not found\n", 19);
	g_last_exit_status = 127;
	return (127);
}
