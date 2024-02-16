NAME = ft_printf.a

CC = gcc
CC_FLAGS = -Wall -Wextra -Werror

SRCS = \
		ft_printf.c \
		ft_pf_write.c \
		ft_pf_itop.c \
		ft_pf_itoa.c \
		ft_pf_itoh.c

OBJS = \
		ft_printf.o \
		ft_pf_write.o \
		ft_pf_itop.o \
		ft_pf_itoa.o \
		ft_pf_itoh.o

all: $(NAME)

$(NAME):
	$(CC) $(CC_FLAGS) -c $(SRCS)
	ar rc $(NAME) $(OBJS)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
