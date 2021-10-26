/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mberne <mberne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 13:30:45 by pthomas           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2021/10/26 13:01:51 by pthomas          ###   ########lyon.fr   */
=======
/*   Updated: 2021/10/25 10:09:11 by mberne           ###   ########lyon.fr   */
>>>>>>> 41535f1d660d24a54f6608df6c35e6189c8fbcc0
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
# include <termios.h>
# include <errno.h>
# include "../libft/includes/libft.h"

/*** ~~ MACROS ~~ ***/

# define EXIT_MISSING 2
# define PROMPT "potatoshell >$ "
# define HEREDOC_PROMPT "> "

/*** ~~ STRUCTURES ~~ ***/

typedef struct s_cmd
{
	int			fd_in;
	char		**cmd;
	char		*path;
	int			fd_out;
}				t_cmd;

typedef struct s_env
{
	char		*name;
	char		*value;
	size_t		index;
	void		*next;
}				t_env;

typedef struct s_signals
{
	bool		sig_int;
	bool		sig_quit;
	pid_t		*pid;
}				t_signals;

typedef struct s_term
{
	struct termios	basic;
	struct termios	new;
}				t_term;

typedef struct s_structs
{
	t_cmd		*cmds;
	size_t		cmds_size;
	t_env		**env;
	size_t		env_size;
	t_term		term;
}				t_structs;

/*** ~~ PROTOTYPES ~~ ***/

// ~~ exit.c
void			free_cmds_struct(t_structs *s);
void			ft_exit(t_structs *s, char *errormsg, int status);
// ~~ env_list.c
int				env_new(t_structs *s, char *var);
void			env_del(t_structs *s, t_env *elem);
void			env_clear(t_structs *s);
char			**list_to_char(t_structs *s);
// ~~ signal.c
void			sig_int(int sig);
void			sig_quit(int sig);
// ~~ parsing.c
void			parsing(t_structs *s, char *line);
// ~~ syntax_checker.c
int				check_syntax_errors(char *line, char *charset);
// ~~ env_var_handler.c
char			*replace_env_variables(t_structs *s, char *line);
char			check_quotes(char c, char quote);
// ~~ parsing_utils.c
void			remove_quotes(char **cmd);
void			skip_spaces(char **line);
char			*get_args(char *line, bool is_file);
// ~~ file_handler.c
int				get_infile(t_structs *s, char **line, int i);
int				get_outfile(t_structs *s, char **line, int i);
// ~~ split_cmd.c
char			**split_cmd(char **cmd);
// ~~ exec.c
void			pipex(t_structs *s);
void			launch_command(t_structs *s, int in, int out, t_cmd *current);
void			wait_child_process(t_structs *s);
int				is_builtin(t_cmd current);
void			builtins(t_structs *s, t_cmd current);
// ~~ paths.c
int				get_path(t_structs *s, t_cmd *current);
char			**get_env_paths(t_structs *s);
char			**add_backslash(char **paths);
int				find_path(t_structs *s, char **paths, t_cmd *current);
int				find_path_in_sys(t_cmd *current, char **paths);
int				find_exe_path(t_structs *s, t_cmd *current);
char			*replace_by_home_path(t_structs *s, char *cmd);
int				path_error_check(t_cmd *current);
// ~~ builtins.c
int				ft_echo(t_structs *s, t_cmd current);
int				ft_pwd(t_structs *s, t_cmd current);
int				ft_env(t_structs *s, t_cmd current);
int				ft_unset(t_structs *s, t_cmd current);
// ~~ builtin_cd.c
int				ft_cd(t_structs *s, t_cmd current);
int				set_oldpwd(t_structs *s);
int				set_pwd(t_structs *s);
// ~~ builtin_export.c
int				ft_export(t_structs *s, t_cmd current);
int				create_env_variable(t_structs *s, t_cmd current);
void			create_variable(t_structs *s, char *cmd, char *tmp);
void			print_export(t_structs *s, t_cmd current);
void			index_list(t_structs *s);

#endif
