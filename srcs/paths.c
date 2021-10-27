/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 17:21:18 by mberne            #+#    #+#             */
/*   Updated: 2021/10/27 18:15:36 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	path_error_check(t_cmd *current)
{
	DIR	*dir;

	dir = opendir(current->path);
	if (!current->path || dir || open(current->path, O_RDONLY) == -1
		|| access(current->path, X_OK) == -1)
		write(2, "minishell: ", 11);
	if (!current->path || open(
			current->path, O_RDONLY) == -1 || access(current->path, X_OK) == -1)
		write(2, current->cmd[0], ft_strlen(current->cmd[0]));
	if (!current->path)
		write(2, ": command not found\n", 20);
	else if (dir)
	{
		write(2, current->path, ft_strlen(current->path));
		write(2, ": is a directory\n", 18);
		closedir(dir);
	}
	else if (open(current->path, O_RDONLY) == -1)
		write(2, ": No such file or directory\n", 29);
	else if (access(current->path, X_OK) == -1)
		write(2, ": Permission denied\n", 21);
	else
		return (0);
	return (-1);
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
	if (cmd && (cmd[0] != '~' || ft_strcmp(elem->name, "HOME")))
		return (ft_strdup(cmd));
	if (cmd)
		new = ft_strjoin_f0(elem->value + 1, cmd + 1);
	else
		new = ft_strdup(elem->value + 1);
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
	{
		free(current->path);
		current->path = NULL;
		return (-1);
	}
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
			paths = ft_split(elem->value + 1, ':');
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
