/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 16:01:23 by mberne            #+#    #+#             */
/*   Updated: 2021/10/15 19:07:25 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_option_n(char *arg) // avec la modif' ligne 40 tu peux supprimer cette fonction
{
	size_t	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void	ft_echo(t_cmd current)
{
	size_t	i;
	int		option;	// remplacer par "bool	option;"

	i = 1;
	option = 0;
	if (current.cmd[i])
	{
		while (!ft_strncmp(current.cmd[i], "-", 1)) // remplacer par "while (current.cmd[i] && current.cmd[i][0] == '-'))"	Sinon ca segfault sur "echo -n" sans le "current.cmd[i]", Ca me rassure je suis pas le seul a faire des strcmp pour un caractere des fois xD on est bete parreil :p
		{
			if (check_option_n(current.cmd[i] + 1)) // remplacer par "if (current.cmd[i][1] == 'n' && current.cmd[i][2] == '\0')""
				option = 1;
			else
				break ;
			i++;
		}
		while (current.cmd[i])
		{
			write(current.fd_out, current.cmd[i], ft_strlen(current.cmd[i])); // il manque le dup2 ici chef
			i++;
			if (current.cmd[i])
				write(current.fd_out, " ", 1);
		}
	}
	if (!option)
		write(current.fd_out, "\n", 1);
}

void	ft_pwd(t_cmd current)
{
	char	cwd[MAXPATHLEN];

	getcwd(cwd, MAXPATHLEN);
	write(current.fd_out, cwd, ft_strlen(cwd));
	write(current.fd_out, "\n", 1);
}

void	ft_env(t_structs *s, t_cmd current)
{
	t_env	*list;	// remplacer par "t_env	*elem;"

	list = *s->env;
	while (list)
	{
		if (ft_strlen(list->value) > 0)
		{
			write(current.fd_out, list->name, ft_strlen(list->name));
			write(current.fd_out, list->value, ft_strlen(list->value));
			write(current.fd_out, "\n", 1);
		}
		list = list->next;
	}
}

void	ft_unset(t_structs *s, t_cmd current)
{
	t_env	*unset;

	if (!current.cmd[1])
		return ;
	unset = *s->env;
	while (unset)
	{
		if (!ft_strcmp(unset->name, current.cmd[1]))
		{
			env_del(s, unset);
			break ; // remplacer par return ; (meme chose, juste plus propre imho)
		}
		unset = unset->next;
	}
}
