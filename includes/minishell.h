/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 13:30:45 by pthomas           #+#    #+#             */
/*   Updated: 2021/10/11 15:46:38 by pthomas          ###   ########lyon.fr   */
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
	void		*next;
}				t_env;

typedef struct s_structs
{
	t_cmd		*cmds;
	size_t		cmds_size;
	t_env		**env;
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
int				check_syntax_errors(char *line, char *charset);
void			replace_env_variables(t_structs *s);
// ~~ parsing_utils.c
void			init_cmds_struct(t_structs *s, char *line);
size_t			nb_of_pipes(char *line);
void			skip_spaces(char **line);
// ~~ parsing_utils2.c
int				get_command(t_structs *s, char **line, int i);
int				get_outfile(t_structs *s, char **line, int i);
int				get_infile(t_structs *s, char **line, int i);
char			*get_args(char *line);
char			*get_file(char *line);

#endif
