# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/04 14:35:00 by pthomas           #+#    #+#              #
#    Updated: 2021/10/14 13:52:08 by pthomas          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

#========================================#
#=============== VARIABLES ==============#
#========================================#

#~~~~ Output ~~~~#

NAME		=	minishell

#~~~~ Paths ~~~~#

PATH_INCS	=	includes
PATH_SRCS	=	srcs
PATH_OBJS	=	objs
PATH_LIBFT	=	libft

#~~~~ Sources ~~~~#
 
SRCS		=	main.c				\
				exit.c				\
				env_list.c			\
				signals.c			\
				parsing.c			\
				parsing_utils1.c	\
				parsing_utils2.c	\
				parsing_utils3.c	\
				paths.c				\
				pipex.c				\
				builtins.c			\
				builtin_cd.c		\
				builtin_export.c	\
				builtin_unset.c		

#~~~~ Objects ~~~~#

OBJS		=	$(addprefix $(PATH_OBJS)/, $(SRCS:.c=.o))

#~~~~ Includes ~~~~#

INCS		=	$(addprefix $(PATH_INCS)/, minishell.h)

#~~~~ Macros ~~~~#

CC			=	gcc
CFLAGS		=	-Wall -Wextra -Werror
LIBS		=	$(PATH_LIBFT)/libft.a
RM			=	rm -f

#========================================#
#=============== TARGETS ================#
#========================================#

#~~~~ Main Rules ~~~~#

all :			libs $(NAME)

$(NAME) :	$(OBJS) $(LIBS)
				$(CC) $(CFLAGS) -lreadline -I $(INCS) $(OBJS) $(LIBS) -o $(NAME)

libs :		
				$(MAKE) -C $(PATH_LIBFT)

re :			fclean all

#~~~~ Compilation Rules ~~~~#

$(PATH_OBJS)/%.o :	$(PATH_SRCS)/%.c $(INCS) $(LIBS)
					@ mkdir -p $(PATH_OBJS)
					$(CC) $(CFLAGS) -O3 -I $(INCS) -c $< -o $@

#~~~~ Norminette ~~~~#

norminette :
				$(MAKE) norminette -C $(PATH_LIBFT)
				norminette $(PATH_SRCS) $(PATH_BSRCS) $(PATH_INCS)

#~~~~ Cleaning Rules ~~~~#

clean :
				$(MAKE) clean -C $(PATH_LIBFT)
				$(RM) -r $(PATH_OBJS)

fclean :		
				$(MAKE) fclean -C $(PATH_LIBFT)
				$(RM) -r $(PATH_OBJS) $(NAME) $(CHECKER)

#~~~~ Eugene ~~~~#

eugene :	
			@ echo "               _,........__"
			@ echo "            ,-'            \"\`-."
			@ echo "          ,'                   \`-."
			@ echo "        ,'                        \\"
			@ echo "      ,'                           ."
			@ echo "      .'\\               ,\"\".       \`"
			@ echo "     ._.'|             / |  \`       \\"
			@ echo "     |   |            \`-.'  ||       \`."
			@ echo "     |   |            '-._,'||       | \\"
			@ echo "     .\`.,'             \`..,'.'       , |\`-."
			@ echo "     l                       .'\`.  _/  |   \`."
			@ echo "     \`-.._'-   ,          _ _'   -\" \\  .     \`"
			@ echo "\`.\"\"\"\"\"'-.\`-...,---------','         \`. \`....__."
			@ echo ".'        \`\"-..___      __,'\\          \\  \\     \\"
			@ echo "\\_ .          |   \`\"\"\"\"'    \`.           . \\     \\"
			@ echo "  \`.          |              \`.          |  .     L"
			@ echo "    \`.        |\`--...________.'.        j   |     |"
			@ echo "      \`._    .'      |          \`.     .|   ,     |"
			@ echo "         \`--,\\       .            \`7\"\"' |  ,      |"
			@ echo "            \` \`      \`            /     |  |      |    _,-'\"\"\"\`-."
			@ echo "             \\ \`.     .          /      |  '      |  ,'          \`."
			@ echo "              \\  v.__  .        '       .   \\    /| /              \\"
			@ echo "               \\/    \`\"\"\\\"\"\"\"\"\"\"\`.       \\   \\  /.''                |"
			@ echo "                \`        .        \`._ ___,j.  \`/ .-       ,---.     |"
			@ echo "                ,\`-.      \\         .\"     \`.  |/        j     \`    |"
			@ echo "               /    \`.     \\       /         \\ /         |     /    j"
			@ echo "              |       \`-.   7-.._ .          |\"          '         /"
			@ echo "              |          \`./_    \`|          |            .     _,'"
			@ echo "              \`.           / \`----|          |-............\`---'"
			@ echo "                \\          \\      |          |"
			@ echo "               ,'           )     \`.         |"
			@ echo "                7____,,..--'      /          |"
			@ echo "                                  \`---.__,--.'"
								  
.PHONY:		all libs norminette clean fclean re eugene
