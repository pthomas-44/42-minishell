/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 17:21:18 by mberne            #+#    #+#             */
/*   Updated: 2021/10/19 15:18:42 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	find_executable_path(t_structs *s, t_cmd current)
{
	char	cwd[MAXPATHLEN];
	char	*tmp;

	if (current.cmd[0][0] == '~')
	{
		if (go_home(s, current, 0) == -1)
		{
			errno = EXIT_FAILURE;
			return (-1);
		}
	}
	if (open(current.cmd[0], O_RDONLY) != -1)
	{
		current.path = ft_strdup(current.cmd[0]);
		if (!current.path)
			return (-1);
	}
	else if (current.cmd[0][0] == '.' && current.cmd[0][1] == '/')
	{
		getcwd(cwd, MAXPATHLEN);
		tmp = ft_strjoin_f1(cwd, "/");
		if (!tmp)
			return (-1);
		tmp = ft_strjoin_f1(tmp, current.cmd[0]);
		if (!tmp)
			return (-1);
		if (open(tmp, O_RDONLY) != -1)
		{
			current.path = ft_strdup(tmp);
			if (!current.path)
				return (-1);
		}
	}
	else
	{
		write(2, "minishell: ", 11);
		write(2, current.cmd[0], ft_strlen(current.cmd[0]));
		write(2, ": No such file or directory\n", 28);
	}
	return (0);
}

int	find_good_path(t_structs *s, char **paths)
{
	size_t	i;
	size_t	j;
	char	*tmp_path;

	i = -1;
	while (++i < s->cmds_size)
	{
		j = -1;
		while (paths[++j])
		{
			tmp_path = ft_strjoin_f0(paths[j], s->cmds[i].cmd[0]);
			if (!tmp_path)
				return (-1);
			if (open(tmp_path, O_RDONLY) != -1)
			{
				s->cmds[i].path = ft_strdup(tmp_path);
				if (!s->cmds[i].path)
				{
					free(tmp_path);
					return (-1);
				}
				break ;
			}
			free(tmp_path);
		}
	}
	return (0);
}

int	set_paths(char **paths, int path_size)
{
	size_t	i;

	i = 0;
	while (paths[i])
	{
		paths[i] = ft_strjoin_f1(paths[i], "/");
		if (!paths[i])
		{
			free_tab(paths, path_size);
			return (-1);
		}
		i++;
	}
	return (0);
}

int	find_cmd_paths(t_structs *s)
{
	char	**paths;
	t_env	*tmp;
	int		path_size;

	tmp = *s->env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, "PATH"))
			break ;
		tmp = tmp->next;
	}
	paths = ft_split(tmp->value, ':');
	if (!paths)
		return (-1);
	path_size = 0;
	while (paths[path_size])
		path_size++;
	if (set_paths(paths, path_size) == -1)
	{
		free_tab(paths, path_size);
		return (-1);
	}
	if (find_good_path(s, paths) == -1)
	{
		free_tab(paths, path_size);
		return (-1);
	}
	free_tab(paths, 0);
	return (0);
}
