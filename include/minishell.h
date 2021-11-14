/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 13:30:45 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/14 11:00:10 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*** ~~ LIBS ~~ ***/

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <stdbool.h>
# include <fcntl.h>
# include <sys/param.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <termios.h>
# include <errno.h>
# include "../libft/include/libft.h"

/*** ~~ MACROS ~~ ***/

# define PROMPT "\033[0;33mpotatoshell \033[0;32m>$ \033[0m"
# define ERR_PROMPT "\033[0;33mpotatoshell \033[1;31m>$ \033[0m"

/*** ~~ GLOBALS ~~ ***/

// ~~ Error var

int	g_error_number;

/*** ~~ STRUCTURES ~~ ***/

// ~~ Command line struct

typedef struct s_cmd
{
	int			fd_in;
	int			fd_out;
	int			pipefd[2];
	char		**cmd;
	char		*path;
}				t_cmd;

// ~~ Env variables struct

typedef struct s_env
{
	char		*name;
	char		*value;
	size_t		index;
	void		*next;
}				t_env;

// ~~ Control struct

typedef struct s_structs
{
	char				*prompt;
	char				*parse_line[2];
	t_cmd				*cmds;
	size_t				cmds_size;
	t_env				**env;
	size_t				env_size;
	struct termios		old_term;
	struct termios		new_term;
}						t_structs;

/*** ~~ PROTOTYPES ~~ ***/

// ~~ exit.c
void			free_all(t_structs *s, bool is_child);
void			free_cmds_struct(t_structs *s);
void			print_error(char *cmd, char *value, char *error, int status);
// ~~ util
int				env_new(t_structs *s, char *var);
void			env_del(t_structs *s, t_env *elem);
void			env_clear(t_structs *s);
t_env			*find_env_var(t_structs *s, char *name);
char			**list_to_char(t_structs *s);
void			init_control_struct(t_structs *s, char **env);
// ~~ signal
void			sig_int(int sig);
void			heredoc_sig_int(int sig);
// ~~ parsing
void			parsing(t_structs *s, char *line);
int				check_syntax_errors(char *line, char *charset);
char			*replace_env_variables(t_structs *s, char *line);
char			*handle_operands(char *line, char *charset);
char			check_quotes(char c, char quote);
void			remove_quotes_and_backslash(char **cmd);
void			skip_spaces(char **line);
char			*get_args(char *line, char *charset);
char			*remove_char(char *str, size_t i);
int				get_infile(t_structs *s, char **line, int i);
int				heredoc_handler(t_structs *s, t_cmd *current, char *stop);
int				get_outfile(t_structs *s, char **line, int i);
char			**split_cmd(char **cmd);
// ~~ exec
void			exec(t_structs *s);
int				is_builtin(t_cmd current);
void			builtins(t_structs *s, t_cmd *current, int fd);
void			pipex(t_structs *s);
void			get_path(t_structs *s, t_cmd *current);
int				path_error_check(t_cmd *current);
// ~~ builtins
void			bi_echo(t_cmd *current, int fd);
void			bi_pwd(int fd);
void			bi_env(t_structs *s, int fd);
void			bi_unset(t_structs *s, t_cmd *current);
void			bi_exit(t_structs *s, t_cmd *current);
void			bi_cd(t_structs *s, t_cmd *current);
void			bi_export(t_structs *s, t_cmd *current, int fd);
int				is_word(char *str);
int				replace_by_home_path(t_structs *s, char *path, char **new);


#endif
