/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 16:00:37 by pthomas           #+#    #+#             */
/*   Updated: 2021/11/13 20:48:18 by pthomas          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

/*** ~~ LIBS ~~ ***/

# include <unistd.h>
# include <stdlib.h>

/*** ~~ MACRO ~~ ***/

# define BUFFER_SIZE 32

/*** ~~ STRUCTURES ~~ ***/

// ~~ BUFFER

typedef struct s_buffer
{
	char			buf[BUFFER_SIZE + 1];
	int				i;
	int				fd;
	struct s_buffer	*next;
}				t_buffer;

/*** ~~ PROTOTYPES ~~ ***/

// ~~ ft_mem1.c
void		*ft_memset(void *b, int c, size_t len);
void		ft_bzero(void *b, size_t n);
void		*ft_memcpy(void *dst, const void *src, size_t n);
void		*ft_memccpy(void *dst, const void *src, int c, size_t n);
// ~~ ft_mem2.c
void		*ft_memmove(void *dst, const void *src, size_t len);
void		*ft_memchr(const void *s, int c, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
void		free_tab(char ***split, size_t size);
// ~~ ft_strinfo1.c
size_t		ft_strlen(const char *s);
char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
// ~~ ft_strinfo2.c
char		*ft_strnstr(const char *haystack, const char *needle, size_t len);
char		*ft_strchrstr(const char *s1, const char *s2);
// ~~ ft_chrtype.c
int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isalnum(int c);
int			ft_isascii(int c);
int			ft_isprint(int c);
// ~~ ft_strtype.c
int			ft_str_isalpha(char *s);
int			ft_str_isdigit(char *s);
int			ft_str_isalnum(char *s);
int			ft_str_isascii(char *s);
int			ft_str_isprint(char *s);
// ~~ ft_num.c
void		ft_swap(int *a, int *b);
int			ft_atoi(const char *str);
long		ft_atol(const char *str);
char		*ft_nbtobase(long long nb, char *base);
// ~~ ft_convertbase.c
char		*ft_convert_base(char *nbr, char *base_from, char *base_to);
// ~~ ft_misc.c
int			ft_framing(int min, int nb, int max);
float		ft_framingf(float min, float nb, float max);
size_t		ft_count_occurences(char *str, char *charset);
// ~~ ft_strmodif.c
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t		ft_strlcat(char *dst, const char *src, size_t dstsize);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char		*ft_strrev(char *s);
// ~~ ft_stralloc.c
void		*ft_calloc(size_t count, size_t size);
char		*ft_strdup(const char *s1);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strtrim(char const *s1, char const *set);
// ~~ ft_strjoin.c
char		*ft_strjoin_f0(char const *s1, char const *s2);
char		*ft_strjoin_f1(const char *s1, const char *s2);
char		*ft_strjoin_f2(const char *s1, const char *s2);
char		*ft_strjoin_f3(const char *s1, const char *s2);
// ~~ ft_split.c
char		**ft_split(char const *s, char c);
// ~~ ft_strput.c
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);
// ~~ get_next_line.c
int			get_next_line(int fd, char **line);
char		**get_line(t_buffer *act, char **line, int i);
int			fill_buffer(t_buffer *act);
t_buffer	*get_actual(t_buffer **start, int fd);
// ~~ get_next_line_utils.c
size_t		linelen(t_buffer *act);
void		lstdelone(t_buffer **start, t_buffer *act);
t_buffer	*lstadd_front(t_buffer **start, int fd);

#endif
