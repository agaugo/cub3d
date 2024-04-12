/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   map_initialization_utils.c                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/03 14:51:31 by trstn4        #+#    #+#                 */
/*   Updated: 2024/04/12 14:17:59 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

t_map	*init_map_and_open_file(char *file, int *fd)
{
	t_map	*map;

	map = ft_calloc(1, sizeof(t_map));
	*fd = open(file, O_RDONLY);
	if (!map || *fd < 0)
	{
		perror("Error\nInitialization or File Opening Failed");
		exit(1);
	}
	return (map);
}

void	allocate_map_field(t_map *map)
{
	map->field = ft_calloc(1, sizeof(char *));
	if (!map->field)
	{
		perror("Error\nMemory allocation failed for map field");
		exit(1);
	}
}

void	finalize_map(t_map *map, int height, int max_width)
{
	map->north_texture = mlx_load_png(map->id_no);
	map->south_texture = mlx_load_png(map->id_so);
	map->west_texture = mlx_load_png(map->id_we);
	map->east_texture = mlx_load_png(map->id_ea);
	map->color_ceiling = cub_parse_rgb_string_to_hex(map->id_c);
	map->color_floor = cub_parse_rgb_string_to_hex(map->id_f);
	map->field[height] = NULL;
	map->height = height;
	map->width = max_width;
    map->pixel_width_per_square = SCREEN_WIDTH / max_width;
    map->pixel_height_per_square = SCREEN_HEIGHT / height;
	map->tile_size = cub_get_tile_size(map);
}

int	cub_get_tile_size(t_map *map)
{
	int	tile_width;
	int	tile_height;
	int	tile_size;

	tile_width = (SCREEN_WIDTH / map->width);
	tile_height = (SCREEN_HEIGHT / map->height);
	if (tile_width < tile_height)
		tile_size = tile_width;
	else
		tile_size = tile_height;
	return (tile_size);
}
