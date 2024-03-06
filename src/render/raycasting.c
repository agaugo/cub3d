/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   raycasting.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 11:53:03 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/06 13:47:43 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/cub3D.h"

void cub_my_mlx_pixel_put(t_mlx *mlx, int x, int y, int color) // put the pixel
{
	if (x < 0) // check the x position
		return ;
	else if (x >= S_W)
		return ;
	if (y < 0) // check the y position
		return ;
	else if (y >= S_H)
		return ;
	mlx_put_pixel(mlx->img, x, y, color); // put the pixel
}

float cub_nor_angle(float angle) // normalize the angle
{
	if (angle < 0)
		angle += (2 * M_PI);
	if (angle > (2 * M_PI))
		angle -= (2 * M_PI);
	return (angle);
}

void cub_draw_floor_ceiling(t_mlx *mlx, int ray, int t_pix, int b_pix) // draw the floor and the ceiling
{
	int  i;

	i = b_pix;
	while (i < S_H)
		cub_my_mlx_pixel_put(mlx, ray, i++, 0xB99470FF); // floor
	i = 0;
	while (i < t_pix)
		cub_my_mlx_pixel_put(mlx, ray, i++, 0x89CFF3FF); // ceiling
}

int cub_get_color(t_mlx *mlx, int flag) // get the color of the wall
{
	mlx->ray->ray_ngl = cub_nor_angle(mlx->ray->ray_ngl); // normalize the angle
	if (flag == 0)
	{
		if (mlx->ray->ray_ngl > M_PI / 2 && mlx->ray->ray_ngl < 3 * (M_PI / 2))
			return (0xB5B5B5FF); // west wall
		else
			return (0xB5B5B5FF); // east wall
	}
	else
	{
		if (mlx->ray->ray_ngl > 0 && mlx->ray->ray_ngl < M_PI)
			return (0xF5F5F5FF); // south wall
		else
			return (0xF5F5F5FF); // north wall
	}
}

void cub_draw_wall(t_mlx *mlx, int ray, int t_pix, int b_pix) // draw the wall
{
	int color;

	color = cub_get_color(mlx, mlx->ray->flag);
	while (t_pix < b_pix)
		cub_my_mlx_pixel_put(mlx, ray, t_pix++, color);
}

void cub_render_wall(t_mlx *mlx, int ray) // render the wall
{
	double wall_h;
	double b_pix;
	double t_pix;

	mlx->ray->distance *= cos(cub_nor_angle(mlx->ray->ray_ngl - mlx->player->angle)); // fix the fisheye
	wall_h = (TILE_SIZE / mlx->ray->distance) * ((S_W / 2) / tan(mlx->player->fov_rd / 2)); // get the wall height
	b_pix = (S_H / 2) + (wall_h / 2); // get the bottom pixel
	t_pix = (S_H / 2) - (wall_h / 2); // get the top pixel
	if (b_pix > S_H) // check the bottom pixel
		b_pix = S_H;
	if (t_pix < 0) // check the top pixel
		t_pix = 0;
	cub_draw_wall(mlx, ray, t_pix, b_pix); // draw the wall
	cub_draw_floor_ceiling(mlx, ray, t_pix, b_pix); // draw the floor and the ceiling
}

int cub_unit_circle(float angle, char c) // check the unit circle
{
	if (c == 'x')
	{
		if (angle > 0 && angle < M_PI)
			return (1);
	}
	else if (c == 'y')
	{
		if (angle > (M_PI / 2) && angle < (3 * M_PI) / 2)
			return (1);
	}
	return (0);
}

int cub_inter_check(float angle, float *inter, float *step, int is_horizon) // check the intersection
{
	if (is_horizon)
	{
		if (angle > 0 && angle < M_PI)
		{
			*inter += TILE_SIZE;
			return (-1);
		}
		*step *= -1;
	}
	else
	{
		if (!(angle > M_PI / 2 && angle < 3 * M_PI / 2)) 
		{
			*inter += TILE_SIZE;
			return (-1);
		}
		*step *= -1;
	}
	return (1);
}

