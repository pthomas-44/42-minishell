/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 14:54:51 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/16 15:03:50 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//~~ Gestion des quotes

char	check_quotes(char c, char quote)
{
	if ((c == '"' || c == '\'') && quote == 0)
		quote = c;
	else if (c == quote)
		quote = 0;
	return (quote);
}

//~~ Enleve le caractere str[i] de la chaine

char	*remove_char(char *str, size_t i)
{
	char	*new;

	if (i)
		new = ft_substr(str, 0, i);
	else
		new = ft_strdup("");
	if (!new)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		return (str);
	}
	new = ft_strjoin(new, str + i + 1, 1);
	if (!new)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		return (str);
	}
	free(str);
	return (new);
}

void	remove_quotes_and_backslash(char **str)
{
	size_t	i;
	char	quote;

	i = 0;
	quote = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == '\\')
			(*str) = remove_char((*str), i);
		else if (!quote && ((*str)[i] == '"' || (*str)[i] == '\''))
		{
			quote = (*str)[i];
			(*str) = remove_char((*str), i--);
		}
		else if ((*str)[i] == quote)
		{
			quote = 0;
			(*str) = remove_char((*str), i--);
		}
		i++;
	}
}

//~~ Enleve les quotes delimitatives dans les arguments

void	format_cmd_array(char **cmd)
{
	size_t	i;

	i = 0;
	while (cmd && cmd[i])
	{
		remove_quotes_and_backslash(&cmd[i]);
		i++;
	}
}

//~~ Recupere des arguments dans la ligne de commandes

char	*get_args(char *line, char *charset)
{
	char	*start;
	char	quote;

	start = line;
	quote = 0;
	while (*line && ((!ft_strchr(charset, *line)
				|| (line == start || *(line - 1) == '\\')) || quote))
	{
		if ((line == start || *(line - 1) != '\\')
			&& (*line == '"' || *line == '\'') && quote == 0)
			quote = *line;
		else if (*line == quote && *(line - 1) != '\\')
			quote = 0;
		line++;
	}
	start = ft_substr(start, 0, line - start);
	if (!start)
		print_error("malloc: ", NULL, NULL, ENOMEM);
	return (start);
}
