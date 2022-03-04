NAME = pipex
CFLAGS = -Wall -Wextra -Werror

SRCS = pipex.c\
	   pipex_utiles.c\
	   ft_split.c\
	   ft_strjoin.c\

OBJS = $(SRCS:.c=.o)
RM = rm -rf
CC = cc

all : $(NAME)
$(NAME) : $(OBJS)
			$(CC) $(OBJS) -o $(NAME)
clean : 
		$(RM) $(OBJS)
fclean : clean
		$(RM) $(NAME)
re : fclean all