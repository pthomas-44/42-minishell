# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dev <dev@student.42lyon.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/04 14:35:00 by pthomas           #+#    #+#              #
#    Updated: 2021/11/15 22:15:26 by dev              ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

#========================================#
#=============== VARIABLES ==============#
#========================================#

#~~~~ Output ~~~~#

NAME		=	minishell

#~~~~ Paths ~~~~#

VPATH		=	src/:src/util/:src/signal/:src/parsing/:src/exec/:src/builtin/
PATH_OBJ	=	obj/
PATH_LIBFT	=	libft/
PATH_INC	=	include/

#~~~~ Files ~~~~#
 
SRCS		=	main.c				\
				exit.c				\
				init.c				\
				env_list.c			\
				signals.c			\
				parsing.c			\
				file_handler.c		\
				heredoc.c			\
				syntax_checker.c	\
				env_var_handler.c	\
				parsing_utils.c		\
				split_cmd.c			\
				exec.c				\
				pipex.c				\
				get_path.c			\
				bi_echo.c			\
				bi_cd.c				\
				bi_pwd.c			\
				bi_export.c			\
				bi_unset.c			\
				bi_env.c			\
				bi_exit.c

OBJS		=	$(addprefix $(PATH_OBJ), $(SRCS:.c=.o))

INCS		=	$(addprefix $(PATH_INC), minishell.h)

#~~~~ Macros ~~~~#

CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror
LIBS		=	$(PATH_LIBFT)libft.a
RM			=	rm -rf

#========================================#
#=============== TARGETS ================#
#========================================#

#~~~~ Main Rules ~~~~#

all :			libs $(NAME)

$(NAME) :		$(OBJS) $(LIBS)
				$(CC) $(CFLAGS) $(OBJS) -lreadline $(LIBS) -o $(NAME) -I $(PATH_INC)

libs :		
				$(MAKE) -C $(PATH_LIBFT)

re :			fclean all

#~~~~ Compilation Rules ~~~~#

$(PATH_OBJ)%.o :	%.c $(INCS) $(LIBS)
					@mkdir -p $(dir $@);
					$(CC) $(CFLAGS) -c $< -o $@ -I $(PATH_INC)

#~~~~ Cleaning Rules ~~~~#

clean :
				$(RM) $(PATH_OBJ)
				$(MAKE) clean -C $(PATH_LIBFT)

fclean :		
				$(MAKE) fclean -C $(PATH_LIBFT)
				$(RM) $(PATH_OBJ) $(NAME)

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

.PHONY:		all libs clean fclean re eugene
