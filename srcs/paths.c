/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 17:21:18 by mberne            #+#    #+#             */
/*   Updated: 2021/10/15 18:57:07 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	find_good_path(t_structs *s, char **paths)	// mettre la fonctions de type "int"
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
				ft_exit(s, "malloc", EXIT_FAILURE);	// remplacer par "return (-1);"
			if (open(tmp_path, O_RDONLY) != -1)
			{
				s->cmds[i].path = ft_strdup(tmp_path);
				if (!s->cmds[i].path)
				{
					free(tmp_path);
					ft_exit(s, "malloc", EXIT_FAILURE);	// remplacer par "return (-1);"
				}
				break ;
			}
			free(tmp_path);
		}
	}
}	// rajouter "return (0);"

void	set_paths(t_structs *s, char **paths)	// mettre la fonctions de type "int"
{
	size_t	i;

	i = 0;
	while (paths[i])
	{
		paths[i] = ft_strjoin_f1(paths[i], "/");
		if (!paths[i])
		{
			free_tab(paths, 0);	// ici freetab() ne liberera pas les elements d'apres paths[i] car paths[i] est a NULL, il faut rajouter en deuxieme argument la taille de paths[i]
			ft_exit(s, "malloc", EXIT_FAILURE);	// remplacer par "return (-1);"
		}
		i++;
	}
}	// rajouter "return (0);"

void	find_cmd_paths(t_structs *s)	// mettre la fonctions de type "int"
{
	char	**paths;
	t_env	*tmp;

	tmp = *s->env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, "PATH"))
			break ;
		tmp = tmp->next;
	}
	paths = ft_split(tmp->value, ':');
	if (!paths)
		ft_exit(s, "malloc", EXIT_FAILURE);	// remplacer par "return (-1);"
	set_paths(s, paths); // remplacer par "if (set_paths(s, paths) == -1){	return(-1);}"
	find_good_path(s, paths);	// remplacer par "if (find_good_path(s, paths) == -1){	free_tab(paths);	return(-1);}
}	// rajouter "freetab(paths);	return (0);"
