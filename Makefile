NAME			= minishell

SRCS	= 	main.c lexer.c utils.c linked.c print_stuffs.c ft_isalnum.c ft_strcmp.c \
			ft_isdigit.c ft_strjoin.c expand.c expand_2.c expand_3.c parser.c \
			ft_split.c  ft_split00.c lexer_utils.c lexer_utils_2.c main_utils.c parser_utils.c \
			parser_utils_2.c utils_2.c \
			built_ins/builts_in.c built_ins/pwd.c built_ins/cd.c\
			built_ins/echo.c  built_ins/env.c  built_ins/export.c\
			built_ins/unset.c built_ins/exit.c\
			utils_execution.c exec.c exec2.c exec1.c exec3.c redirections.c\
			ft_itoa.c ft_strlcat.c ft_strlcpy.c ft_atoi.c \
			ft_putendl_fd.c herdoc.c ft_calloc.c garbage_collector.c 
CC				= cc
RM				= rm -f
# CFLAGS			= -Wall -Wextra -Werror  -g3 -fsanitize=address 
CFLAGS			= -Wall -Wextra -Werror -g3 
# CFLAGS			= -Wall -Wextra -Werror 
OBJS			= $(SRCS:.c=.o)

all: $(NAME)
	@echo "done"


$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lreadline 


clean:
	$(RM) $(OBJS) 

fclean:clean 
	$(RM) $(NAME) 

re: fclean all
