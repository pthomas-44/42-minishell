/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 14:54:51 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/29 18:01:25 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//~~ Enleve le caractere str[i] de la chaine

static char	*remove_char(char *str, size_t i)
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
	new = ft_strjoin_f1(new, str + i + 1);
	if (!new)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		return (str);
	}
	free(str);
	return (new);
}

//~~ Enleve les quotes delimitatives dans les arguments

void	remove_quotes(char **cmd)
{
	size_t	i;
	size_t	j;
	char	quote;

	i = 0;
	quote = 0;
	while (cmd && cmd[i])
	{
		j = 0;
		while (cmd[i][j])
		{
			if (!quote && (cmd[i][j] == '"' || cmd[i][j] == '\''))
			{
				quote = cmd[i][j];
				cmd[i] = remove_char(cmd[i], j--);
			}
			else if (cmd[i][j] == quote)
			{
				quote = 0;
				cmd[i] = remove_char(cmd[i], j--);
			}
			j++;
		}
		i++;
	}
}

//~~ Passe des espaces successifs dans une chaine de caracteres

void	skip_spaces(char **line)
{
	while (**line == ' ')
		(*line)++;
}

//~~ Recupere des arguments dans la ligne de commandes

char	*get_args(char *line, bool is_file)
{
	char	*start;
	char	quote;

	start = line;
	quote = 0;
	while (*line && (((!is_file || *line != ' ' )
				&& *line != '<' && *line != '>' && *line != '|') || quote))
	{
		if ((*line == '"' || *line == '\'') && quote == 0)
			quote = *line;
		else if (*line == quote)
			quote = 0;
		line++;
	}
	start = ft_substr(start, 0, line - start);
	if (!start)
		print_error("malloc: ", NULL, NULL, ENOMEM);
	return (start);
}
