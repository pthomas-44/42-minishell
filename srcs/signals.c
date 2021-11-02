/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 19:42:48 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/29 21:02:26 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//~~ La fonction qui s'occupe de gérer les 'CTRL'+'C'

void	sig_int(int sig)
{
	(void)sig;
	char	*saved_line;

	saved_line = ft_strdup(rl_line_buffer);
	saved_line = ft_strjoin_f2(PROMPT, saved_line);
	rl_replace_line(saved_line, 1);
	rl_redisplay();
	rl_replace_line("", 1);
	rl_redisplay();
	rl_replace_line(saved_line, 1);
	rl_redisplay();
	free(saved_line);
	rl_replace_line("", 1);
	rl_on_new_line();
	write(1, "\n", 1);
	rl_redisplay();
}

//~~ La fonction qui s'occupe de gérer les 'CTRL'+'\'

void	sig_quit(int sig)
{
	(void)sig;
	char	*saved_line;

	saved_line = ft_strdup(rl_line_buffer);
	saved_line = ft_strjoin_f2(PROMPT, saved_line);
	rl_replace_line(saved_line, 1);
	rl_redisplay();
	rl_replace_line("", 1);
	rl_redisplay();
	rl_replace_line(saved_line, 1);
	rl_redisplay();
	free(saved_line);
	rl_replace_line("", 1);
	rl_on_new_line();
	write(1, "\n", 1);
	rl_redisplay();
}

//~~ La fonction qui s'occupe de gérer les 'CTRL'+'C' dans le processe enfant

void	child_sig_int(int sig)
{
	kill(0, sig);
}

//~~ La fonction qui s'occupe de gérer les 'CTRL'+'\' dans le processe enfant

void	child_sig_quit(int sig)
{
	kill(0, sig);
}
