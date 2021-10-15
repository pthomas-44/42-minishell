/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 10:31:13 by mberne            #+#    #+#             */
/*   Updated: 2021/10/14 14:36:27 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_unset(t_structs *s, t_cmd current)
{
	t_env	*unset;
	t_env	*prec;

	if (!current.cmd[1])
		return ;
	if (!ft_strcmp((*s->env)->name, current.cmd[1]))
	{
		unset = *s->env;
		*s->env = (*s->env)->next;
		free(unset->name);
		free(unset->value);
		free(unset);
	}
	else
	{
		prec = *s->env;
		unset = (*s->env)->next;
		while (unset)
		{
			if (!ft_strcmp(unset->name, current.cmd[1]))
			{
				prec->next = unset->next;
				free(unset->name);
				free(unset->value);
				free(unset);
				break ;
			}
			prec = unset;
			unset = unset->next;
		}
	}
}
