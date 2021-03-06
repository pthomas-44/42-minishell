# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pthomas <pthomas@student.42lyon.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/04 14:35:00 by pthomas           #+#    #+#              #
#    Updated: 2021/11/16 15:02:27 by pthomas          ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

#========================================#
#=============== VARIABLES ==============#
#========================================#

#~~~~ Output ~~~~#

NAME		=	minishell

#~~~~ Paths ~~~~#

VPATH		=	src/:src/util/:src/signal/:src/parsing/:src/exec/:src/builtin/
PATH_INC	=	include/
PATH_LIBFT	=	libft/
PATH_OBJ	=	obj/

#~~~~ Files ~~~~#
 
SRC			=	main.c				\
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

OBJ			=	$(addprefix $(PATH_OBJ), $(SRC:.c=.o))

INC			=	$(addprefix $(PATH_INC), minishell.h)

LIB			=	$(PATH_LIBFT)libft.a

#~~~~ Macros ~~~~#

CC			=	cc
CFLAG		=	-Wall -Wextra -Werror
INCLUDES	=	-I $(PATH_INC) -I $(PATH_LIBFT)$(PATH_INC)
RM			=	rm -rf

#========================================#
#=============== TARGETS ================#
#========================================#

#~~~~ Main Rules ~~~~#

all :			lib $(NAME)

$(NAME) :		$(OBJ) $(LIB)
				$(CC) $(CFLAG) $(OBJ) -lreadline $(LIB) -o $(NAME) $(INCLUDES)

lib :		
				$(MAKE) -C $(PATH_LIBFT)

re :			fclean all

#~~~~ Compilation Rules ~~~~#

$(PATH_OBJ)%.o :	%.c $(INC) $(LIB)
					@mkdir -p $(dir $@);
					$(CC) $(CFLAG) -c $< -o $@ $(INCLUDES)

#~~~~ Cleaning Rules ~~~~#

clean :
				$(MAKE) clean -C $(PATH_LIBFT)
				$(RM) $(PATH_OBJ)

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

.PHONY:		all lib clean fclean re eugene