int cub_wall_hit(float x, float y, t_mlx *mlx) // check the wall hit
{
	int  x_m;
	int  y_m;

	if (x < 0 || y < 0)
		return (0);
	x_m = floor (x / TILE_SIZE); // get the x position in the map
	y_m = floor (y / TILE_SIZE); // get the y position in the map
	if ((y_m >= mlx->map->height || x_m >= mlx->map->width))
		return (0);
	if (mlx->map->field[y_m] && x_m <= (int)strlen(mlx->map->field[y_m]))
		if (mlx->map->field[y_m][x_m] == '1')
			return (0);
	return (1);
}

float cub_get_h_inter(t_mlx *mlx, float angl) // get the horizontal intersection
{
	float h_x;
	float h_y;
	float x_step;
	float y_step;
	int  pixel;

	y_step = TILE_SIZE;
	x_step = TILE_SIZE / tan(angl);
	h_y = floor(mlx->player->pixel_y / TILE_SIZE) * TILE_SIZE;
	pixel = cub_inter_check(angl, &h_y, &y_step, 1);
	h_x = mlx->player->pixel_x + (h_y - mlx->player->pixel_y) / tan(angl);
	if ((cub_unit_circle(angl, 'y') && x_step > 0) || (!cub_unit_circle(angl, 'y') && x_step < 0)) // check x_step value
		x_step *= -1;
	while (cub_wall_hit(h_x, h_y - pixel, mlx)) // check the wall hit whit the pixel value
	{
		h_x += x_step;
		h_y += y_step;
	}
	return (sqrt(pow(h_x - mlx->player->pixel_x, 2) + pow(h_y - mlx->player->pixel_y, 2))); // get the distance
}

float cub_get_v_inter(t_mlx *mlx, float angl) // get the vertical intersection
{
	float v_x;
	float v_y;
	float x_step;
	float y_step;
	int  pixel;

	x_step = TILE_SIZE; 
	y_step = TILE_SIZE * tan(angl);
	v_x = floor(mlx->player->pixel_x / TILE_SIZE) * TILE_SIZE;
	pixel = cub_inter_check(angl, &v_x, &x_step, 0); // check the intersection and get the pixel value
	v_y = mlx->player->pixel_y + (v_x - mlx->player->pixel_x) * tan(angl);
	if ((cub_unit_circle(angl, 'x') && y_step < 0) || (!cub_unit_circle(angl, 'x') && y_step > 0)) // check y_step value
		y_step *= -1;
	while (cub_wall_hit(v_x - pixel, v_y, mlx)) // check the wall hit whit the pixel value
	{
		v_x += x_step;
		v_y += y_step;
	}
	return (sqrt(pow(v_x - mlx->player->pixel_x, 2) + pow(v_y - mlx->player->pixel_y, 2))); // get the distance
}

void cub_cast_rays(t_mlx *mlx) // cast the rays
{
	double h_inter;
	double v_inter;
	int  ray;

	ray = 0;
	mlx->ray->ray_ngl = mlx->player->angle - (mlx->player->fov_rd / 2); // the start angle
	while (ray < S_W) // loop for the rays
	{
		mlx->ray->flag = 0; // flag for the wall
		h_inter = cub_get_h_inter(mlx, cub_nor_angle(mlx->ray->ray_ngl)); // get the horizontal intersection
		v_inter = cub_get_v_inter(mlx, cub_nor_angle(mlx->ray->ray_ngl)); // get the vertical intersection
		if (v_inter <= h_inter) // check the distance
			mlx->ray->distance = v_inter; // get the distance
		else
		{
			mlx->ray->distance = h_inter; // get the distance
			mlx->ray->flag = 1; // flag for the wall
		}
		cub_render_wall(mlx, ray); // render the wall
		ray++; // next ray
		mlx->ray->ray_ngl += (mlx->player->fov_rd / S_W); // next angle
	}
}
