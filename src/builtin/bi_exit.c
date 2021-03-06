/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 01:51:53 by mberne            #+#    #+#             */
/*   Updated: 2021/11/16 15:08:25 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//~~ Verifie si l'argument est numerique

static int	is_arg_numeric(char *arg)
{
	skip_spaces(&arg);
	if (*arg == '-' || *arg == '+' || ft_isdigit(*arg))
		arg++;
	while (ft_isdigit(*arg))
		arg++;
	skip_spaces(&arg);
	if (*arg)
		return (0);
	else
		return (1);
}

//~~ Built-in exit

void	bi_exit(t_structs *s, t_cmd *current)
{
	if (s->cmds_size == 1)
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (current->cmd[1] && !is_arg_numeric(current->cmd[1]))
	{
		print_error("exit: ", current->cmd[1],
			"numeric argument required\n", 255);
	}
	else if (current->cmd[1] && current->cmd[2])
	{
		print_error("exit: ", NULL, "too many arguments\n", EXIT_FAILURE);
		return ;
	}
	else if (current->cmd[1])
		g_error_number = ft_atoi(ft_strchrstr(current->cmd[1], "-0123456789"));
	free_all(s, s->cmds_size - 1);
	exit(g_error_number);
}
