/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 17:21:18 by mberne            #+#    #+#             */
/*   Updated: 2021/11/03 06:35:39 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// ~~ Gestion d'erreur de la recuperation du chemin

int	path_error_check(t_cmd *current)
{
	int	tmp;
	DIR	*dir;
	int	fd;

	tmp = errno;
	dir = opendir(current->path);
	fd = open(current->path, O_RDONLY);
	errno = tmp;
	if (!current->path)
		print_error(NULL, current->cmd[0], "command not found\n", 127);
	else if (dir)
	{
		print_error(NULL, current->path, NULL, EISDIR);
		closedir(dir);
	}
	else if (fd == -1 || close(fd) == -1)
		print_error(NULL, current->cmd[0], NULL, ENOENT);
	else if (access(current->path, X_OK) == -1)
		print_error(NULL, current->cmd[0], NULL, EACCES);
	else
		return (0);
	return (-1);
}

// ~~ Trouve le chemin de la commande dans l'ordinateur

int	find_exe_path(t_structs *s, t_cmd *current)
{
	char	cwd[MAXPATHLEN];
	int		nb_args;

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
	nb_args = 0;
	while (current->cmd[nb_args])
		nb_args++;
	free(current->cmd[0]);
	current->cmd[0] = ft_strdup(ft_strrchr(current->path, '/') + 1);
	if (!current->cmd[0])
	{
		free_tab(current->cmd, nb_args);
		return (-1);
	}
	return (0);
}

// ~~ Trouve le chemin de la commande dans PATH

int	find_path_in_sys(t_cmd *current, char **paths)
{
	size_t	i;
	int		fd;

	i = 0;
	while (paths[i])
	{
		current->path = ft_strjoin_f0(paths[i], current->cmd[0]);
		if (!current->path)
			return (-1);
		fd = open(current->path, O_RDONLY);
		if (fd != -1)
		{
			if (close(fd) == -1)
				print_error("close: ", NULL, NULL, errno);
			break ;
		}
		else
		{
			free(current->path);
			current->path = NULL;
		}
		i++;
	}
	return (0);
}

// ~~ Ajoute un '/' a la fin des chemins

static char	**add_backslash(char **paths)
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
			print_error("malloc: ", NULL, NULL, ENOMEM);
			free_tab(paths, paths_size);
			paths = NULL;
			return (NULL);
		}
		i++;
	}
	return (paths);
}

// ~~ Recupere les chemins de la variable PATH

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
			if (!paths)
				print_error("malloc: ", NULL, NULL, ENOMEM);
			break ;
		}
		elem = elem->next;
	}
	return (add_backslash(paths));
}