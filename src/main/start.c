/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   start.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 11:53:08 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/06 13:47:26 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3D.h"

void cub_game_loop(void *ml) // game loop
{
	t_mlx *mlx;

	mlx = ml; // cast to the mlx structure
	mlx_delete_image(mlx->mlx_p, mlx->img); // delete the image
	mlx->img = mlx_new_image(mlx->mlx_p, S_W, S_H); // create new image
	cub_hook(mlx, 0, 0); // hook the player
	cub_cast_rays(mlx);
	mlx_image_to_window(mlx->mlx_p, mlx->img, 0, 0);
}

void cub_init_the_player(t_mlx mlx) // init the player structure
{
	mlx.player->pixel_x = mlx.map->start_x * TILE_SIZE + TILE_SIZE / 2; // player x position in pixels in the center of the tile
	mlx.player->pixel_y = mlx.map->start_y * TILE_SIZE + TILE_SIZE / 2; // player y position in pixels in the center of the tile
	mlx.player->fov_rd = (FOV * M_PI) / 180; // field of view in radians
	mlx.player->angle = M_PI; // player angle
}

void cub_start_game(t_map *map) // start the game
{
	t_mlx mlx;

	mlx.map = map; // init the mlx structure
	mlx.player = calloc(1, sizeof(t_player)); // init the player structure i'm using calloc to initialize the variables to zero
	mlx.ray = calloc(1, sizeof(t_ray)); // init the ray structure
	mlx.mlx_p = mlx_init(S_W, S_H, "cub3D", 0); // init the mlx pointer
	cub_init_the_player(mlx); // init the player structure
	mlx_loop_hook(mlx.mlx_p, &cub_game_loop, &mlx); // game loop
	mlx_key_hook(mlx.mlx_p, &cub_mlx_key, &mlx); // key press and release
	mlx_loop(mlx.mlx_p); // mlx loop
	cub_exit(&mlx); // exit the game
}
