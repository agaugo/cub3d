/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   move.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 11:53:06 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/06 21:14:32 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3D.h"
#include <math.h> // Include the math library for sin, cos, and conversion from degrees to radians

void key_event(mlx_key_data_t keydata, void* ml) {
    t_mlx *mlx;

    mlx = ml;

    // Convert rotation from degrees to radians for trigonometric calculations
    double rotation_radians = mlx->player->rotation * (M_PI / 180.0);

    if (keydata.key == MLX_KEY_W) {
        mlx->player->pixel_x += cos(rotation_radians) * PLAYER_SPEED;
        mlx->player->pixel_y += sin(rotation_radians) * PLAYER_SPEED;
    }
    if (keydata.key == MLX_KEY_S) {
        mlx->player->pixel_x -= cos(rotation_radians) * PLAYER_SPEED;
        mlx->player->pixel_y -= sin(rotation_radians) * PLAYER_SPEED;
    }
    if (keydata.key == MLX_KEY_A) {
        mlx->player->pixel_x -= sin(rotation_radians) * PLAYER_SPEED;
        mlx->player->pixel_y += cos(rotation_radians) * PLAYER_SPEED;
    }
    if (keydata.key == MLX_KEY_D) {
        mlx->player->pixel_x += sin(rotation_radians) * PLAYER_SPEED;
        mlx->player->pixel_y -= cos(rotation_radians) * PLAYER_SPEED;
    }
    if (keydata.key == MLX_KEY_LEFT) {
        mlx->player->rotation -= ROTATION_SPEED;
        if (mlx->player->rotation < 0) {
            mlx->player->rotation += 360;
        }
    }
    if (keydata.key == MLX_KEY_RIGHT) {
        mlx->player->rotation += ROTATION_SPEED;
        if (mlx->player->rotation >= 360) {
            mlx->player->rotation -= 360;
        }
    }
    printf("X: %d, Y: %d, Rotation: %d\n", mlx->player->pixel_x, mlx->player->pixel_y, mlx->player->rotation);
}
