/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 19:42:48 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/05 19:31:20 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//~~ La fonction qui s'occupe de gérer les 'CTRL'+'C'

void	sig_int(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	ft_putchar_fd('\n', STDERR_FILENO);
	rl_on_new_line();
	rl_redisplay();
	g_numberr = 1;
}

//~~ La fonction qui s'occupe de gérer les 'CTRL'+'C' dans le heredoc

void	heredoc_sig_int(int sig)
{
	(void)sig;
	rl_replace_line("oskour\n", 0);
	g_numberr = 1;
	exit(g_numberr);
}

//~~ La fonction qui s'occupe de gérer les 'CTRL'+'C' dans le processe enfant

void	child_sig_int(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', STDERR_FILENO);
	if (g_numberr)
		g_numberr = 130;
}

//~~ La fonction qui s'occupe de gérer les 'CTRL'+'\' dans le processe enfant

void	child_sig_quit(int sig)
{
	(void)sig;
	ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
	if (g_numberr)
		g_numberr = 131;
}