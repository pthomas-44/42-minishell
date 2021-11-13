/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:25:14 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/13 20:47:01 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_operands(char *line, char *charset)
{
	size_t	size;
	size_t	i;
	char	*new;

	size = ft_strlen(line) + ft_count_occurences(line, charset);
	new = ft_calloc(size + 1, sizeof(char));
	if (!new)
		return (NULL);
	i = 0;
	while (*line)
	{
		if (ft_strchr(charset, *line))
		{
			new[i] = '\\';
			i++;
		}
		new[i] = *line;
		line++;
		i++;
	}
	new[i] = '\0';
	return (new);
}

//~~ Remplace la variable d'environnement par sa valeur

static char	*replace_var(char *line, size_t i, t_env *var)
{
	char	*new;

	new = ft_substr(line, 0, i);
	if (line[i + 1] == '?')
	{
		new = ft_strjoin_f3(new, ft_nbtobase(g_error_number, "0123456789"));
		new = ft_strjoin_f1(new, &line[i + 2]);
	}
	else if (var)
	{
		new = ft_strjoin_f3(new, handle_operands(var->value + 1, "\\\'\"<>|"));
		new = ft_strjoin_f1(new, &line[i] + ft_strlen(var->name) + 1);
	}
	else if (line[++i])
	{
		while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
			i++;
		new = ft_strjoin_f1(new, &line[i]);
	}
	free(line);
	return (new);
}

//~~ Trouve la variable correspondante dans t_env

static t_env	*get_var_node(t_structs *s, char *line)
{
	t_env	*elem;
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
	elem = find_env_var(s, name);
	free(name);
	return (elem);
}

//~~ Passe le heredoc

static void	skip_heredoc(char *line, size_t *i)
{
	(*i) += 2;
	while (line[*i] == ' ')
		(*i)++;
	while (line[*i] && !ft_strchr("<>| ", line[*i]))
		(*i)++;
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
		if (line[i] == '<' && line[i + 1] == '<')
			skip_heredoc(line, &i);
		if (line[i] == '$' && (ft_isalpha(line[i + 1])
				|| line[i + 1] == '_' || line[i + 1] == '?') && quote != '\'')
		{
			var = get_var_node(s, &line[i + 1]);
			line = replace_var(line, i, var);
			if (!line)
				return (NULL);
		}
		else if (line[i])
			i++;
	}
	return (line);
}
