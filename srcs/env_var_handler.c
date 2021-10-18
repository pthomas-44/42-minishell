/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:25:14 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/18 17:14:44 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*handle_operands(char *value, char *charset)
{
	char	*new;
	size_t	i;

	new = NULL;
	while (*value)
	{
		i = 0;
		while (value[i] && !ft_strchr(charset, value[i]))
			i++;
		if (value[i])
		{
			new = ft_substr(value, 0, i);
			new = ft_strjoin_f3(new, ft_strjoin_f2("\"",
						ft_strjoin_f1(ft_substr(&value[i], 0, 1), "\"")));
			value += i + 1;
		}
		else
		{
			new = ft_strjoin_f1(new, value);
			break ;
		}
	}
	return (new);
}

char	*replace_var(char *line, size_t i, t_env *var, char c)
{
	char	*new;

	new = ft_calloc(ft_strlen(line), sizeof(char));
	if (!new)
		return (NULL);
	ft_strlcpy(new, line, i + 1);
	if (var)
	{
		new = ft_strjoin_f3(new, handle_operands(var->value + 1, "<>|"));
		new = ft_strjoin_f1(new, line + i + ft_strlen(var->name) + 1);
	}
	else if (c == '?')
	{
		new = ft_strjoin_f3(new, ft_nbtobase(errno, "0123456789"));
		new = ft_strjoin_f1(new, line + i + 2);
	}
	else
	{
		while (line[i] && line[i] != '"' && line[i] != ' ' && line[i] != '\'')
			i++;
		new = ft_strjoin_f1(new, line + i);
	}
	return (new);
}

t_env	*find_var(t_structs *s, char *line)
{
	t_env	*current;
	char	*name;
	size_t	i;

	i = 0;
	while (line[i] && line[i] != ' ' && line[i] != '"' && line[i] != '\'')
		i++;
	name = ft_substr(line, 0, i);
	current = *s->env;
	while (current->next
		&& ft_strncmp(current->name, name, ft_strlen(name) + 1))
		current = current->next;
	if (ft_strncmp(current->name, line, ft_strlen(name)))
	{
		free(name);
		return (NULL);
	}
	else
	{
		free(name);
		return (current);
	}
}

//~~ Remplace les variables d'environnement pas leurs valeurs

char	*replace_env_variables(t_structs *s, char *line)
{
	t_env	*var;
	size_t	i;
	char	quote;
	char	*new;

	i = 0;
	quote = 0;
	new = NULL;
	while (line && line[i])
	{
		if ((line[i] == '"' || line[i] == '\'') && quote == 0)
			quote = line[i];
		else if (line[i] == quote)
			quote = 0;
		if (line[i] == '$' && line[i + 1] != ' '
			&& line[i + 1] != 0 && quote != '\'')
		{
			var = find_var(s, &line[i + 1]);
			new = replace_var(line, i, var, line[i + 1]);
		}
		i++;
	}
	if (!new)
		new = ft_strdup(line);
	return (new);
}
