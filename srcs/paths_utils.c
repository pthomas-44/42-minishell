/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 17:21:18 by mberne            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2021/10/29 18:23:33 by mberne           ###   ########lyon.fr   */
=======
/*   Updated: 2021/10/29 18:21:51 by pthomas          ###   ########lyon.fr   */
>>>>>>> 1a57e435d2cb07996cffbd50ad3992d4d9fd9c4e
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// ~~ Gestion d'erreur de la recuperation du chemin

int	path_error_check(t_cmd *current)
{
	DIR	*dir;
	int	tmp;

	tmp = errno;
	dir = opendir(current->path);
	errno = tmp;
	if (!current->path)
		print_error(NULL, current->cmd[0], ": command not found\n", 127);
	else if (dir)
	{
		print_error(NULL, current->path, NULL, EISDIR);
		closedir(dir);
	}
	else if (open(current->path, O_RDONLY) == -1)
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
			break ;
		}
		elem = elem->next;
	}
	if (!paths)
		print_error("malloc: ", NULL, NULL, ENOMEM);
	return (add_backslash(paths));
}
