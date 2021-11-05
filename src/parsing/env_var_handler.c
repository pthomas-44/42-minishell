/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:25:14 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/05 16:16:10 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//~~ Inhibe les operateurs dans les noms de variables

static size_t	get_new_size(char *value, char *charset)
{
	size_t	i;
	size_t	size;

	i = 0;
	size = 0;
	while (value[i])
	{
		if (!ft_strchr(charset, value[i]))
			size++;
		size++;
		i++;
	}
	return (size);
}

static char	*handle_operands(char *value, char *charset)
{
	size_t	i;
	size_t	j;
	char	*new;

	i = 0;
	j = 0;
	new = ft_calloc(get_new_size(value, charset), sizeof(char));
	if (!new)
		print_error("malloc: ", NULL, NULL, ENOMEM);
	while (value[i])
	{
		if (ft_strchr(charset, value[i]))
		{
			new[j] = '\\';
			j++;
		}
		new[j] = value[i];
		j++;
		i++;
	}
	return (new);
}

//~~ Remplace la variable d'environnement par sa valeur

static char	*replace_var(char *line, size_t i, t_env *var)
{
	char	*new;

	new = ft_substr(line, 0, i);
	if (line[i + 1] == '?')
	{
		new = ft_strjoin_f3(new, ft_nbtobase(g_numberr, "0123456789"));
		new = ft_strjoin_f1(new, &line[i + 2]);
	}
	else if (var)
		new = ft_strjoin_f3(new,
				handle_operands(ft_strdup(var->value + 1), "\"\'\\<>|"));
	else
	{
		i++;
		while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
			i++;
		if (line[i])
			new = ft_strjoin_f1(new, &line[i]);
	}
	free(line);
	return (new);
}

//~~ Trouve la variable correspondante dans t_env

static t_env	*find_var(t_structs *s, char *line)
{
	t_env	*current;
	char	*name;

	name = line;
	while (ft_isalnum(*name) || *name == '_')
		name++;
	name = ft_substr(line, 0, name - line);
	if (!name)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		return (NULL);
	}
	current = *s->env;
	while (current->next
		&& ft_strncmp(current->name, name, ft_strlen(name) + 1))
		current = current->next;
	if (ft_strncmp(current->name, line, ft_strlen(name)))
		current = NULL;
	free(name);
	return (current);
}

//~~ Remplace les variables d'environnement pas leurs valeurs

char	*replace_env_variables(t_structs *s, char *line)
{
	t_env	*var;
	size_t	i;
	char	quote;

	i = 0;
	quote = 0;
	while (line && line[i])
	{
		quote = check_quotes(line[i], quote);
		if (line[i] == '$' && (ft_isalpha(line[i + 1])
				|| line[i + 1] == '_' || line[i + 1] == '?') && quote != '\'')
		{
			var = find_var(s, &line[i + 1]);
			line = replace_var(line, i, var);
			if (!line)
				return (NULL);
		}
		else if (line[i])
			i++;
	}
	return (line);
}
