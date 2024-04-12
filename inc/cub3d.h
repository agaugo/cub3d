/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cub3d.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 11:52:59 by trstn4        #+#    #+#                 */
/*   Updated: 2024/04/12 13:42:46 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <fcntl.h>
# include <ctype.h>

# include "../lib/libft/libft.h"
# include "../lib/get_next_line/get_next_line.h"
# include "../lib/MLX42/include/MLX42/MLX42.h"

# define SCREEN_WIDTH 1900
# define SCREEN_HEIGHT 1000
# define TILE_SIZE 64

# define FOV 60
# define ROTATION_SPEED 0.025
# define PLAYER_SPEED 4

#define MAP_WIDTH 24  // based on the number of columns in your map
#define MAP_HEIGHT 6   // based on the number of rows in your map
#define pixel_width_per_square (SCREEN_WIDTH / MAP_WIDTH)
#define pixel_height_per_square (SCREEN_HEIGHT / MAP_HEIGHT)

typedef struct s_ray
{
	int		index;
	double	ray_ngl;
	double	horiz_x;
	double	horiz_y;
	double	vert_x;
	double	vert_y;
	double	distance;
	int		flag;
}	t_ray;

typedef struct s_player
{
	int		pixel_x;
	int		pixel_y;
	char	direction;
    double	angle;
	float	fov_rd;
}	t_player;

typedef struct s_key
{
	int		key_w_s;
	int		key_a_d;
	int		key_l_r;
}	t_key;

typedef struct s_map
{
	char			**field;
	int				player_start_x;
	int				player_start_y;
	int				width;
	int				height;
	int				tile_size;
	char			*id_no;
	char			*id_so;
	char			*id_ea;
	char			*id_we;
	char			*id_f;
	char			*id_c;
	mlx_texture_t	*north_texture;
	mlx_texture_t	*south_texture;
	mlx_texture_t	*east_texture;
	mlx_texture_t	*west_texture;
	int				color_floor;
	int				color_ceiling;
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
	t_ray		*ray;
}	t_mlx;

// Map:
void			cub_is_cub_extension(char *file);
int				cub_validate_map(t_map *map);
void			cub_check_map_characters(t_map *map);
int				cub_setup_map_checks(t_map *map, t_check_map *check_map);
void			cub_is_border_valid(t_check_map *check_map);
t_map			*cub_load_map_values(char *file);
void			cub_check_and_set_player(t_mlx *mlx);
void			cub_set_player_start_position(t_mlx *mlx);
void			finalize_map(t_map *map, int height, int max_width);
t_map			*init_map_and_open_file(char *file, int *fd);
void			allocate_map_field(t_map *map);
void			process_lines(int fd, t_map *map);
unsigned int	cub_parse_rgb_string_to_hex(char *rgb_string);
int				cub_get_tile_size(t_map *map);

// Movement:
void			cub_check_key(mlx_key_data_t keydata, void *parsed_mlx);
void			cub_player_update_frame(t_mlx *mlx, double move_x,
					double move_y);

// Render:
void cast_rays(t_mlx *mlx);

// Utils:
void			*allocate_memory(size_t buffer_size);
void			*memory_realloc(void *ptr, size_t new_size);
void			free_memory(void *buffer);
void			cub_handle_error(int exit_code, char *message);
void			cub_exit(t_mlx *mlx);



void draw_rays(t_mlx *mlx);

#endif
