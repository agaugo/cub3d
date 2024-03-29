/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cub3d.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 11:52:59 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/15 10:49:57 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <fcntl.h>

# include "../lib/libft/libft.h"
# include "../lib/get_next_line/get_next_line.h"
# include "../lib/MLX42/include/MLX42/MLX42.h"

# define SCREEN_WIDTH 1300
# define SCREEN_HEIGHT 700

# define FOV 60
# define ROTATION_SPEED 0.025
# define PLAYER_SPEED 4

typedef struct s_player
{
	int		pixel_x;
	int		pixel_y;
	double	angle;
	char	direction;
}	t_player;

typedef struct s_key
{
	int		key_w_s;
	int		key_a_d;
	int		key_l_r;
}	t_key;

typedef struct s_map
{
	char	**field;
	int		player_start_x;
	int		player_start_y;
	int		width;
	int		height;
	int		tile_size;
	char	*id_no;
	char	*id_so;
	char	*id_we;
	char	*id_ea;
	char	*id_f;
	char	*id_c;
	int		color_floor;
	int		color_ceiling;
}	t_map;

typedef struct s_check_map
{
	char	**field;
	int		width;
	int		height;
}	t_check_map;

typedef struct s_mlx
{
	mlx_image_t	*img;
	mlx_t		*mlx_p;
	t_map		*map;
	t_check_map	*check_map;
	t_player	*player;
	t_key		*key;
}	t_mlx;

// Map:
int		cub_validate_map(t_map *map, char *file);
void	cub_check_map_characters(t_map *map);
int		cub_setup_map_checks(t_map *map, t_check_map *check_map);
void	cub_is_border_valid(t_check_map *check_map);
t_map	*cub_load_map_values(char *file);
void	cub_check_and_set_player(t_mlx *mlx);
void	cub_set_player_start_position(t_mlx *mlx);

// Movement:
void	cub_check_key(mlx_key_data_t keydata, void *parsed_mlx);
void	cub_player_update_frame(t_mlx *mlx, double move_x, double move_y);

// Render:
void	cub_cast_rays(t_mlx *mlx);

// Utils:
void	*allocate_memory(size_t buffer_size);
void	*memory_realloc(void *ptr, size_t new_size);
void	free_memory(void *buffer);
void	cub_handle_error(int exit_code, char *message);
void	cub_exit(t_mlx *mlx);

#endif
