/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 04:34:49 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/11 11:41:02 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//~~ Verifie si les variables d'environnement sont manquantes

static void	check_if_exist(t_structs *s, bool *pwd, bool *oldpwd, bool *shlvl)
{
	t_env	*elem;
	char	*tmp;

	elem = find_env_var(s, "SHLVL");
	if (elem)
	{
		tmp = ft_strjoin_f2("=",
				ft_nbtobase(ft_atoi(elem->value + 1) + 1, "0123456789"));
		free(elem->value);
		elem->value = tmp;
		*shlvl = 1;
	}
	elem = find_env_var(s, "PWD");
	if (elem)
		*pwd = 1;
	elem = find_env_var(s, "OLDPWD");
	if (elem)
	{
		free(elem->value);
		elem->value = NULL;
		*oldpwd = 1;
	}
}

//~~ Cree les variables d'environnement manquantes

static void	create_env_var_if_not_exist(t_structs *s)
{
	char		cwd[MAXPATHLEN];
	char		*tmp;
	bool		pwd;
	bool		oldpwd;
	bool		shlvl;

	pwd = 0;
	oldpwd = 0;
	shlvl = 0;
	check_if_exist(s, &pwd, &oldpwd, &shlvl);
	if (!pwd)
	{
		getcwd(cwd, MAXPATHLEN);
		tmp = ft_strjoin_f0("PWD=", cwd);
		env_new(s, tmp);
		free(tmp);
	}
	if (!oldpwd)
		env_new(s, "OLDPWD");
	if (!shlvl)
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
		if (env_new(s, env[i]) == -1)
		{
			print_error("malloc: ", NULL, NULL, ENOMEM);
			free_all(s, 0);
		}
		i++;
	}
	create_env_var_if_not_exist(s);
}

//~~ Initialisation des parametres du terminal

static void	set_new_terminal(t_structs *s)
{
	if (tcgetattr(STDIN_FILENO, &s->term[OLD]) == -1)
		print_error("termios: ", NULL, NULL, errno);
	s->term[NEW] = s->term[OLD];
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
		exit(g_numberr);
	}
	set_env_list(s, env);
}
