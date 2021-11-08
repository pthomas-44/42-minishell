/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 13:51:28 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/08 15:59:11 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ~~ Recupere le nombre de mots dans la chaine

static size_t	ft_countwords(const char *s)
{
	size_t	nb;
	size_t	i;
	char	quote;

	nb = 0;
	i = 0;
	quote = 0;
	while (s[i])
	{
		if (i == 0 || s[i - 1] != '\\')
			quote = check_quotes(s[i], quote);
		if (s[i] && s[i] != ' ' && (s[i + 1] == ' '
				|| s[i + 1] == 0) && !quote)
			nb++;
		i++;
	}
	return (nb);
}

// ~~ Recupere le prochain mot

static char	*get_next_word(char **str)
{
	size_t	i;
	char	*start;
	char	quote;

	i = 0;
	quote = 0;
	while ((*str)[i] && (*str)[i] == ' ' && !quote)
	{
		if (i == 0 || (*str)[i - 1] != '\\')
			quote = check_quotes((*str)[i], quote);
		i++;
	}
	start = (*str) + i;
	while ((*str)[i] && ((*str)[i] != ' ' || quote))
	{
		if (i == 0 || (*str)[i - 1] != '\\')
			quote = check_quotes((*str)[i], quote);
		i++;
	}
	(*str) += i;
	return (ft_substr(start, 0, (*str) - start));
}

// ~~ Initialise split

static char	**init(char **cmd, size_t *nb_of_words)
{
	char	**split;

	if (!cmd || !cmd[0])
	{
		free_tab(&cmd, 0);
		return (NULL);
	}
	*nb_of_words = ft_countwords(cmd[0]);
	split = malloc(sizeof(char *) * (*nb_of_words + 1));
	if (!split)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		free_tab(&cmd, 0);
		return (NULL);
	}
	return (split);
}

// ~~ Split la commande et ses arguments

char	**split_cmd(char **cmd)
{
	char		**split;
	size_t		nb_of_words;
	size_t		i;
	char		*str;

	split = init(cmd, &nb_of_words);
	if (!split)
	{
		free_tab(&cmd, 0);
		return (NULL);
	}
	str = cmd[0];
	i = 0;
	while (i < nb_of_words)
	{
		split[i] = get_next_word(&str);
		str++;
		i++;
	}
	split[i] = NULL;
	free_tab(&cmd, 0);
	return (split);
}
