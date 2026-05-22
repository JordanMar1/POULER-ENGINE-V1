##
## EPITECH PROJECT, 2026
## MY_GZDOOM
## File description:
## Makefile
##

SRCS = array_lib.c
OBJS = $(SRCS:.c=.o)
NAME = libarray.a

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@for i in $(OBJS); do \
		echo $$i compiled into $(NAME); \
	done

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all