/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 17:23:47 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/26 12:26:22 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_signals	g_sigs;

//~~ La loop qui affiche le prompt et envoie l'entree standard dans le parsing

static void	prompt_loop(t_structs *s)
{
	char	*line;
	char	*tmp;

	line = NULL;
	tmp = NULL;
	signal(SIGINT, &sig_int);
	signal(SIGQUIT, &sig_quit);
	while (1)
	{
		line = readline(PROMPT);
		if (!line)
			break ;
		parsing(s, line);
		if (line[0] && ft_strcmp(line, tmp))
			add_history(line);
		free(tmp);
		tmp = line;
	}
	free(tmp);
	ft_exit(s, "", EXIT_SUCCESS);
}

//~~ Initialisation de la structure de controle

static void	init_control_struct(t_structs *s, char **env)
{
	size_t	i;

	ft_bzero(s, sizeof(t_structs));
	s->cmds = NULL;
	s->env = malloc(sizeof(t_env));
	if (!s->env)
		ft_exit(s, "malloc", EXIT_FAILURE);
	s->env_size = 0;
	i = 0;
	while (env[i])
	{
		if (env_new(s, env[i]) == -1)
			ft_exit(s, "malloc", EXIT_FAILURE);
		i++;
	}
}

int	main(int ac, char **av, char **env)
{
	t_structs	s;

	// tcgetattr(STDIN_FILENO, &s.term.basic);
	// s.term.new = s.term.basic;
	// s.term.new.c_cc[VQUIT] = 0;
	// tcsetattr(STDOUT_FILENO, TCSANOW, &s.term.new);
	(void)av;
	init_control_struct(&s, env);
	if (ac != 1)
		ft_exit(&s, "error: too many arguments\n", EXIT_MISSING);
	prompt_loop(&s);
	ft_exit(&s, "", 0);
	return (0);
}
