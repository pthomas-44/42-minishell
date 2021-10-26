/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 13:51:28 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/22 13:20:44 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static char	check_quotes(char c, char quote)
{
	if ((c == '"' || c == '\'') && quote == 0)
		quote = c;
	else if (c == quote)
		quote = 0;
	return (quote);
}

static int	ft_countwords(const char *s, char c)
{
	int	nb;
	char	quote;

	nb = 0;
	while (*s)
	{
		quote = check_quotes(*s, quote);
		if (*s && *s != c && (*(s + 1) == c || *(s + 1) == 0) && !quote)
			nb++;
		s++;
	}
	return (nb);
}

static char	**ft_freetab(char **tab, int i)
{
	while (tab[--i])
		free(tab[i]);
	free(tab);
	return (0);
}

static char	**loop(char const **s, char **tab, int *i)
{
	const char	*start;
	char		quote;

	quote = 0;
	while (*(*s) && *(*s) == ' ' && !quote)
	{
		quote = check_quotes(*(*s), quote);
		(*s)++;
	}
	start = (*s);
	while (*(*s) && (*(*s) != ' ' || quote))
	{
		quote = check_quotes(*(*s), quote);
		(*s)++;
	}
	if ((*s) != start)
	{
		tab[*i] = malloc(sizeof(char) * ((*s) - start + 1));
		if (!tab[*i])
			return (ft_freetab(tab, *i));
		ft_strlcpy(tab[(*i)++], start, (*s) - start + 1);
	}
	return (tab);
}

char	**ft_split_quotes(char const *s)
{
	char		**tab;
	int			i;

	i = 0;
	tab = malloc(sizeof(char *) * (ft_countwords(s, ' ') + 1));
	if (!s || !tab)
		return (0);
	while (*s)
	{
		tab = loop(&s, tab, &i);
	}
	tab[i] = 0;
	return (tab);
}
