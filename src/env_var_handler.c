/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:25:14 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/02 23:49:38 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//~~ Gestion des quotes

char	check_quotes(char c, char quote)
{
	if ((c == '"' || c == '\'') && quote == 0)
		quote = c;
	else if (c == quote)
		quote = 0;
	return (quote);
}

//~~ Inhibe les operateurs dans les noms de variables

static char	*handle_operands(char *value, char *charset)
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
	if (!new)
		perror("malloc");
	return (new);
}

//~~ Remplace la variable par sa valeur

char	*replace_var(char *line, size_t i, t_env *var)
{
	char	*new;

	new = ft_substr(line, 0, i);
	if (line[i + 1] == '?')
	{
		new = ft_strjoin_f3(new, ft_nbtobase(errno, "0123456789"));
		new = ft_strjoin_f1(new, &line[i + 2]);
	}
	else if (var)
	{
		new = ft_strjoin_f3(new, ft_strjoin_f2("\"", ft_strjoin_f1(
						handle_operands(var->value + 1, "<>|"), "\"")));
		new = ft_strjoin_f1(new, &line[i] + ft_strlen(var->name) + 1);
	}
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
				|| line[i + 1] == '_' || line[i + 1] == '?'))
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
