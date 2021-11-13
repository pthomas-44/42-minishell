/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 17:21:18 by mberne            #+#    #+#             */
/*   Updated: 2021/11/13 20:32:59 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ~~ Trouve le chemin de la commande dans l'ordinateur

static int	find_exe_path(t_structs *s, t_cmd *current)
{
	char	cwd[MAXPATHLEN];
	int		nb_args;

	replace_by_home_path(s, current->cmd[0], &current->path);
	if (!current->path)
		return (-1);
	if (current->path[0] != '/')
	{
		if (!getcwd(cwd, MAXPATHLEN))
			print_error("getcwd: ", NULL, NULL, errno);
		current->path = ft_strjoin_f3(ft_strjoin_f0(cwd, "/"), current->path);
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
		free_tab(&current->cmd, nb_args);
		return (-1);
	}
	return (0);
}

// ~~ Trouve le chemin de la commande dans PATH

static int	find_path_in_sys(t_cmd *current, char **paths)
{
	size_t	i;
	int		fd;

	i = 0;
	while (paths[i] && ft_strcmp(current->cmd[0], "")
		&& ft_strcmp(current->cmd[0], "..") && ft_strcmp(current->cmd[0], "."))
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
			free_tab(&paths, paths_size);
			paths = NULL;
			return (NULL);
		}
		i++;
	}
	return (paths);
}

// ~~ Recupere les chemins de la variable PATH

static char	**get_env_paths(t_structs *s)
{
	char	**paths;
	t_env	*elem;

	paths = NULL;
	elem = find_env_var(s, "PATH");
	if (elem)
	{
		paths = ft_split(elem->value + 1, ':');
		if (!paths)
			print_error("malloc: ", NULL, NULL, ENOMEM);
	}
	return (add_backslash(paths));
}

// ~~ Recupere le chemin d'une commande

void	get_path(t_structs *s, t_cmd *current)
{
	char	**paths;

	if (!current->cmd)
		return ;
	paths = get_env_paths(s);
	if (paths && !ft_strchr(current->cmd[0], '/')
		&& find_path_in_sys(current, paths) == -1)
		print_error("malloc: ", NULL, NULL, ENOMEM);
	else if ((!paths || ft_strchr(current->cmd[0], '/'))
		&& !current->path && find_exe_path(s, current) == -1)
		print_error("malloc: ", NULL, NULL, ENOMEM);
	free_tab(&paths, 0);
}
