NAME	:= cub3D
CFLAGS	:= #-Wextra -Wall -Werror
LIBMLX	:= ./lib/MLX42
LIBFT	:= ./lib/libft/libft.a
GNL		:= ./lib/get_next_line/get_next_line.a

HEADERS	:= -I ./include -I $(LIBMLX)/include
LIBS	:= $(LIBMLX)/build/libmlx42.a -ldl `pkg-config --libs glfw3` -pthread -lm
SRCS	:= $(shell find src -iname "*.c")
OBJS	:= $(SRCS:%.c=objs/%.o)
OBJS_DIR = objs/

RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
RESET = \033[0m

all: libmlx $(NAME)

libmlx:
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

$(NAME): $(OBJS) $(LIBFT) $(GNL)
	@echo "$(YELLOW)Compiling $(NAME)...$(RESET)"
	@$(CC) $(OBJS) $(LIBFT) $(GNL) $(LIBS) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled successfully!$(RESET)"

$(OBJS_DIR)%.o: %.c
	@echo "$(BLUE)Creating $(NAME) object files...$(RESET)"
	mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

$(LIBFT):
	$(MAKE) all -C ./lib/libft

$(GNL):
	$(MAKE) all -C ./lib/get_next_line

clean:
	$(MAKE) clean -C ./lib/libft
	$(MAKE) clean -C ./lib/get_next_line
	@rm -rf objs
	@rm -rf $(LIBMLX)/build

fclean: clean
	$(MAKE) fclean -C ./lib/libft
	$(MAKE) fclean -C ./lib/get_next_line
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
