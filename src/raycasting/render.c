/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/07 17:30:23 by trstn4        #+#    #+#                 */
/*   Updated: 2024/04/05 12:34:02 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	get_rgba(int r, int g, int b, int a)
{
	return (r << 24 | g << 16 | b << 8 | a << 0);
}

int	reverse_bytes(int c)
{
	unsigned int	b;

	b = 0;
	b |= (c & 0xFF) << 24;
	b |= (c & 0xFF00) << 8;
	b |= (c & 0xFF0000) >> 8;
	b |= (c & 0xFF000000) >> 24;
	return (b);
}

void my_mlx_pixel_put(t_mlx *mlx, int x, int y, int color)
{
	if (x < 0)
		return ;
	else if (x >= SCREEN_WIDTH)
		return ;
	if (y < 0)
		return ;
	else if (y >= SCREEN_HEIGHT)
		return ;
	mlx_put_pixel(mlx->img, x, y, color);
}

float nor_angle(float angle)
{
	if (angle < 0)
		angle += (2 * M_PI);
	if (angle > (2 * M_PI))
		angle -= (2 * M_PI);
	return (angle);
}

void draw_floor_ceiling(t_mlx *mlx, int ray, int t_pix, int b_pix)
{
	int	i;

	i = b_pix;
	while (i < SCREEN_HEIGHT)
		my_mlx_pixel_put(mlx, ray, i++, mlx->map->color_ceiling);
	i = 0;
	while (i < t_pix)
		my_mlx_pixel_put(mlx, ray, i++, mlx->map->color_floor);
}

mlx_texture_t	*get_texture(t_mlx *mlx, int flag)
{
	mlx->ray->ray_ngl = nor_angle(mlx->ray->ray_ngl);
	if (flag == 0)
	{
		if (mlx->ray->ray_ngl > M_PI / 2 && mlx->ray->ray_ngl < 3 * (M_PI / 2))
			return (mlx->tex->ea);
		else
			return (mlx->tex->we);
	}
	else
	{
		if (mlx->ray->ray_ngl > 0 && mlx->ray->ray_ngl < M_PI)
			return (mlx->tex->so);
		else
			return (mlx->tex->no);
	}
}

double	get_x_o(mlx_texture_t	*texture, t_mlx *mlx)
{
	double	x_o;

	if (mlx->ray->flag == 1)
		x_o = (int)fmodf((mlx->ray->horiz_x * \
		(texture->width / mlx->map->tile_size)), texture->width);
	else
		x_o = (int)fmodf((mlx->ray->vert_y * \
		(texture->width / mlx->map->tile_size)), texture->width);
	printf("wall_x: %f, texture width: %d, x_o: %f\n", mlx->ray->horiz_x, texture->width, x_o);
	return (x_o);
}

void	draw_wall(t_mlx *mlx, int t_pix, int b_pix, double wall_h)
{
	double			x_o;
	double			y_o;
	mlx_texture_t	*texture;
	uint32_t		*arr;
	double			factor;

	texture = get_texture(mlx, mlx->ray->flag);
	arr = (uint32_t *)texture->pixels;
	factor = (double)texture->height / wall_h;
	x_o = get_x_o(texture, mlx);
	y_o = (t_pix - (SCREEN_HEIGHT / 2) + (wall_h / 2)) * factor;
	if (y_o < 0)
		y_o = 0;
	while (t_pix < b_pix)
	{
		my_mlx_pixel_put(mlx, mlx->ray->index, t_pix, reverse_bytes \
		(arr[(int)y_o * texture->width + (int)x_o]));
		y_o += factor;
		t_pix++;
	}
}

void	render_wall(t_mlx *mlx, int ray)
{
	double	wall_h;
	double	b_pix;
	double	t_pix;

	mlx->ray->distance *= cos(nor_angle(mlx->ray->ray_ngl - mlx->player->angle));
	wall_h = (mlx->map->tile_size / mlx->ray->distance) * ((SCREEN_WIDTH / 2) / \
	tan(mlx->player->fov_rd / 2));
	b_pix = (SCREEN_HEIGHT / 2) + (wall_h / 2);
	t_pix = (SCREEN_HEIGHT / 2) - (wall_h / 2);
	if (b_pix > SCREEN_HEIGHT)
		b_pix = SCREEN_HEIGHT;
	if (t_pix < 0)
		t_pix = 0;
	mlx->ray->index = ray;
	draw_wall(mlx, t_pix, b_pix, wall_h);
	draw_floor_ceiling(mlx, ray, t_pix, b_pix);
}

int	unit_circle(float angle, char c)
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

int inter_check(t_mlx *mlx, float angle, float *inter, float *step, int is_horizon) // check the intersection
{
 if (is_horizon)
 {
  if (angle > 0 && angle < M_PI)
  {
   *inter += mlx->map->tile_size;
   return (-1);
  }
  *step *= -1;
 }
 else
 {
  if (!(angle > M_PI / 2 && angle < 3 * M_PI / 2)) 
  {
   *inter += mlx->map->tile_size;
   return (-1);
  }
  *step *= -1;
 }
 return (1);
}

int wall_hit(float x, float y, t_mlx *mlx) // check the wall hit
{
 int  x_m;
 int  y_m;

 if (x < 0 || y < 0)
  return (0);
 x_m = floor (x / mlx->map->tile_size); // get the x position in the map
 y_m = floor (y / mlx->map->tile_size); // get the y position in the map
 if ((y_m >= mlx->map->height || x_m >= mlx->map->width))
  return (0);
 if (mlx->map->field[y_m] && x_m <= (int)strlen(mlx->map->field[y_m]))
 {
  if (mlx->map->field[y_m][x_m] == '1')
   return (0);
 }
 return (1);
}

float	get_h_inter(t_mlx *mlx, float angl)
{
	float	h_x;
	float	h_y;
	float	x_step;
	float	y_step;
	int		pixel;

	y_step = mlx->map->tile_size;
	x_step = mlx->map->tile_size / tan(angl);
	h_y = floor(mlx->player->pixel_y / mlx->map->tile_size) * mlx->map->tile_size;
	pixel = inter_check(mlx, angl, &h_y, &y_step, 1);
	h_x = mlx->player->pixel_x + (h_y - mlx->player->pixel_y) / tan(angl);
	if ((unit_circle(angl, 'y') && x_step > 0) || (!unit_circle(angl, 'y') && x_step < 0))
		x_step *= -1;
	while (wall_hit(h_x, h_y - pixel, mlx))
	{
		h_x += x_step;
		h_y += y_step;
	}
	mlx->ray->horiz_x = h_x;
	mlx->ray->horiz_y = h_y;
	return (sqrt(pow(h_x - mlx->player->pixel_x, 2) + \
	pow(h_y - mlx->player->pixel_y, 2)));
}

float	get_v_inter(t_mlx *mlx, float angl)
{
	float	v_x;
	float	v_y;
	float	x_step;
	float	y_step;
	int		pixel;

	x_step = mlx->map->tile_size;
	y_step = mlx->map->tile_size * tan(angl);
	v_x = floor(mlx->player->pixel_x / mlx->map->tile_size) * mlx->map->tile_size;
	pixel = inter_check(mlx, angl, &v_x, &x_step, 0);
	v_y = mlx->player->pixel_y + (v_x - mlx->player->pixel_x) * tan(angl);
	if ((unit_circle(angl, 'x') && y_step < 0) || (!unit_circle(angl, 'x') && y_step > 0))
		y_step *= -1;
	while (wall_hit(v_x - pixel, v_y, mlx))
	{
		v_x += x_step;
		v_y += y_step;
	}
	mlx->ray->vert_x = v_x;
	mlx->ray->vert_y = v_y;
	return (sqrt(pow(v_x - mlx->player->pixel_x, 2) + \
	pow(v_y - mlx->player->pixel_y, 2)));
}

void cast_rays(t_mlx *mlx) // cast the rays
{
 double h_inter;
 double v_inter;
 int  ray;

 ray = 0;
 mlx->ray->ray_ngl = mlx->player->angle - (mlx->player->fov_rd / 2); // the start angle
 while (ray < SCREEN_WIDTH) // loop for the rays
 {
  mlx->ray->flag = 0; // flag for the wall
  h_inter = get_h_inter(mlx, nor_angle(mlx->ray->ray_ngl)); // get the horizontal intersection
  v_inter = get_v_inter(mlx, nor_angle(mlx->ray->ray_ngl)); // get the vertical intersection
  if (v_inter <= h_inter) // check the distance
   mlx->ray->distance = v_inter; // get the distance
  else
  {
   mlx->ray->distance = h_inter; // get the distance
   mlx->ray->flag = 1; // flag for the wall
  }
  render_wall(mlx, ray); // render the wall
  ray++; // next ray
  mlx->ray->ray_ngl += (mlx->player->fov_rd / SCREEN_WIDTH); // next angle
 }
}
