/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 17:23:47 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/03 00:23:03 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//~~ La loop qui affiche le prompt et envoie l'entree standard dans le parsing

static void	prompt_loop(t_structs *s)
{
	int	tmp_errno;

	signal(SIGINT, &sig_int);
	signal(SIGQUIT, &sig_quit);
	while (1)
	{
		tmp_errno = errno;
		s->parse_line[0] = readline(PROMPT);
		errno = tmp_errno;
		if (!s->parse_line[0])
			break ;
		if (s->parse_line[0][0])
			parsing(s, s->parse_line[0]);
		if (s->parse_line[0][0]
			&& ft_strcmp(s->parse_line[0], s->parse_line[1]))
			add_history(s->parse_line[0]);
		free(s->parse_line[1]);
		s->parse_line[1] = s->parse_line[0];
	}
	write(2, "exit\n", 6);
	free_all(s);
	exit(errno);
}

//~~ Initialisation de la liste de variables d'environnement

static void	set_env_list(t_structs *s, char **env)
{
	char		*tmp;
	t_env		*elem;
	size_t		i;

	*s->env = NULL;
	s->env_size = 0;
	i = 0;
	while (env[i])
	{
		if (env_new(s, env[i++]) == -1)
		{
			print_error("malloc: ", NULL, NULL, ENOMEM);
			free_all(s);
		}
	}
	elem = *s->env;
	while (elem && ft_strcmp(elem->name, "SHLVL"))
		elem = elem->next;
	if (!ft_strcmp(elem->name, "SHLVL"))
	{
		tmp = ft_strjoin_f0("=",
				ft_nbtobase(ft_atoi(elem->value + 1) + 1, "0123456789"));
		free(elem->value);
		elem->value = tmp;
	}
}

//~~ Initialisation des parametres du terminal

static void	set_new_terminal(t_structs *s)
{
	tcgetattr(STDIN_FILENO, &s->term[OLD]);
	tcgetattr(STDIN_FILENO, &s->term[NEW]);
	s->term[NEW].c_cc[VQUIT] = 0;
	s->term[NEW].c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &s->term[NEW]);
}

//~~ Initialisation de la structure de controle

static void	init_control_struct(t_structs *s, char **env)
{
	ft_bzero(s, sizeof(t_structs));
	set_new_terminal(s);
	s->parse_line[0] = NULL;
	s->parse_line[1] = NULL;
	s->cmds = NULL;
	s->env = ft_calloc(1, sizeof(t_env));
	if (!s->env)
	{
		print_error("malloc: ", NULL, NULL, ENOMEM);
		free_all(s);
	}
	set_env_list(s, env);
}

int	main(int ac, char **av, char **env)
{
	t_structs	s;

	(void)av;
	if (ac != 1)
	{
		print_error(NULL, NULL, NULL, E2BIG);
		exit(errno);
	}
	init_control_struct(&s, env);
	prompt_loop(&s);
	return (0);
}
