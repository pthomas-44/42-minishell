/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mem2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/18 13:21:51 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/26 17:49:54 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"
#include <stdio.h>

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	void	*p;

	p = dst;
	if (src == 0 && dst == 0)
		return (0);
	while (len--)
	{
		if (src < dst)
			*(unsigned char *)(dst + len) = *(unsigned char *)(src + len);
		else
			*(unsigned char *)dst++ = *(unsigned char *)src++;
	}
	return (p);
}

void	*ft_memchr(const void *s, int c, size_t n)
{
	while (n--)
		if (*(unsigned char *)s++ == (unsigned char)c)
			return ((unsigned char *)(s - 1));
	return (0);
}

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	while (n--)
		if (*(unsigned char *)s1++ - *(unsigned char *)s2++ != 0)
			return (*(unsigned char *)(s1 - 1) - *(unsigned char *)(s2 - 1));
	return (0);
}

void	free_tab(char **split, size_t size)
{
	size_t	i;

	i = 0;
	if (split && size)
	{
		while (size && --size >= 0)
			free(split[size]);
		free(split);
	}
	else if (split)
	{
		while (split[i])
			i++;
		while (i && i-- >= 0)
			free(split[i]);
		free(split);
	}
}
