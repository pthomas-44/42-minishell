/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 19:42:48 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/14 15:24:52 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//~~ La fonction qui s'occupe de gérer les 'CTRL'+'C'

void	sig_int(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	ft_putchar_fd('\n', STDERR_FILENO);
	rl_on_new_line();
	rl_redisplay();
	g_error_number = 1;
}

//~~ La fonction qui s'occupe de gérer les 'CTRL'+'C' dans le heredoc

void	heredoc_sig_int(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', STDERR_FILENO);
	g_error_number = 1;
	exit(g_error_number);
}

//~~ Gere les signaux des process enfants

void	sig_child(int sig)
{
	if (sig == SIGINT)
	{
		ft_putchar_fd('\n', STDERR_FILENO);
		g_error_number = 130;
	}
	else if (sig == SIGQUIT)
	{
		ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
		g_error_number = 131;
	}
}