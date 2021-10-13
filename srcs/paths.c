/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 17:21:18 by mberne            #+#    #+#             */
/*   Updated: 2021/10/13 10:45:20 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	find_good_path(t_structs *s, char **paths)
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
				ft_exit(s, "malloc", EXIT_FAILURE);
			if (open(tmp_path, O_RDONLY) != -1)
			{
				s->cmds[i].path = ft_strdup(tmp_path);
				if (!s->cmds[i].path)
				{
					free(tmp_path);
					ft_exit(s, "malloc", EXIT_FAILURE);
				}
				break ;
			}
			free(tmp_path);
		}
	}
}

void	set_paths(t_structs *s, char **paths)
{
	size_t	i;

	i = 0;
	while (paths[i])
	{
		paths[i] = ft_strjoin_f1(paths[i], "/");
		if (!paths[i])
		{
			free_tab(paths, 0);
			ft_exit(s, "malloc", EXIT_FAILURE);
		}
		i++;
	}
}

void	find_cmd_paths(t_structs *s)
{
	char	**paths;
	t_env	*tmp;

	tmp = *s->env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->name, "PATH", 5))
			break ;
		tmp = tmp->next;
	}
	paths = ft_split(tmp->value, ':');
	if (!paths)
		ft_exit(s, "malloc", EXIT_FAILURE);
	set_paths(s, paths);
	find_good_path(s, paths);
}
