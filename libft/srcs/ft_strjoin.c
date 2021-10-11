/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/25 00:39:29 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/11 12:44:46 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strjoin_f0(const char *s1, const char *s2)
{
	int		i;
	int		j;
	char	*l;

	i = 0;
	j = 0;
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
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
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
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
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
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
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
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
