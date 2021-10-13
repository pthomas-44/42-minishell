/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 17:23:47 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/13 10:43:20 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//~~ La loop qui affiche le prompt et envoie l'entrée standard dans le parsing

void	prompt_loop(t_structs *s)
{
	char	*line;
	char	*tmp;

	line = NULL;
	tmp = NULL;
	signal(SIGINT, &sig_int);
	signal(SIGQUIT, &sig_quit);
	while (ft_strcmp(line, "exit"))
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

void	init_control_struct(t_structs *s, char **env)
{
	(void)env;
	ft_bzero(s, sizeof(t_structs));
	s->cmds = NULL;
	// env init
}

int	main(int ac, char **av, char **env)
{
	t_structs	s;

	(void)av;
	(void)env;
	if (ac != 1)
		ft_exit(&s, "error: too many arguments\n", EXIT_MISSING);
	init_control_struct(&s, env);
	prompt_loop(&s);
	ft_exit(&s, "", 0);
	return (0);
}
