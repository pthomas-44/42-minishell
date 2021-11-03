/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 13:51:28 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/03 12:50:38 by pthomas          ###   ########lyon.fr   */
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

// ~~ Recupere le prochain mot

char	*get_next_word(char **str)
{
	char	*start;
	char	quote;

	quote = 0;
	while (*(*str) && *(*str) == ' ' && !quote)
		quote = check_quotes(*(*str)++, quote);
	start = (*str);
	while (*(*str) && (*(*str) != ' ' || quote))
		quote = check_quotes(*(*str)++, quote);
	return (ft_substr(start, 0, (*str) - start));
}

// ~~ Initialise split

char	**init(char **cmd, size_t *nb_of_words)
{
	char	**split;

	if (!cmd || !cmd[0])
	{
		free_tab(cmd, 0);
		return (NULL);
	}
	*nb_of_words = ft_countwords(cmd[0]);
	split = malloc(sizeof(char *) * (*nb_of_words + 1));
	if (!split)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		free_tab(cmd, 0);
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
		free_tab(cmd, 0);
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
	free_tab(cmd, 0);
	return (split);
}
