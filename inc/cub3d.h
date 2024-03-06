/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cub3d.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 11:52:59 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/06 20:50:05 by trstn4        ########   odam.nl         */
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

#define SCREEN_WIDTH 1300  // make sure these match your actual screen dimensions
#define SCREEN_HEIGHT 700
#define MAP_WIDTH 24  // based on the number of columns in your map
#define MAP_HEIGHT 6   // based on the number of rows in your map

#define pixel_width_per_square (SCREEN_WIDTH / MAP_WIDTH)
#define pixel_height_per_square (SCREEN_HEIGHT / MAP_HEIGHT)

# define TILE_SIZE 30 // tile size
# define FOV 60 // field of view
# define ROTATION_SPEED 2 // rotation speed
# define PLAYER_SPEED 2 // player speed

typedef struct s_player //the player structure
{
	int  pixel_x;
	int  pixel_y;
	double angle;
	float fov_rd;
	int  rotation; // rotation flag
	int  l_r; // left right flag
	int  u_d; // up down flag
}	t_player;

typedef struct s_ray //the ray structure
{
	double ray_ngl; // ray angle
	double distance; // distance to the wall
	int  flag;  // flag for the wall
}	t_ray;

typedef struct s_map
{
	char **field;
	int  start_x;
	int  start_y;
	int  width;
	int  height;
}	t_map;

typedef struct s_mlx
{
	mlx_image_t  *img; // the image
	mlx_t   *mlx_p; // the mlx pointer
	t_ray   *ray; // the ray structure
	t_map   *map; // the data structure
	t_player  *player; // the player structure
}	t_mlx;

int		cub_is_border_valid(const char *file);
int		cub_setup_map(char *file);
t_map	*cub_load_map_values(char *file);
int		cub_validate_map(char *file);
void	cub_start_game(t_map *map);
void	cub_hook(t_mlx *mlx, double move_x, double move_y);
void	cub_exit(t_mlx *mlx);
void	cub_mlx_key(mlx_key_data_t keydata, void *ml);
void	cub_handle_error(int exit_code, char *message);
void	*memory_realloc(void *ptr, size_t new_size);
void	free_memory(void *buffer);
void	*allocate_memory(size_t buffer_size);
void	key_event(mlx_key_data_t keydata, void* ml);

#endif
