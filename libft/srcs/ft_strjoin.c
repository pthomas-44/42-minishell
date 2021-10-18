/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 00:39:29 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/18 14:58:09 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static char	*error_handling(const char *s1, const char *s2,
			const char *f1, const char *f2)
{
	char	*new;

	new = NULL;
	if (!s1 && !s2)
		new = NULL;
	else if (!s1)
	{
		new = ft_strdup(s2);
		free((char *)f2);
	}
	else if (!s2)
	{
		new = ft_strdup(s1);
		free((char *)f1);
	}
	return (new);
}

char	*ft_strjoin_f0(const char *s1, const char *s2)
{
	int		i;
	int		j;
	char	*l;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (error_handling(s1, s2, NULL, NULL));
	l = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!l)
		return (NULL);
	while (s1[j])
		l[i++] = s1[j++];
	j = 0;
	while (s2[j])
		l[i++] = s2[j++];
	l[i] = '\0';
	return (l);
}

char	*ft_strjoin_f1(const char *s1, const char *s2)
{
	int		i;
	int		j;
	char	*l;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (error_handling(s1, s2, s1, NULL));
	l = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!l)
		return (NULL);
	while (s1[j])
		l[i++] = s1[j++];
	j = 0;
	while (s2[j])
		l[i++] = s2[j++];
	l[i] = '\0';
	free((char *)s1);
	return (l);
}

char	*ft_strjoin_f2(const char *s1, const char *s2)
{
	int		i;
	int		j;
	char	*l;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (error_handling(s1, s2, NULL, s2));
	l = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!l)
		return (NULL);
	while (s1[j])
		l[i++] = s1[j++];
	j = 0;
	while (s2[j])
		l[i++] = s2[j++];
	l[i] = '\0';
	free((char *)s2);
	return (l);
}

char	*ft_strjoin_f3(const char *s1, const char *s2)
{
	int		i;
	int		j;
	char	*l;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (error_handling(s1, s2, s1, s2));
	l = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!l)
		return (NULL);
	while (s1[j])
		l[i++] = s1[j++];
	j = 0;
	while (s2[j])
		l[i++] = s2[j++];
	l[i] = '\0';
	free((char *)s1);
	free((char *)s2);
	return (l);
}
