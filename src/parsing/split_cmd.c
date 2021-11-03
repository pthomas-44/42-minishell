/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 13:51:28 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/03 01:15:50 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// ~~ Recupere le nombre de mots dans la chaine

static size_t	ft_countwords(const char *s)
{
	size_t	nb;
	char	quote;

	nb = 0;
	quote = 0;
	while (*s)
	{
		quote = check_quotes(*s, quote);
		if (*s && *s != ' ' && (*(s + 1) == ' ' || *(s + 1) == 0) && !quote)
			nb++;
		s++;
	}
	return (nb);
}

// ~~ Recupere le prochain mot dans la chaine

static char	**get_word(char *s, char **split, size_t *i, size_t *j)
{
	const char	*start;
	char		quote;

	quote = 0;
	while (s[*j] && s[*j] == ' ' && !quote)
		quote = check_quotes(s[(*j)++], quote);
	start = &s[*j];
	while (s[*j] && (s[*j] != ' ' || quote))
		quote = check_quotes(s[(*j)++], quote);
	if (&s[*j] != start)
	{
		split[*i] = malloc(sizeof(char) * (&s[*j] - start + 1));
		if (!split[*i])
		{
			perror("malloc");
			free_tab(split, 0);
			return (NULL);
		}
		ft_strlcpy(split[(*i)++], start, &s[*j] - start + 1);
	}
	return (split);
}

// ~~ Split en prenant en compte les quotes

char	**split_cmd(char **cmd)
{
	char		**split;
	size_t		i;
	size_t		j;

	i = 0;
	j = 0;
	if (!cmd || !cmd[0])
		return (NULL);
	split = malloc(sizeof(char *) * (ft_countwords(cmd[0]) + 1));
	if (!split)
	{
		perror("malloc");
		return (NULL);
	}
	while ((cmd[0])[j])
	{
		split = get_word((cmd[0]), split, &i, &j);
		if (!split)
		{
			free_tab(cmd, 0);
			cmd = NULL;
			return (NULL);
		}
	}
	split[i] = 0;
	free_tab(cmd, 0);
	return (split);
}
