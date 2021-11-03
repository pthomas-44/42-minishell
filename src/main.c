/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 17:23:47 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/03 11:04:17 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//~~ La loop qui affiche le prompt et envoie l'entree standard dans le parsing

static void	prompt_loop(t_structs *s)
{
	int	tmp_errno;

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
	ft_putstr_fd("exit\n", STDERR_FILENO);
	free_all(s, 0);
	exit(errno);
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
