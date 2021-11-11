/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 13:42:08 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/11 10:41:47 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//~~ Cree et ajoute un nouvel element a la liste env

int	env_new(t_structs *s, char *var)
{
	t_env	*new;
	t_env	*current;

	new = ft_calloc(1, sizeof(t_env));
	if (!new)
		return (-1);
	new->name = ft_substr(var, 0, ft_strchr(var, '=') - var);
	if (!new->name)
		return (-1);
	new->value = NULL;
	if (ft_strchr(var, '='))
		new->value = ft_substr(ft_strchr(var, '='), 0, ft_strlen(var));
	new->next = NULL;
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
	return (0);
}

//~~ Supprime un element de la liste env

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

//~~ Supprime la liste env

void	env_clear(t_structs *s)
{
	t_env	*current;

	while (s->env && *s->env)
	{
		current = *s->env;
		while (current->next)
			current = current->next;
		env_del(s, current);
	}
	s->env_size = 0;
	free(s->env);
}

//~~ Trouve un élément dans la liste

t_env	*find_env_var(t_structs *s, char *name)
{
	t_env	*elem;

	elem = *s->env;
	while (elem)
	{
		if (!ft_strcmp(elem->name, name))
			break ;
		elem = elem->next;
	}
	return (elem);
}

//~~ Converti la liste env en tableau de chaines de caracteres

char	**list_to_char(t_structs *s)
{
	size_t	i;
	t_env	*current;
	char	**envp;

	i = 0;
	current = *s->env;
	envp = ft_calloc(s->env_size + 1, sizeof(char *));
	if (!envp)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		return (NULL);
	}
	while (current)
	{
		envp[i] = ft_strjoin_f0(current->name, current->value);
		if (!envp[i++])
		{
			print_error("malloc: ", NULL, NULL, ENOMEM);
			free_tab(&envp, 0);
			return (NULL);
		}
		current = current->next;
	}
	envp[i] = NULL;
	return (envp);
}
