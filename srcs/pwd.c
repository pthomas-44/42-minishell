/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 13:43:33 by mberne            #+#    #+#             */
/*   Updated: 2021/10/11 13:44:07 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	set_pwd(t_structs *s, int index_pwd)
{
	char	*pwd;

	pwd = malloc(sizeof(char) * 100); // 100 au pif...
	if (!pwd)
		ft_exit(s, "malloc", EXIT_FAILURE);
	pwd = getcwd(pwd, 100);
	if (!pwd)
		ft_exit(s, "getcwd", EXIT_FAILURE);
	s->env[index_pwd] = ft_strdup(pwd);
	if (!s->env[index_pwd])
	{
		free(pwd);
		ft_exit(s, "malloc", EXIT_FAILURE);
	}
	free(pwd);
}

int	set_oldpwd(t_structs *s)
{
	int	i;
	int	j;

	i = -1;
	while (s->env[++i])
	{
		if (!ft_strncmp(s->env[i], "PWD=", 4))
		{
			j = i;
			while (s->env[j])
			{
				if (!ft_strncmp(s->env[j], "OLDPWD=", 4))
				{
					s->env[j] = NULL;
					s->env[j] = ft_strjoin_f0("OLDPWD=", s->env[i] + 4);
					if (!s->env[j])
						ft_exit(s, "malloc", EXIT_FAILURE);
					break ;
				}
				j++;
			}
			break ;
		}
	}
	return (i - 1);
}

void	ft_pwd(t_structs *s, int out)
{
	int		index_pwd;

	index_pwd = set_oldpwd(s);
	set_pwd(s, index_pwd);
	write(out, s->env[index_pwd], ft_strlen(s->env[index_pwd]));
	write(out, "\n", 1);
}
