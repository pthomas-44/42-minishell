/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 01:51:20 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/09 10:52:13 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//~~ Built-in env

void	bi_env(t_structs *s)
{
	t_env	*elem;

	g_numberr = EXIT_SUCCESS;
	elem = *s->env;
	while (elem)
	{
		if (ft_strlen(elem->value) > 0)
		{
			ft_putstr_fd(elem->name, STDOUT_FILENO);
			ft_putstr_fd(elem->value, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		elem = elem->next;
	}
}
