/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 15:26:00 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/05 18:20:39 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*heredoc_loop(char *stop)
{
	char	*line;
	char	*content;

	line = NULL;
	content = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, stop))
			break ;
		if (content)
			content = ft_strjoin_f1(content, "\n");
		content = ft_strjoin_f3(content, line);
		if (!content)
		{
			print_error("malloc: ", NULL, NULL, ENOMEM);
			break ;
		}
	}
	return (content);
}

static int	heredoc_handler2(t_structs *s, char *stop, int i, int pipe_fd[2])
{
	char	*content;

	content = heredoc_loop(stop);
	if (!content)
		return (-1);
	ft_putstr_fd(content, pipe_fd[STDOUT_FILENO]);
	free(content);
	if (close(pipe_fd[STDOUT_FILENO]) == -1)
	{
		print_error("close: ", NULL, NULL, errno);
		return (-1);
	}
	s->cmds[i].fd_in = pipe_fd[STDIN_FILENO];
	return (0);
}

//~~ La gestion du heredoc

static int	heredoc_handler(t_structs *s, char *stop, int i)
{
	int		pipe_fd[2];
	int		status;
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
	{
		print_error("pipe: ", NULL, NULL, errno);
		return (-1);
	}
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		print_error("fork: ", NULL, NULL, errno);
		return (-1);
	}
	else if (pid == 0)
	{
		signal(SIGINT, &heredoc_sig_int);
		heredoc_handler2(s, stop, i, pipe_fd);
		free_all(s, 1);
		exit(g_numberr);
	}
	else if (waitpid(-1, &status, WUNTRACED) == -1)
	{
		print_error("waitpid: ", NULL, NULL, errno);
		return (-1);
	}
	if (WIFEXITED(status))
		g_numberr = WEXITSTATUS(status);
	if (close(pipe_fd[STDOUT_FILENO]) == -1)
	{
		print_error("close: ", NULL, NULL, errno);
		return (-1);
	}
	signal(SIGINT, &sig_int);
	return (0);
}

//~~ La boucle de lecture du heredoc 

// static char	*heredoc_loop(char *stop)
// {
// 	char	*line;
// 	char	*content;

// 	line = NULL;
// 	content = NULL;
// 	signal(SIGINT, &heredoc_sig_int);
// 	while (1)
// 	{
// 		printf("test1\n");
// 		line = readline("> ");
// 		printf("test2\n");
// 		if (!line || !ft_strcmp(line, stop))
// 			break ;
// 		if (content)
// 			content = ft_strjoin_f1(content, "\n");
// 		content = ft_strjoin_f3(content, line);
// 		if (!content)
// 		{
// 			print_error("malloc: ", NULL, NULL, ENOMEM);
// 			break ;
// 		}
// 	}
// 	signal(SIGINT, &sig_int);
// 	return (content);
// }

// //~~ La gestion du heredoc

// static int	heredoc_handler(t_structs *s, char *stop, int i)
// {
// 	int		pipe_fd[2];
// 	char	*content;

// 	content = heredoc_loop(stop);
// 	if (!content)
// 		return (-1);
// 	if (pipe(pipe_fd) == -1)
// 	{
// 		print_error("pipe: ", NULL, NULL, errno);
// 		free(content);
// 		return (-1);
// 	}
// 	ft_putstr_fd(content, pipe_fd[STDOUT_FILENO]);
// 	free(content);
// 	if (close(pipe_fd[STDOUT_FILENO]) == -1)
// 	{
// 		print_error("close: ", NULL, NULL, errno);
// 		return (-1);
// 	}
// 	s->cmds[i].fd_in = pipe_fd[STDIN_FILENO];
// 	return (0);
// }

//~~ Recuparation du fichier de redirection de la sortie

int	get_outfile(t_structs *s, char **line, int i)
{
	char	*tmp[2];

	if (s->cmds[i].fd_out > 1 && close(s->cmds[i].fd_out) == -1)
	{
		print_error("close: ", NULL, NULL, errno);
		return (-1);
	}
	tmp[0] = *line;
	while (*(*line) == '>')
		(*line)++;
	skip_spaces(&(*line));
	tmp[1] = get_args(*line, 1);
	if (*(tmp[0] + 1) == '>')
		s->cmds[i].fd_out = open(tmp[1], O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		s->cmds[i].fd_out = open(tmp[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (s->cmds[i].fd_out == -1)
	{
		print_error("open: ", tmp[1], NULL, errno);
		free(tmp[1]);
		return (-1);
	}
	(*line) += ft_strlen(tmp[1]);
	free(tmp[1]);
	return (0);
}

//~~ Suite de la fonction get_infile() (norme)

static int	get_infile_sequel(t_structs *s, int i, char **tmp)
{
	s->cmds[i].fd_in = open(tmp[1], O_RDONLY);
	if (s->cmds[i].fd_in == -1)
	{
		print_error("open: ", tmp[1], NULL, errno);
		free(tmp[1]);
		return (-1);
	}
	return (0);
}

//~~ Recuparation du fichier de redirection de l'entree

int	get_infile(t_structs *s, char **line, int i)
{
	char	*tmp[2];

	if (s->cmds[i].fd_in > 0 && close(s->cmds[i].fd_in) == -1)
	{
		print_error("close: ", NULL, NULL, errno);
		return (-1);
	}
	tmp[0] = *line;
	while (*(*line) == '<')
		(*line)++;
	skip_spaces(line);
	tmp[1] = get_args(*line, 1);
	if (*(tmp[0] + 1) == '<')
	{
		if (heredoc_handler(s, tmp[1], i) == -1)
		{
			free(tmp[1]);
			return (-1);
		}
	}
	else if (get_infile_sequel(s, i, tmp) == -1)
		return (-1);
	(*line) += ft_strlen(tmp[1]);
	free(tmp[1]);
	return (0);
}
