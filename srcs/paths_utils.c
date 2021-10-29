/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 17:21:18 by mberne            #+#    #+#             */
/*   Updated: 2021/10/27 19:28:26 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// ~~ Gestion d'erreur de la recuperation du chemin

int	path_error_check(t_cmd *current)
{
	DIR	*dir;

	dir = opendir(current->path);
	if (!current->path)
	{
		errno = 127;
		write(2, "potatoshell: ", 13);
		write(2, current->cmd[0], ft_strlen(current->cmd[0]));
		write(2, ": command not found\n", 20);
	}
	else if (dir)
	{
		errno = EISDIR;
		write(2, "potatoshell: ", 13);
		perror(current->path);
		closedir(dir);
	}
	else if (open(current->path, O_RDONLY) == -1)
	{
		errno = ENOENT;
		write(2, "potatoshell: ", 13);
		perror(current->cmd[0]);
	}
	else if (access(current->path, X_OK) == -1)
	{
		errno = EACCES;
		write(2, "potatoshell: ", 13);
		perror(current->cmd[0]);
	}
	else
		return (0);
	return (-1);
}

// ~~ Trouve le chemin de la commande dans l'ordinateur

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

// ~~ Trouve le chemin de la commande dans PATH

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

// ~~ Ajoute un '/' a la fin des chemins

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
			break ;
		}
		elem = elem->next;
	}
	return (add_backslash(paths));
}
