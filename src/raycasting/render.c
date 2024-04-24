/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/07 17:30:23 by trstn4        #+#    #+#                 */
/*   Updated: 2024/04/24 15:44:30 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

#define RAY_LENGTH 100000

int	reverse_bytes(int color)
{
	return (((color & 0xFF) << 24)
		| ((color & 0xFF00) << 8)
		| ((color & 0xFF0000) >> 8)
		| ((color & 0xFF000000) >> 24));
}

void	my_mlx_pixel_put(t_mlx *mlx, int x, int y, int color)
{
	if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
		mlx_put_pixel(mlx->img, x, y, color);
}

double	distance_between_points(double x1, double y1, double x2, double y2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

void	draw_line(t_mlx *mlx, int x0, int y0, int x1, int y1, int color)
{
	int	dx;
	int	sx;
	int	dy;
	int	sy;
	int	e2;
	int	err;

	dx = abs(x1 - x0);
	sx = x0 < x1 ? 1 : -1;
	dy = -abs(y1 - y0);
	sy = y0 < y1 ? 1 : -1;
	err = dx + dy;
	while (1 == 1)
	{
		my_mlx_pixel_put(mlx, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break;
		e2 = 2 * err;
		if (e2 >= dy)
			err += dy; x0 += sx;
		if (e2 <= dx)
			err += dx; y0 += sy;
	}
}


int	cub_is_wall(t_mlx *mlx, int x, int y)
{
	int	map_grid_x;
	int	map_grid_y;

	map_grid_x = x / mlx->map->pixel_width_per_square;
	map_grid_y = y / mlx->map->pixel_height_per_square;
	if (map_grid_x < 0 || map_grid_x >= mlx->map->width || map_grid_y < 0 \
		|| map_grid_y >= mlx->map->height)
		return (1);
	if (mlx->map->field[map_grid_y][map_grid_x] == '1')
		return (1);
	return (0);
}

mlx_texture_t*	cub_select_texture(t_mlx *mlx, t_ray *ray, double horz_hit_distance, double vert_hit_distance)
{
	if (horz_hit_distance < vert_hit_distance)
	{
		if (ray->is_ray_facing_down)
			return mlx->map->south_texture;
		else
			return mlx->map->north_texture;
	}
	else
	{
		if (ray->is_ray_facing_right)
			return mlx->map->east_texture;
		else
			return mlx->map->west_texture;
	}
}

void	cub_draw_wall(t_mlx *mlx, t_ray *ray)
{
	int		texture_x;
	double	texture_step;
	double	texture_pos;
	int		y;
	int		tex_y;
	
	texture_x = (int)(fmod(ray->wall_hit_pos, TILE_SIZE) / TILE_SIZE * \
		ray->texture->width);
	texture_step = ray->texture->height / ray->wall_slice_height;
	texture_pos = (ray->draw_start - SCREEN_HEIGHT / 2 + \
		ray->wall_slice_height / 2) * texture_step;
	y = ray->draw_start;
	while (y < ray->draw_end)
	{
		tex_y = (int)texture_pos % ray->texture->height;
		my_mlx_pixel_put(mlx, ray->ray_num, y, reverse_bytes(( \
			(uint32_t*)ray->texture->pixels)[tex_y * \
			ray->texture->width + texture_x]));
		texture_pos += texture_step;
		y++;
	}
}

void	cub_draw(t_mlx *mlx, t_ray *ray)
{
	int	y;

	cub_draw_wall(mlx, ray);
	y = 0;
	while (y < ray->draw_start)
	{
		my_mlx_pixel_put(mlx, ray->ray_num, y, mlx->map->color_ceiling);
		y++;
	}
	y = ray->draw_end;
	while (y < SCREEN_HEIGHT)
	{
		my_mlx_pixel_put(mlx, ray->ray_num, y, mlx->map->color_floor);
		y++;
	}
}

double	tt(t_mlx *mlx, t_ray *ray, double *horz_hit_x)
{
	double yintercept;
	double xintercept;
	double ystep;
	double xstep;
	double next_horz_touch_x;
	double next_horz_touch_y;
	double horz_hit_y;
	
	ystep = mlx->map->pixel_height_per_square;
	ystep *= ray->is_ray_facing_down ? 1 : -1;
	xstep = mlx->map->pixel_height_per_square / tan(ray->ray_angle);
	xstep *= (ray->is_ray_facing_right && xstep < 0) ? -1 : 1;
	xstep *= (!ray->is_ray_facing_right && xstep > 0) ? -1 : 1;
	yintercept = floor(mlx->player->pixel_y / mlx->map->pixel_height_per_square) * mlx->map->pixel_height_per_square;
	yintercept += ray->is_ray_facing_down ? mlx->map->pixel_height_per_square : 0;
	next_horz_touch_y = yintercept;
	xintercept = mlx->player->pixel_x + (yintercept - mlx->player->pixel_y) / tan(ray->ray_angle);
	next_horz_touch_x = xintercept;
	while (next_horz_touch_x >= 0 && next_horz_touch_x < mlx->map->width * mlx->map->pixel_width_per_square &&
		   next_horz_touch_y >= 0 && next_horz_touch_y < mlx->map->height * mlx->map->pixel_height_per_square)
	{
		if (cub_is_wall(mlx, next_horz_touch_x, next_horz_touch_y - (ray->is_ray_facing_down ? 0 : 1)))
		{
			*horz_hit_x = next_horz_touch_x;
			horz_hit_y = next_horz_touch_y;
			return (distance_between_points(mlx->player->pixel_x, mlx->player->pixel_y, *horz_hit_x, horz_hit_y));
		}
		else
		{
			next_horz_touch_x += xstep;
			next_horz_touch_y += ystep;
		}
	}
	return (INFINITY);
}

double	ll(t_mlx *mlx, t_ray *ray, double *vert_hit_y)
{
	double ystep_vert;
	double xintercept_vert;
	double yintercept_vert;
	double xstep_vert;
	double next_vert_touch_x;
	double next_vert_touch_y;
	double vert_hit_x;
	
	xintercept_vert = floor(mlx->player->pixel_x / mlx->map->pixel_width_per_square) * mlx->map->pixel_width_per_square;
	xintercept_vert += ray->is_ray_facing_right ? mlx->map->pixel_width_per_square : 0;
	yintercept_vert = mlx->player->pixel_y + (xintercept_vert - mlx->player->pixel_x) * tan(ray->ray_angle);
		
	xstep_vert = mlx->map->pixel_width_per_square;
	xstep_vert *= ray->is_ray_facing_right ? 1 : -1;
		
	ystep_vert = mlx->map->pixel_width_per_square * tan(ray->ray_angle);
	ystep_vert *= (ray->is_ray_facing_down && ystep_vert < 0) ? -1 : 1;
	ystep_vert *= (!ray->is_ray_facing_down && ystep_vert > 0) ? -1 : 1;

	next_vert_touch_x = xintercept_vert;
	next_vert_touch_y = yintercept_vert;

	while (next_vert_touch_x >= 0 && next_vert_touch_x < mlx->map->width * mlx->map->pixel_width_per_square &&
		   next_vert_touch_y >= 0 && next_vert_touch_y < mlx->map->height * mlx->map->pixel_height_per_square)
	{
		if (cub_is_wall(mlx, next_vert_touch_x - (ray->is_ray_facing_right ? 0 : 1), next_vert_touch_y))
		{
			vert_hit_x = next_vert_touch_x;
			*vert_hit_y = next_vert_touch_y;
			return (distance_between_points(mlx->player->pixel_x, mlx->player->pixel_y, vert_hit_x, *vert_hit_y));
		}
		else
		{
			next_vert_touch_x += xstep_vert;
			next_vert_touch_y += ystep_vert;
		}
	}
	return (INFINITY);
}

void	cub_cast_single_ray(t_mlx *mlx, t_ray *ray)
{
	double	horz_hit_x;
	double	vert_hit_y;
	double	horz_hit_distance;
	double	vert_hit_distance;

	horz_hit_x = 0;
	vert_hit_y = 0;
	horz_hit_distance = tt(mlx, ray, &horz_hit_x);
	vert_hit_distance = ll(mlx, ray, &vert_hit_y);
	ray->is_ray_facing_down = ray->ray_angle > 0 && ray->ray_angle < M_PI;
	ray->is_ray_facing_right = ray->ray_angle < 0.5 * M_PI || ray->ray_angle > \
		1.5 * M_PI;
	ray->texture = cub_select_texture(mlx, ray, horz_hit_distance, \
		vert_hit_distance);
	ray->perp_distance = min(horz_hit_distance, vert_hit_distance) * \
		cos(ray->ray_angle - mlx->player->angle);
	if (ray->perp_distance > 0)
	{
		ray->wall_slice_height = (TILE_SIZE / ray->perp_distance) * \
			ray->distance_to_proj_plane;
	}
	else
		ray->wall_slice_height = SCREEN_HEIGHT;
	ray->draw_start = max(0, SCREEN_HEIGHT / 2 - \
		(int)(ray->wall_slice_height / 2));
	ray->draw_end = min(SCREEN_HEIGHT - 1, SCREEN_HEIGHT / 2 + \
		(ray->wall_slice_height / 2));
	if (vert_hit_distance < horz_hit_distance)
		ray->wall_hit_pos = vert_hit_y;
	else
		ray->wall_hit_pos = horz_hit_x;
	cub_draw(mlx, ray);
}

void	cub_cast_rays(t_mlx *mlx)
{
	t_ray	ray;

	ray.ray_angle = mlx->player->angle - (FOV / 2.0 * (M_PI / 180.0));
	ray.angle_increment = FOV / (float)SCREEN_WIDTH * (M_PI / 180.0);
	ray.distance_to_proj_plane = (SCREEN_WIDTH / 2.0) / tan(FOV / 2.0 * \
		(M_PI / 180.0));
	ray.ray_num = 0;
	while (ray.ray_num < SCREEN_WIDTH)
	{
		ray.ray_angle = fmod(ray.ray_angle, 2.0 * M_PI);
		if (ray.ray_angle < 0)
			ray.ray_angle += 2.0 * M_PI;
		cub_cast_single_ray(mlx, &ray);
		ray.ray_angle += ray.angle_increment;
		ray.ray_num++;
	}
}
