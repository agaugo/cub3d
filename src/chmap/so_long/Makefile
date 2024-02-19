NAME = so_long
MLX = mlx/libmlx.a
LIBFT = libft/libft.a
FT_PRINTF = ft_printf/ft_printf.a
GET_NEXT_LINE = get_next_line/get_next_line.a

CC = gcc
CC_FLAGS = -Wall -Wextra -Werror
MLX_FLAGS = -Lmlx -lmlx -framework OpenGL -framework AppKit

SRCS = \
		ft_so_long.c \
		ft_load_map.c \
		ft_check_if_map_valid.c \
		ft_put_images.c \
		ft_player_move.c \
		ft_has_a_valid_path.c \
		ft_2d_map.c \
		ft_has_closed_walls.c

OBJS = \
		ft_so_long.o \
		ft_load_map.o \
		ft_check_if_map_valid.o \
		ft_put_images.o \
		ft_player_move.o \
		ft_has_a_valid_path.o \
		ft_2d_map.o \
		ft_has_closed_walls.o

all: $(NAME)

$(NAME):
	$(MAKE) all -C mlx
	$(MAKE) all -C libft
	$(MAKE) all -C ft_printf
	$(MAKE) all -C get_next_line
	$(CC) $(CC_FLAGS) $(MLX_FLAGS) $(SRCS) $(MLX) $(LIBFT) $(FT_PRINTF) $(GET_NEXT_LINE) -o $(NAME)

clean:
	rm -rf $(OBJS)
	$(MAKE) clean -C mlx
	$(MAKE) clean -C libft
	$(MAKE) clean -C ft_printf
	$(MAKE) clean -C get_next_line

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C libft
	$(MAKE) fclean -C ft_printf
	$(MAKE) fclean -C get_next_line

re: fclean all
