/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 13:30:45 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/06 20:00:33 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*** ~~ LIBS ~~ ***/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <locale.h>
# include <limits.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/select.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <term.h>
# include "../libft/includes/libft.h"

/*** ~~ MACROS ~~ ***/

# define EXIT_MISSING 2
# define PROMPT "minishell $> "

/*** ~~ STRUCTURES ~~ ***/

typedef struct s_cmd
{
	char		*in;
	char		*cmd;
	char		*out;
}				t_cmd;

typedef struct s_structs
{
	t_cmd		*cmds;
}				t_structs;

/*** ~~ PROTOTYPES ~~ ***/

// ~~ main.c
void			init_struct(t_structs *s);
void			prompt_loop(t_structs *s);
void			ft_exit(t_structs *s, char *errormsg, int status);
// ~~ signal.c
void			sig_int(int sig);
void			sig_quit(int sig);
// ~~ parsing.c
void			parsing(t_structs *s, char *line);
int				is_wrong_char(char *line);
void			fill_cmd_struct(t_structs *s, t_cmd *cmd, char *line);
void			replace_env_variables(t_structs *s);

#endif
