/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 13:42:08 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/15 14:07:56 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	env_new(t_structs *s, char *var)
{
	t_env	*new;
	size_t	i;
	t_env	*current;

	new = malloc(sizeof(t_env));
	i = 0;
	if (!new)
		ft_exit(s, "malloc", EXIT_FAILURE);
	while (var && var[i] && var[i] != '=')
		i++;
	new->name = ft_substr(var, 0, i);
	new->value = ft_substr(var + i, 0, ft_strlen(var));
	new->next = NULL;
	if (!new->name || !new->value)
		ft_exit(s, "malloc", EXIT_FAILURE);
	if (s->env_size)
	{
		current = *s->env;
		while (current->next)
			current = current->next;
		current->next = new;
	}
	else
		*s->env = new;
	s->env_size++;
}

void	env_del(t_structs *s, t_env *elem)
{
	t_env	*current;

	current = *s->env;
	if (current == elem)
		*s->env = current->next;
	else
	{
		while (current->next != elem)
			current = current->next;
		current->next = elem->next;
	}
	s->env_size--;
	free(elem->name);
	free(elem->value);
	free(elem);
}

void	env_clear(t_structs *s)
{
	t_env	*current;

	while (*s->env)
	{
		current = *s->env;
		while (current->next)
			current = current->next;
		env_del(s, current);
	}
	s->env_size = 0;
	free(s->env);
}
