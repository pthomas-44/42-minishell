/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 01:50:39 by mberne            #+#    #+#             */
/*   Updated: 2021/11/16 15:08:34 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//~~ Built-in pwd

void	bi_pwd(int fd)
{
	char	cwd[MAXPATHLEN];

	g_error_number = EXIT_SUCCESS;
	if (!getcwd(cwd, MAXPATHLEN))
	{
		print_error("getcwd: ", NULL, NULL, errno);
		g_error_number = EXIT_FAILURE;
		return ;
	}
	ft_putstr_fd(cwd, fd);
	ft_putchar_fd('\n', fd);
}
