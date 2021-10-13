/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 13:30:45 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/13 10:33:35 by mberne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*** ~~ LIBS ~~ ***/

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <fcntl.h>
# include <locale.h>
# include <limits.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/select.h>
# include <sys/param.h>
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
	int			fd_in;
	char		**cmd;
	char		*path;
	int			fd_out;
	pid_t		pid;
}				t_cmd;

typedef struct s_env
{
	char		*name;
	char		*value;
	size_t		index;
	void		*next;
}				t_env;

typedef struct s_structs
{
	t_cmd		*cmds;
	size_t		cmds_size;
	t_env		**env;
	size_t		env_size;
}				t_structs;

/*** ~~ PROTOTYPES ~~ ***/

// ~~ main.c
void			init_control_struct(t_structs *s, char **env);
void			prompt_loop(t_structs *s);
void			ft_exit(t_structs *s, char *errormsg, int status);
// ~~ exit.c
void			free_cmds_struct(t_structs *s);
void			ft_exit(t_structs *s, char *errormsg, int status);

// ~~ signal.c
void			sig_int(int sig);
void			sig_quit(int sig);
// ~~ parsing.c
void			parsing(t_structs *s, char *line);
int				check_unclosed_quotes(char *line);
void			replace_env_variables(t_structs *s);
// ~~ paths.c
void			find_cmd_paths(t_structs *s);
void			set_paths(t_structs *s, char **paths);
void			find_good_path(t_structs *s, char **paths);
// ~~ pipex.c
void			pipex(t_structs *s);
void			launch_command(t_structs *s, int in, int out, int i);
// ~~ builtins.c
void			ft_echo(t_cmd current);
void			ft_pwd(t_cmd current);
void			ft_env(t_structs *s, t_cmd current);
// ~~ builtin_cd.c
void			ft_cd(t_structs *s, t_cmd current);
t_env			*set_oldpwd(t_structs *s);
void			set_pwd(t_structs *s, t_env *pwd);
// ~~ builtin_export.c
void			ft_export(t_structs *s, t_cmd current);
// ~~ builtin_unset.c
void			ft_unset(t_structs *s, t_cmd current);

#endif
