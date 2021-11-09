/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 01:50:39 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/09 16:14:26 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//~~ Built-in pwd

void	bi_pwd(int fd)
{
	char	cwd[MAXPATHLEN];

	g_numberr = EXIT_SUCCESS;
	getcwd(cwd, MAXPATHLEN);
	ft_putstr_fd(cwd, fd);
	ft_putchar_fd('\n', fd);
}
