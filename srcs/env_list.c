/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 13:42:08 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/15 09:55:01 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	env_init(t_structs *s, char **env)
{
	int	i;

	i = 0;
	s->env_size = 0;
	while (env[i])
	{
		env_new(s, env[i]);
		i++;
	}
	return ;
}

void	env_new(t_structs *s, char *var)
{
	t_env	*current;
	t_env	*new;
	size_t	i;

	new = malloc(sizeof(t_env));
	if (!new)
		ft_exit(s, "malloc", EXIT_FAILURE);
	while (var && var[i] && var[i] != '=')
		i++;
	new->name = ft_substr(var, 0, i - 1);
	var += i + 1;
	new->value = ft_substr(var, 0, ft_strlen(var));
	new->next = NULL;
	if (!new->name || !new->value)
		ft_exit(s, "malloc", EXIT_FAILURE);
	current = *s->env;
	if (current)
	{
		while (current->next)
			current = current->next;
		current->next = new;
	}
	else
		current = new;
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
}
