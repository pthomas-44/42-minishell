/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 16:01:23 by mberne            #+#    #+#             */
/*   Updated: 2021/10/18 17:27:23 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_option_n(char *arg)
{
	size_t	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	ft_echo(t_cmd current)
{
	size_t	i;
	bool	option;

	i = 1;
	option = 0;
	if (current.cmd[i])
	{
		while (current.cmd[i] && current.cmd[i][0] == '-')
		{
			if (check_option_n(current.cmd[i] + 1))
				option = 1;
			else
				break ;
			i++;
		}
		while (current.cmd[i])
		{
			write(current.fd_out, current.cmd[i], ft_strlen(current.cmd[i]));
			i++;
			if (current.cmd[i])
				write(current.fd_out, " ", 1);
		}
	}
	if (!option)
		write(current.fd_out, "\n", 1);
}

void	ft_pwd(t_cmd current)
{
	char	cwd[MAXPATHLEN];

	getcwd(cwd, MAXPATHLEN);
	write(current.fd_out, cwd, ft_strlen(cwd));
	write(current.fd_out, "\n", 1);
}

void	ft_env(t_structs *s, t_cmd current)
{
	t_env	*elem;

	elem = *s->env;
	while (elem)
	{
		if (ft_strlen(elem->value) > 0)
		{
			write(STDOUT_FILENO, elem->name, ft_strlen(elem->name));
			write(current.fd_out, elem->value, ft_strlen(elem->value));
			write(current.fd_out, "\n", 1);
		}
		elem = elem->next;
	}
}

void	ft_unset(t_structs *s, t_cmd current)
{
	t_env	*unset;

	if (!current.cmd[1])
		return ;
	unset = *s->env;
	while (unset)
	{
		if (!ft_strcmp(unset->name, current.cmd[1]))
		{
			env_del(s, unset);
			return ;
		}
		unset = unset->next;
	}
}
