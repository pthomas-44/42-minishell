/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 01:50:39 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/11 14:14:42 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//~~ Built-in pwd

void	bi_pwd(int fd)
{
	char	cwd[MAXPATHLEN];

	g_numberr = EXIT_SUCCESS;
	if (!getcwd(cwd, MAXPATHLEN))
	{
		print_error("getcwd: ", NULL, NULL, errno);
		g_numberr = EXIT_FAILURE;
		return ;
	}
	ft_putstr_fd(cwd, fd);
	ft_putchar_fd('\n', fd);
}
