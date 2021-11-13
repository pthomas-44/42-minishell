/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_misc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 15:18:51 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/13 20:48:00 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_framing(int min, int nb, int max)
{
	if (min <= nb && nb <= max)
		return (1);
	else
		return (0);
}

float	ft_framingf(float min, float nb, float max)
{
	if (min <= nb && nb <= max)
		return (1);
	else
		return (0);
}

size_t	ft_count_occurences(char *str, char *charset)
{
	size_t	nb_of_occ;

	nb_of_occ = 0;
	while (*str)
	{
		if (ft_strchr(charset, *str))
			nb_of_occ++;
		str++;
	}
	return (nb_of_occ);
}
