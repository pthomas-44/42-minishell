/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 16:01:23 by mberne            #+#    #+#             */
/*   Updated: 2021/11/02 17:24:21 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//~~ Built-in echo

void	bi_echo(t_cmd current)
{
	size_t	i;
	bool	option;

	i = 0;
	option = 0;
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
			write(STDOUT_FILENO, current.cmd[i], ft_strlen(current.cmd[i]));
			if (current.cmd[++i])
				write(STDOUT_FILENO, " ", 1);
		}
	}
	if (!option)
		write(STDOUT_FILENO, "\n", 1);
	errno = EXIT_SUCCESS;
}

//~~ Built-in pwd

void	bi_pwd(void)
{
	char	cwd[MAXPATHLEN];

	getcwd(cwd, MAXPATHLEN);
	write(STDOUT_FILENO, cwd, ft_strlen(cwd));
	write(STDOUT_FILENO, "\n", 1);
	errno = EXIT_SUCCESS;
}

//~~ Built-in env

void	bi_env(t_structs *s)
{
	t_env	*elem;

	elem = *s->env;
	while (elem)
	{
		if (ft_strlen(elem->value) > 0)
		{
			write(STDOUT_FILENO, elem->name, ft_strlen(elem->name));
			write(STDOUT_FILENO, elem->value, ft_strlen(elem->value));
			write(STDOUT_FILENO, "\n", 1);
		}
		elem = elem->next;
	}
	errno = EXIT_SUCCESS;
}

//~~ Built-in unset

void	bi_unset(t_structs *s, t_cmd current)
{
	t_env	*unset;
	size_t	i;

	if (!current.cmd[1])
		return ;
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
	errno = EXIT_SUCCESS;
}

//~~ Built-in exit

void	bi_exit(t_structs *s, t_cmd current)
{
	int	exit_status;

	write(STDERR_FILENO, "exit\n", 5);
	if (!current.cmd[1])
	{
		free_all(s);
		exit(errno);
	}
	if (current.cmd[2])
	{
		print_error(NULL, "exit", "too many arguments\n", EXIT_FAILURE);
		return ;
	}
	else if ((current.cmd[1][0] == '-' || ft_isdigit(current.cmd[1][0]))
		&& ft_str_isdigit(current.cmd[1] + 1))
		errno = ft_atoi(current.cmd[1]);
	else if (ft_str_isprint(current.cmd[1])
		&& !ft_atoi_exit(current.cmd[1], &exit_status))
		errno = exit_status;
	else
		print_error("exit: ", current.cmd[1],
			"numeric argument required\n", 255);
	free_all(s);
	exit(errno);
}
