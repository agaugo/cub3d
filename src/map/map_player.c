/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   map_player.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/08 11:18:29 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/08 15:31:16 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/cub3D.h"

void cub_set_player_start_position(t_mlx *mlx) {
    mlx->player->pixel_x = mlx->map->player_start_x * mlx->map->tile_size + (mlx->map->tile_size / 2);
    mlx->player->pixel_y = mlx->map->player_start_y * mlx->map->tile_size + (mlx->map->tile_size / 2);

    mlx->player->angle = 0;
}
