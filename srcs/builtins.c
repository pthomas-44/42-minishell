/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 16:01:23 by mberne            #+#    #+#             */
/*   Updated: 2021/10/26 16:02:17 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_echo(t_structs *s, t_cmd current)
{
	size_t	i;
	bool	option;

	(void)s;
	i = 0;
	option = 0;
	printf("%d | %d\n", current.fd_in, current.fd_out);
	if (current.cmd[1])
	{
		while (current.cmd[++i] && current.cmd[i][0] == '-')
		{
			if (check_option_n(current.cmd[i] + 1))
				option = 1;
			else
				break ;
		}
		while (current.cmd[i])
		{
			write(current.fd_out, current.cmd[i], ft_strlen(current.cmd[i]));
			if (current.cmd[++i])
				write(current.fd_out, " ", 1);
		}
	}
	if (!option)
		write(current.fd_out, "\n", 1);
	return (0);
}

int	ft_pwd(t_structs *s, t_cmd current)
{
	char	cwd[MAXPATHLEN];

	(void)s;
	getcwd(cwd, MAXPATHLEN);
	write(current.fd_out, cwd, ft_strlen(cwd));
	write(current.fd_out, "\n", 1);
	return (0);
}

int	ft_env(t_structs *s, t_cmd current)
{
	t_env	*elem;

	elem = *s->env;
	while (elem)
	{
		if (ft_strlen(elem->value) > 0)
		{
			write(current.fd_out, elem->name, ft_strlen(elem->name));
			write(current.fd_out, elem->value, ft_strlen(elem->value));
			write(current.fd_out, "\n", 1);
		}
		elem = elem->next;
	}
	return (0);
}

int	ft_unset(t_structs *s, t_cmd current)
{
	t_env	*unset;
	size_t	i;

	if (!current.cmd[1])
	{
		errno = EXIT_FAILURE;
		return (-1);
	}
	i = 1;
	while (current.cmd[i])
	{
		unset = *s->env;
		while (unset)
		{
			if (!ft_strcmp(unset->name, current.cmd[i]))
			{
				env_del(s, unset);
				break ;
			}
			unset = unset->next;
		}
		i++;
	}
	return (0);
}
