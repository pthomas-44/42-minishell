/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 01:51:34 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/11 11:06:10 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//~~ Vérifie qu'une variable a le bon format selon POSIX

int	is_word(char *str)
{
	size_t	i;

	i = 0;
	if (ft_isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

//~~ Built-in unset

void	bi_unset(t_structs *s, t_cmd *current)
{
	t_env	*unset;
	size_t	i;

	i = 1;
	g_numberr = EXIT_SUCCESS;
	while (current->cmd[i])
	{
		if (!is_word(current->cmd[i]))
			print_error("unset: ", current->cmd[i],
				"not a valid identifier\n", EXIT_FAILURE);
		else
		{
			unset = find_env_var(s, current->cmd[i]);
			if (unset)
				env_del(s, unset);
		}
		i++;
	}
}
