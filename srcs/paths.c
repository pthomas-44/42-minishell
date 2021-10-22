/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 17:21:18 by mberne            #+#    #+#             */
/*   Updated: 2021/10/20 20:55:00 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	path_error_check(t_cmd *current)
{
	if (!current->path)
	{
		write(2, "minishell: ", 11);
		write(2, current->cmd[0], ft_strlen(current->cmd[0]));
		write(2, ": command not found\n", 20);
		return (-1);
	}
	if (opendir(current->path))
	{
		write(2, "minishell: ", 11);
		write(2, current->path, ft_strlen(current->cmd[0]));
		write(2, ": is a directory\n", 18);
		return (-1);
	}
	if (open(current->path, O_RDONLY) == -1)
	{
		write(2, "minishell: ", 11);
		write(2, current->cmd[0], ft_strlen(current->cmd[0]));
		write(2, ": No such file or directory\n", 28);
		return (-1);
	}
	return (0);
}

char	*replace_by_home_path(t_structs *s, char *cmd)
{
	t_env	*elem;
	char	*new;

	elem = *s->env;
	while (elem)
	{
		if (!ft_strcmp(elem->name, "HOME"))
			break ;
		elem = elem->next;
	}
	if (cmd[0] != '~' || ft_strcmp(elem->name, "HOME"))
		return (ft_strdup(cmd));
	new = ft_strjoin_f0(elem->value + 1, cmd + 1);
	return (new);
}

int	find_exe_path(t_structs *s, t_cmd *current)
{
	char	cwd[MAXPATHLEN];
	char	*tmp;

	current->path = replace_by_home_path(s, current->cmd[0]);
	if (!current->path)
		return (-1);
	if (current->path[0] != '/')
	{
		current->path = ft_strjoin_f2(
				ft_strjoin_f0(getcwd(cwd, MAXPATHLEN), "/"), current->path);
		if (!current->path)
			return (-1);
	}
	tmp = ft_strrchr(current->path, '/');
	free(current->cmd[0]);
	current->cmd[0] = ft_strdup(ft_strrchr(current->path, '/') + 1);
	return (0);
}

int	find_path_in_sys(t_cmd *current, char **paths)
{
	size_t	i;

	i = 0;
	while (paths[i])
	{
		current->path = ft_strjoin_f0(paths[i], current->cmd[0]);
		if (!current->path)
			return (-1);
		if (open(current->path, O_RDONLY) != -1)
			break ;
		else
		{
			free(current->path);
			current->path = NULL;
		}
		i++;
	}
	return (0);
}

int	find_path(t_structs *s, char **paths, t_cmd *current)
{
	if (paths && !ft_strchr(current->cmd[0], '/')
		&& find_path_in_sys(current, paths) == -1)
	{
		perror("malloc");
		return (-1);
	}
	if ((!paths || ft_strchr(current->cmd[0], '/'))
		&& !current->path && find_exe_path(s, current) == -1)
	{
		perror("malloc");
		return (-1);
	}
	if (path_error_check(current) == -1)
		return (-1);
	return (0);
}

char	**add_backslash(char **paths)
{
	size_t	i;
	size_t	paths_size;

	i = 0;
	paths_size = 0;
	while (paths && paths[paths_size])
		paths_size++;
	while (paths && paths[i])
	{
		paths[i] = ft_strjoin_f1(paths[i], "/");
		if (!paths[i])
		{
			free_tab(paths, paths_size);
			paths = NULL;
			return (paths);
		}
		i++;
	}
	return (paths);
}

char	**get_env_paths(t_structs *s)
{
	char	**paths;
	t_env	*elem;

	elem = *s->env;
	paths = NULL;
	while (elem)
	{
		if (!ft_strcmp(elem->name, "PATH"))
		{
			paths = ft_split(elem->value, ':');
			break ;
		}
		elem = elem->next;
	}
	return (add_backslash(paths));
}

int	get_path(t_structs *s, t_cmd *current)
{
	char	**paths;

	paths = get_env_paths(s);
	if (find_path(s, paths, current) == -1)
	{
		free_tab(paths, 0);
		return (-1);
	}
	free_tab(paths, 0);
	return (0);
}
