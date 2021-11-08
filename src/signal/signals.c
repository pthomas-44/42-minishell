/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 19:42:48 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/08 16:08:01 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//~~ La fonction qui s'occupe de gérer les 'CTRL'+'C'

void	sig_int(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	dprintf(2, "parent_sig_int\n");
	ft_putchar_fd('\n', STDERR_FILENO);
	rl_on_new_line();
	rl_redisplay();
	g_numberr = 1;
}

//~~ La fonction qui s'occupe de gérer les 'CTRL'+'C' dans le heredoc

void	heredoc_sig_int(int sig)
{
	(void)sig;
	dprintf(2, "heredoc_sig_int\n");
	ft_putchar_fd('\n', STDERR_FILENO);
	g_numberr = 1;
	exit(g_numberr);
}

//~~ La fonction qui s'occupe de gérer les 'CTRL'+'C' dans le process enfant

void	child_sig_int(int sig)
{
	(void)sig;
	dprintf(2, "child_sig_int\n");
	ft_putchar_fd('\n', STDERR_FILENO);
	g_numberr = 130;
	exit(g_numberr);
}

//~~ La fonction qui s'occupe de gérer les 'CTRL'+'\' dans le process enfant

void	child_sig_quit(int sig)
{
	(void)sig;
	dprintf(2, "child_sig_quit\n");
	ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
	g_numberr = 131;
	exit(g_numberr);
}
