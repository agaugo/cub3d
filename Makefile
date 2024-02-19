NAME	= cub3d

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -Iinc/

LIBFT = libft/libft.a
GET_NEXT_LINE = get_next_line/get_next_line.a

OBJS_DIR	= obj/

RED		= \033[1;31m
GREEN	= \033[1;32m
YELLOW	= \033[1;33m
BLUE	= \033[1;34m
RESET	= \033[0m

SRC =	src/init/main.c \
		src/init/check.c \

OBJS = $(addprefix $(OBJS_DIR), $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(GET_NEXT_LINE)
	@echo "$(YELLOW)Compiling $(NAME)...$(RESET)"
	$(CC) $(OBJS) $(LIBFT) $(GET_NEXT_LINE) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled successfully!$(RESET)"

$(LIBFT):
	@echo "$(YELLOW)Compiling libft...$(RESET)"
	@$(MAKE) -C libft

$(GET_NEXT_LINE):
	@echo "$(YELLOW)Compiling get_next_line...$(RESET)"
	@$(MAKE) -C get_next_line

$(OBJS_DIR)%.o: %.c
	@echo "$(BLUE)Creating $(NAME) object files...$(RESET)"
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean: clean_libft clean_get_next_line
	@echo "$(RED)Cleaning $(NAME) object files and directory...$(RESET)"
	rm -rf $(OBJS_DIR)

clean_libft:
	@echo "$(RED)Cleaning libft object files...$(RESET)"
	@$(MAKE) -C libft clean

clean_get_next_line:
	@echo "$(RED)Cleaning get_next_line object files...$(RESET)"
	@$(MAKE) -C get_next_line clean

fclean: fclean_libft fclean_get_next_line
	@echo "$(RED)Removing $(NAME) executable and $(OBJS_DIR) directory...$(RESET)"
	rm -f $(NAME)
	rm -rf $(OBJS_DIR)

fclean_libft:
	@echo "$(RED)Removing libft archive...$(RESET)"
	@$(MAKE) -C libft fclean

fclean_get_next_line:
	@echo "$(RED)Removing get_next_line archive...$(RESET)"
	@$(MAKE) -C get_next_line fclean

re: fclean all

.PHONY: all clean fclean re clean_libft clean_get_next_line fclean_libft fclean_get_next_line
