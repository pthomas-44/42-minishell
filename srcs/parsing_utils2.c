/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 17:44:07 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/15 18:06:59 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*replace_var(char *line, size_t i, t_env *var)
{
	char	*new;

	new = ft_calloc(ft_strlen(line), sizeof(char));
	if (!new)
		return (NULL);
	ft_strlcpy(new, line, i + 1);
	if (var)
	{
		new = ft_strjoin_f1(new, var->value + 1);
		new = ft_strjoin_f1(new, line + i + ft_strlen(var->name) + 1);
	}
	else
	{
		while (line[i] != '"' && line[i] != ' ' && line[i] != '\'')
			i++;
		new = ft_strjoin_f1(new, line + i);
	}
	return (new);
}

t_env	*find_var(t_structs *s, char *line)
{
	t_env	*current;
	char	*name;
	size_t	i;

	i = 0;
	while (line[i] && line[i] != ' ' && line[i] != '"' && line[i] != '\'')
		i++;
	name = ft_substr(line, 0, i);
	current = *s->env;
	while (current->next
		&& ft_strncmp(current->name, name, ft_strlen(name) + 1))
		current = current->next;
	if (ft_strncmp(current->name, line, ft_strlen(name)))
	{
		free(name);
		return (NULL);
	}
	else
	{
		free(name);
		return (current);
	}
}

char	*remove_char(char *str, size_t i)
{
	char	*new;

	if (i)
		new = ft_substr(str, 0, i);
	else
		new = ft_strdup("");
	new = ft_strjoin_f1(new, str + i + 1);
	free(str);
	return (new);
}

void	remove_quotes(char ***cmd)
{
	size_t	i;
	size_t	j;
	char	quote;

	i = 0;
	quote = 0;
	while ((*cmd)[i])
	{
		j = 0;
		while ((*cmd)[i][j])
		{
			if (!quote && ((*cmd)[i][j] == '"' || (*cmd)[i][j] == '\''))
			{
				quote = (*cmd)[i][j];
				(*cmd)[i] = remove_char((*cmd)[i], j--);
			}
			else if ((*cmd)[i][j] == quote)
			{
				quote = 0;
				(*cmd)[i] = remove_char((*cmd)[i], j--);
			}
			j++;
		}
		i++;
	}
}

int	get_infile_sequel(t_structs *s, char ***line, int i, char **tmp)
{
	if (*(tmp[0] - 2) != '<')
		s->cmds[i].fd_in = open(tmp[1], O_RDONLY);
	if (s->cmds[i].fd_in == -1)
	{
		free(tmp[1]);
		perror("open");
		return (-1);
	}
	free(tmp[1]);
	(*(*line)) += ft_strlen(tmp[1]);
	return (0);
}
