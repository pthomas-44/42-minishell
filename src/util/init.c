/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 04:34:49 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/08 18:45:43 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//~~ Initialisation du niveau de shell

static void	set_shlvl(t_structs *s)
{
	t_env		*elem;
	char		*tmp;

	elem = *s->env;
	while (elem && ft_strcmp(elem->name, "SHLVL"))
		elem = elem->next;
	if (elem && !ft_strcmp(elem->name, "SHLVL"))
	{
		tmp = ft_strjoin_f2("=",
				ft_nbtobase(ft_atoi(elem->value + 1) + 1, "0123456789"));
		free(elem->value);
		elem->value = tmp;
	}
	else
		env_new(s, "SHLVL=1");
}

//~~ Initialisation de la liste de variables d'environnement

static void	set_env_list(t_structs *s, char **env)
{
	size_t		i;

	*s->env = NULL;
	s->env_size = 0;
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], "OLDPWD=", 7))
		{
			if (env_new(s, "OLDPWD") == -1)
			{
				print_error("malloc: ", NULL, NULL, ENOMEM);
				free_all(s, 0);
			}
			i++;
		}
		else if (env_new(s, env[i++]) == -1)
		{
			print_error("malloc: ", NULL, NULL, ENOMEM);
			free_all(s, 0);
		}
	}
	set_shlvl(s);
}

//~~ Initialisation des parametres du terminal

static void	set_new_terminal(t_structs *s)
{
	if (tcgetattr(STDIN_FILENO, &s->term[OLD]) == -1)
		print_error("termios: ", NULL, NULL, errno);
	if (tcgetattr(STDIN_FILENO, &s->term[NEW]) == -1)
		print_error("termios: ", NULL, NULL, errno);
	s->term[NEW].c_cc[VQUIT] = 0;
	s->term[NEW].c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &s->term[NEW]) == -1)
		print_error("termios: ", NULL, NULL, errno);
	signal(SIGINT, &sig_int);
	signal(SIGQUIT, SIG_IGN);
}

//~~ Initialisation de la structure de controle

void	init_control_struct(t_structs *s, char **env)
{
	g_numberr = 0;
	ft_bzero(s, sizeof(t_structs));
	set_new_terminal(s);
	s->parse_line[0] = NULL;
	s->parse_line[1] = NULL;
	s->cmds = NULL;
	s->env = ft_calloc(1, sizeof(t_env));
	if (!s->env)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		free_all(s, 0);
	}
	set_env_list(s, env);
}
