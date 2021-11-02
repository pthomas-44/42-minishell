/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 19:42:48 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/02 19:33:54 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//~~ La fonction qui s'occupe de gérer les 'CTRL'+'C'

void	sig_int(int sig)
{
	(void)sig;
	rl_replace_line("", 1);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

//~~ La fonction qui s'occupe de gérer les 'CTRL'+'\'

void	sig_quit(int sig)
{
	(void)sig;
}

//~~ La fonction qui s'occupe de gérer les 'CTRL'+'C' dans le processe enfant

void	child_sig_int(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_redisplay();
}

//~~ La fonction qui s'occupe de gérer les 'CTRL'+'\' dans le processe enfant

void	child_sig_quit(int sig)
{
	(void)sig;
	write(2, "Quit: 3\n", 8);
	rl_redisplay();
}
