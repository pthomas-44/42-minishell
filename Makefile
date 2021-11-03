# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/04 14:35:00 by pthomas           #+#    #+#              #
#    Updated: 2021/11/03 01:21:22 by pthomas          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

#========================================#
#=============== VARIABLES ==============#
#========================================#

#~~~~ Output ~~~~#

NAME		=	minishell

#~~~~ Paths ~~~~#

VPATH		=	src:src/util:src/signal:src/parsing:src/exec:src/builtin:include
PATH_OBJS	=	obj
PATH_LIBFT	=	libft

#~~~~ Sources ~~~~#
 
SRCS		=	main.c				\
				exit.c				\
				env_list.c			\
				signals.c			\
				parsing.c			\
				file_handler.c		\
				syntax_checker.c	\
				env_var_handler.c	\
				parsing_utils.c		\
				split_cmd.c			\
				exec.c				\
				pipex.c				\
				paths_utils.c		\
				builtins.c			\
				builtin_cd.c		\
				builtin_export.c	\
				builtins_utils.c

#~~~~ Objects ~~~~#

OBJS		=	$(addprefix $(PATH_OBJS)/, $(SRCS:.c=.o))

#~~~~ Includes ~~~~#

INCS		 =	minishell.h

#~~~~ Macros ~~~~#

CC			=	gcc
CFLAGS		=	-Wall -Wextra -Werror -fsanitize=address
LIBS		=	$(PATH_LIBFT)/libft.a
RM			=	rm -rf

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

run :			re
				./$(NAME)

#~~~~ Compilation Rules ~~~~#

$(PATH_OBJS)/%.o :	%.c $(INCS) $(LIBS)
					@mkdir -p $(dir $@);
					$(CC) $(CFLAGS) -I $(INCS) -c $< -o $@

#~~~~ Cleaning Rules ~~~~#

clean :
				$(RM) $(PATH_OBJS)
				$(MAKE) clean -C $(PATH_LIBFT)

fclean :		
				$(MAKE) fclean -C $(PATH_LIBFT)
				$(RM) $(PATH_OBJS) $(NAME)

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
