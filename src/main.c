/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 17:23:47 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/05 16:09:27 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//~~ La loop qui affiche le prompt et envoie l'entree standard dans le parsing

static void	prompt_loop(t_structs *s)
{

	while (1)
	{
		s->parse_line[0] = readline(PROMPT);
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
	ft_putstr_fd("exit\n", STDERR_FILENO);
	free_all(s, 0);
	exit(g_numberr);
}

int	main(int ac, char **av, char **env)
{
	t_structs	s;

	(void)av;
	if (ac != 1)
	{
		print_error(NULL, NULL, NULL, E2BIG);
		exit(g_numberr);
	}
	init_control_struct(&s, env);
	prompt_loop(&s);
	return (0);
}
