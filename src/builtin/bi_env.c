/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 01:51:20 by mberne            #+#    #+#             */
/*   Updated: 2021/11/16 15:08:18 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//~~ Built-in env

void	bi_env(t_structs *s, int fd)
{
	t_env	*elem;

	g_error_number = EXIT_SUCCESS;
	elem = *s->env;
	while (elem)
	{
		if (elem->value)
		{
			ft_putstr_fd(elem->name, fd);
			ft_putstr_fd(elem->value, fd);
			ft_putchar_fd('\n', fd);
		}
		elem = elem->next;
	}
}
