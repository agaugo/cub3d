/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   move.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 11:53:06 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/06 13:47:40 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/cub3D.h"

void cub_reles(mlx_key_data_t keydata, t_mlx *mlx) // release the key
{
	if (keydata.key == MLX_KEY_D && (keydata.action == MLX_RELEASE))
		mlx->player->l_r = 0;
	else if (keydata.key == MLX_KEY_A && (keydata.action == MLX_RELEASE))
		mlx->player->l_r = 0;
	else if (keydata.key == MLX_KEY_S && (keydata.action == MLX_RELEASE))
		mlx->player->u_d = 0;
	else if (keydata.key == MLX_KEY_W && (keydata.action == MLX_RELEASE))
		mlx->player->u_d = 0;
	else if (keydata.key == MLX_KEY_LEFT && keydata.action == MLX_RELEASE)
		mlx->player->rot = 0;
	else if (keydata.key == MLX_KEY_RIGHT && keydata.action == MLX_RELEASE)
		mlx->player->rot = 0;
}

void cub_mlx_key(mlx_key_data_t keydata, void *ml) // key press
{
	t_mlx *mlx;

	mlx = ml;
	if (keydata.key == MLX_KEY_ESCAPE && (keydata.action == MLX_PRESS || keydata.action == MLX_REPEAT)) // exit the game
		cub_exit(mlx);
	else if (keydata.key == MLX_KEY_A && (keydata.action == MLX_PRESS)) // move left
		mlx->player->l_r = -1;
	else if (keydata.key == MLX_KEY_D && (keydata.action == MLX_PRESS)) // move right
		mlx->player->l_r = 1;
	else if (keydata.key == MLX_KEY_S && (keydata.action == MLX_PRESS)) // move down
		mlx->player->u_d = -1;
	else if (keydata.key == MLX_KEY_W && keydata.action == MLX_PRESS) // move up
		mlx->player->u_d = 1;
	else if (keydata.key == MLX_KEY_LEFT && keydata.action == MLX_PRESS) // rotate left
		mlx->player->rot = -1;
	else if (keydata.key == MLX_KEY_RIGHT && keydata.action == MLX_PRESS) // rotate right
		mlx->player->rot = 1;
	cub_reles(keydata, mlx); // release the key
}

void cub_rotate_player(t_mlx *mlx, int i) // rotate the player
{
	if (i == 1)
	{
		mlx->player->angle += ROTATION_SPEED; // rotate right
		if (mlx->player->angle > 2 * M_PI)
			mlx->player->angle -= 2 * M_PI;
	}
	else
	{
		mlx->player->angle -= ROTATION_SPEED; // rotate left
		if (mlx->player->angle < 0)
			mlx->player->angle += 2 * M_PI;
	}
}

void cub_move_player(t_mlx *mlx, double move_x, double move_y) // move the player
{
	int  map_grid_y;
	int  map_grid_x;
	int  new_x;
	int  new_y;

	new_x = roundf(mlx->player->pixel_x + move_x); // get the new x position
	new_y = roundf(mlx->player->pixel_y + move_y); // get the new y position
	map_grid_x = (new_x / TILE_SIZE); // get the x position in the map
	map_grid_y = (new_y / TILE_SIZE); // get the y position in the map
	if (mlx->map->field[map_grid_y][map_grid_x] != '1' && \
	(mlx->map->field[map_grid_y][mlx->player->pixel_x / TILE_SIZE] != '1' && \
	mlx->map->field[mlx->player->pixel_y / TILE_SIZE][map_grid_x] != '1')) // check the wall hit and the diagonal wall hit
	{
		mlx->player->pixel_x = new_x; // move the player
		mlx->player->pixel_y = new_y; // move the player
	}
}

void cub_hook(t_mlx *mlx, double move_x, double move_y) // hook the player
{
	if (mlx->player->rot == 1) //rotate right
		cub_rotate_player(mlx, 1);
	if (mlx->player->rot == -1) //rotate left
		cub_rotate_player(mlx, 0);
	if (mlx->player->l_r == 1) //move right
	{
		move_x = -sin(mlx->player->angle) * PLAYER_SPEED;
		move_y = cos(mlx->player->angle) * PLAYER_SPEED;
	}
	if (mlx->player->l_r == -1) //move left
	{
		move_x = sin(mlx->player->angle) * PLAYER_SPEED;
		move_y = -cos(mlx->player->angle) * PLAYER_SPEED;
	}
	if (mlx->player->u_d == 1) //move up
	{
		move_x = cos(mlx->player->angle) * PLAYER_SPEED;
		move_y = sin(mlx->player->angle) * PLAYER_SPEED;
	}
	if (mlx->player->u_d == -1) //move down
	{
		move_x = -cos(mlx->player->angle) * PLAYER_SPEED;
		move_y = -sin(mlx->player->angle) * PLAYER_SPEED;
	}
	cub_move_player(mlx, move_x, move_y); // move the player
}
