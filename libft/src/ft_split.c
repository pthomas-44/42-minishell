/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 04:16:43 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/02 23:49:38 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

static int	ft_countwords(const char *s, char c)
{
	int	nb;

	nb = 0;
	while (*s)
	{
		if (*s && *s != c && (*(s + 1) == c || *(s + 1) == 0))
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

char	**ft_split(char const *s, char c)
{
	int			i;
	const char	*start;
	char		**tab;

	i = 0;
	tab = ft_calloc(ft_countwords(s, c) + 1, sizeof(char *));
	if (!s || !tab)
		return (0);
	while (*s)
	{
		while (*s && *s == c)
			s++;
		start = s;
		while (*s && *s != c)
			s++;
		if (s != start)
		{
			tab[i] = malloc(sizeof(char) * (s - start + 1));
			if (!tab[i])
				return (ft_freetab(tab, i));
			ft_strlcpy(tab[i++], start, s - start + 1);
		}
	}
	tab[i] = 0;
	return (tab);
}
