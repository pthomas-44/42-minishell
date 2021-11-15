/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dev <dev@student.42lyon.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 13:43:33 by mberne            #+#    #+#             */
/*   Updated: 2021/11/15 21:25:00 by dev              ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ~~ Remplace le '~' par le HOME

static int	replace_by_home_path(t_structs *s, char **path)
{
	t_env	*elem;

	elem = find_env_var(s, "HOME");
	if (!elem || !elem->value)
		return (-1);
	else
		*(path) = ft_strdup(elem->value + 1);
	return (0);
}

//~~ Set les variables d'environnement PWD et OLDPWD

static void	set_pwd(t_structs *s, char *name)
{
	t_env	*current;
	char	cwd[MAXPATHLEN];

	if (!getcwd(cwd, MAXPATHLEN))
	{
		print_error("getcwd: ", NULL, NULL, errno);
		g_error_number = EXIT_FAILURE;
	}
	current = find_env_var(s, name);
	if (current)
	{
		free(current->value);
		current->value = ft_strjoin("=", ft_strdup(cwd), 2);
	}
	if (current && !current->value)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		g_error_number = EXIT_FAILURE;
	}
}

//~~ Built-in cd

void	bi_cd(t_structs *s, t_cmd *current)
{
	g_error_number = EXIT_SUCCESS;
	if (!current->cmd[1])
	{
		if (replace_by_home_path(s, &current->path) == -1)
		{
			print_error("cd: ", NULL, "HOME not set\n", EXIT_FAILURE);
			return ;
		}
	}
	else
		current->path = ft_strdup(current->cmd[1]);
	set_pwd(s, "OLDPWD");
	if (current->path && chdir(current->path) == -1)
	{
		print_error("cd: ", current->path, NULL, errno);
		g_error_number = EXIT_FAILURE;
	}
	else if (!current->path)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		g_error_number = EXIT_FAILURE;
	}
	set_pwd(s, "PWD");
}
