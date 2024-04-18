/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   move.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 11:53:06 by trstn4        #+#    #+#                 */
/*   Updated: 2024/04/18 14:39:27 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	cub_is_wall_hit(t_mlx *mlx, int map_grid_x, int map_grid_y)
{
    if (map_grid_x < 0 || map_grid_x >= mlx->map->width ||
        map_grid_y < 0 || map_grid_y >= mlx->map->height) {
        return 1; // Out of bounds is treated as a wall
    }
    return (mlx->map->field[map_grid_y][map_grid_x] == '1'); // '1' is a wall
}

// Move the player if the next position is not a wall
void	cub_move_player(t_mlx *mlx, double move_x, double move_y)
{
    int new_player_pixel_x = (int)(mlx->player->pixel_x + move_x);
    int new_player_pixel_y = (int)(mlx->player->pixel_y + move_y);
    
	        int pixel_height_per_square;
    int pixel_width_per_square;

    // Calculate the pixel size of each square
    pixel_width_per_square = SCREEN_WIDTH / mlx->map->width;
    pixel_height_per_square = SCREEN_HEIGHT / mlx->map->height;
	
    // Convert pixel position to grid coordinates
    int new_map_grid_x = new_player_pixel_x / pixel_width_per_square;
    int new_map_grid_y = new_player_pixel_y / pixel_height_per_square;
    
    // Check the grid coordinates the player is trying to move to
    if (!cub_is_wall_hit(mlx, new_map_grid_x, new_map_grid_y))
    {
        // No wall hit, update player position
        mlx->player->pixel_x = new_player_pixel_x;
        mlx->player->pixel_y = new_player_pixel_y;
    }
}

void	cub_rotate_player(t_mlx *mlx)
{
	if (mlx->key->key_l_r == 'r')
	{
		mlx->player->angle += ROTATION_SPEED;
		if (mlx->player->angle > 2 * M_PI)
			mlx->player->angle -= 2 * M_PI;
	}
	if (mlx->key->key_l_r == 'l')
	{
		mlx->player->angle -= ROTATION_SPEED;
		if (mlx->player->angle < 0)
			mlx->player->angle += 2 * M_PI;
	}
}

void	cub_player_update_frame(t_mlx *mlx, double move_x, double move_y)
{
	if (mlx->key->key_w_s == 'w')
	{
		move_x = cos(mlx->player->angle) * PLAYER_SPEED;
		move_y = sin(mlx->player->angle) * PLAYER_SPEED;
	}
	if (mlx->key->key_w_s == 's')
	{
		move_x = -cos(mlx->player->angle) * PLAYER_SPEED;
		move_y = -sin(mlx->player->angle) * PLAYER_SPEED;
	}
	if (mlx->key->key_a_d == 'a')
	{
		move_x = sin(mlx->player->angle) * PLAYER_SPEED;
		move_y = -cos(mlx->player->angle) * PLAYER_SPEED;
	}
	if (mlx->key->key_a_d == 'd')
	{
		move_x = -sin(mlx->player->angle) * PLAYER_SPEED;
		move_y = cos(mlx->player->angle) * PLAYER_SPEED;
	}
	cub_rotate_player(mlx);
	cub_move_player(mlx, move_x, move_y);
}
