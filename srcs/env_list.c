/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 13:42:08 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/14 14:15:40 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	env_init(t_structs *s, char **env)
{
	(void)s;
	(void)env;
	return ;
}

void	env_new(t_structs *s, char *var)
{
	t_env	*current;
	t_env	*new;
	size_t	i;

	new = malloc(sizeof(t_env));
	while (var && var[i] && var[i] != '=')
		i++;
	new->name = ft_substr(var, 0, i - 1);
	var += i + 1;
	new->value = ft_substr(var, 0, ft_strlen(var));
	new->next = NULL;
	if (!new->name || !new->value)
		ft_exit(s, "malloc", EXIT_FAILURE);
	current = *s->env;
	while (current->next)
		current = current->next;
	current->next = new;
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
	free(elem->name);
	free(elem->value);
	free(elem);
}

void	env_clear(t_structs *s)
{
	(void)s;
	return ;
}
