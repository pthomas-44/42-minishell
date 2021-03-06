/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 01:49:55 by mberne            #+#    #+#             */
/*   Updated: 2021/11/16 15:07:58 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//~~ Vérifie l'option -n pour echo

static int	check_option_n(char *arg)
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

//~~ Built-in echo

void	bi_echo(t_cmd *current, int fd)
{
	size_t	i;
	bool	option;

	g_error_number = EXIT_SUCCESS;
	i = 0;
	option = 0;
	if (current->cmd[1])
	{
		while (current->cmd[++i] && current->cmd[i][0] == '-')
		{
			if (check_option_n(current->cmd[i] + 1))
				option = 1;
			else
				break ;
		}
		while (current->cmd[i])
		{
			ft_putstr_fd(current->cmd[i], fd);
			if (current->cmd[++i])
				ft_putchar_fd(' ', fd);
		}
	}
	if (!option)
		ft_putchar_fd('\n', fd);
}
