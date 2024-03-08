/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/07 17:30:23 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/08 21:51:06 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3D.h"

#define RAY_LENGTH 100000

void	cub_set_pixel(t_mlx *mlx, int x, int y, int color)
{
	if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
		mlx_put_pixel(mlx->img, x, y, color);
}

void	cub_draw_vertical_line(t_mlx *mlx, int x, float wall_height)
{
	int	y;
	int	start;
	int	end;

	start = (SCREEN_HEIGHT / 2) - (wall_height / 2);
	end = start + wall_height;
	if (start < 0)
		start = 0;
	if (end > SCREEN_HEIGHT)
		end = SCREEN_HEIGHT;
	y = 0;
	while (y++ < start)
		cub_set_pixel(mlx, x, y, 0x444444);
	y = start;
	while (y++ < end)
		cub_set_pixel(mlx, x, y, 0xFFFFFF);
	y = end;
	while (y++ < SCREEN_HEIGHT)
		cub_set_pixel(mlx, x, y, 0xA9A9A9);
}

void	cub_handle_wall_found(t_mlx *mlx, float ray_x, float ray_y, float ray_angle, int ray_num, float projection_distance)
{
	float delta_x;
	float delta_y;
	float direct_distance;
	float perpendicular_distance;
	float wall_height;
	
	delta_x = ray_x - mlx->player->pixel_x;
	delta_y = ray_y - mlx->player->pixel_y;
	direct_distance = sqrt(delta_x * delta_x + delta_y * delta_y);
	perpendicular_distance = direct_distance * cos(ray_angle - mlx->player->angle);
	wall_height = (mlx->map->tile_size / perpendicular_distance) * projection_distance;
	cub_draw_vertical_line(mlx, ray_num, wall_height);
}

void	cub_cast_single_ray(t_mlx *mlx, float ray_angle, int ray_num, float projection_distance)
{
	float ray_x = mlx->player->pixel_x;
	float ray_y = mlx->player->pixel_y;
	int i;

	i = 0;
	while (i++ < RAY_LENGTH)
	{
		int map_x = (int)ray_x / mlx->map->tile_size;
		int map_y = (int)ray_y / mlx->map->tile_size;

		if (mlx->map->field[map_y][map_x] == '1') {
			cub_handle_wall_found(mlx, ray_x, ray_y, ray_angle, ray_num, projection_distance);
			break;
		}
		ray_x += cos(ray_angle);
		ray_y += sin(ray_angle);
	}
}

void	cub_init_ray_casting(t_mlx *mlx, float *ray_angle, float *angle_increment, float *projection_distance)
{
	*ray_angle = mlx->player->angle - (FOV / 2.0 * (M_PI / 180.0));
	*angle_increment = FOV / (float)SCREEN_WIDTH * (M_PI / 180.0);
	*projection_distance = (SCREEN_WIDTH / 2) / tan(FOV / 2.0 * (M_PI / 180.0));
}

void	cub_cast_rays(t_mlx *mlx)
{
	float ray_angle;
	float angle_increment;
	float projection_distance;
	int ray_num = 0;
	
	cub_init_ray_casting(mlx, &ray_angle, &angle_increment, &projection_distance);
	while (ray_num++ < SCREEN_WIDTH)
	{
		ray_angle = fmod(ray_angle, 2.0 * M_PI);
		if (ray_angle < 0)
			ray_angle += 2.0 * M_PI;
		cub_cast_single_ray(mlx, ray_angle, ray_num, projection_distance);
		ray_angle += angle_increment;
	}
}
