/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 16:01:23 by mberne            #+#    #+#             */
/*   Updated: 2021/10/11 13:43:43 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_echo(t_structs *s, int option, int out)
{
	int	i;

	if (!option)
		i = 1;
	else
		i = 2;
	while (s->cmds[2].cmd[i])
	{
		write(out, s->cmds[2].cmd[i], ft_strlen(s->cmds[2].cmd[i]));
		i++;
	}
	if (!option)
		write(out, "\n", 1);
}

void	ft_cd(t_structs *s, char *path)
{
	if (chdir(path) == -1)
		ft_exit(s, "chdir", EXIT_FAILURE);
	// changer le dolpwd et le pwd
}

void	ft_export(t_structs *s)
{
	(void)s;
}

void	ft_unset(t_structs *s)
{
	(void)s;
}

void	ft_env(t_structs *s)
{
	(void)s;
}
