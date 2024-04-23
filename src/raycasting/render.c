/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/07 17:30:23 by trstn4        #+#    #+#                 */
/*   Updated: 2024/04/23 14:59:33 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

#define RAY_LENGTH 100000

int reverse_bytes(int color) {
	return ((color & 0xFF) << 24) |         // Move the first byte to the last
		   ((color & 0xFF00) << 8) |        // Move the second byte to the third
		   ((color & 0xFF0000) >> 8) |      // Move the third byte to the second
		   ((color & 0xFF000000) >> 24);    // Move the last byte to the first
}

void my_mlx_pixel_put(t_mlx *mlx, int x, int y, int color) {
	if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
		mlx_put_pixel(mlx->img, x, y, color);
	}
}

double distance_between_points(double x1, double y1, double x2, double y2) {
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void draw_line(t_mlx *mlx, int x0, int y0, int x1, int y1, int color) {
	int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2;

	for (;;) {
		my_mlx_pixel_put(mlx, x0, y0, color);
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if (e2 >= dy) { err += dy; x0 += sx; }
		if (e2 <= dx) { err += dx; y0 += sy; }
	}
}


int IsWall(t_mlx *mlx, int x, int y) {
	int mapGridX = x / mlx->map->pixel_width_per_square;
	int mapGridY = y / mlx->map->pixel_height_per_square;

	if (mapGridX < 0 || mapGridX >= mlx->map->width || mapGridY < 0 || mapGridY >= mlx->map->height) {
		return 1;
	}

	if (mlx->map->field[mapGridY][mapGridX] == '1') {
		return 1;
	}

	return 0;
}

mlx_texture_t* cub_select_texture(t_mlx *mlx, double horzHitDistance, double vertHitDistance, int isRayFacingDown, int isRayFacingRight) {
	if (horzHitDistance < vertHitDistance) {
		return isRayFacingDown ? mlx->map->south_texture : mlx->map->north_texture;
	} else {
		return isRayFacingRight ? mlx->map->east_texture : mlx->map->west_texture;
	}
}

void cub_draw_wall(t_mlx *mlx, int x, int drawStart, int drawEnd, mlx_texture_t *texture, double wallHitPos, double wallSliceHeight) {
	int texture_x = (int)(fmod(wallHitPos, TILE_SIZE) / TILE_SIZE * texture->width);
	double textureStep = texture->height / wallSliceHeight;
	double texturePos = (drawStart - SCREEN_HEIGHT / 2 + wallSliceHeight / 2) * textureStep;
	int	y;
	
	y = drawStart;
	while (y < drawEnd)
	{
		int texY = (int)texturePos % texture->height;
		my_mlx_pixel_put(mlx, x, y, reverse_bytes(((uint32_t*)texture->pixels)[texY * texture->width + texture_x]));
		texturePos += textureStep;
		y++;
	}
}

void cub_draw(t_mlx *mlx, int ray_num, int drawStart, int drawEnd, mlx_texture_t *texture, double wallHitPos, double wallSliceHeight)
{
	int	y;

	cub_draw_wall(mlx, ray_num, drawStart, drawEnd, texture, wallHitPos, wallSliceHeight);
	y = 0;
	while (y < drawStart)
	{
		my_mlx_pixel_put(mlx, ray_num, y, mlx->map->color_ceiling);
		y++;
	}
	y = drawEnd;
	while (y < SCREEN_HEIGHT)
	{
		my_mlx_pixel_put(mlx, ray_num, y, mlx->map->color_floor);
		y++;
	}
}

double tt(t_mlx *mlx, int isRayFacingDown, int isRayFacingRight, float ray_angle, double *horzHitX, double *horzHitY)
{
	double yintercept = floor(mlx->player->pixel_y / mlx->map->pixel_height_per_square) * mlx->map->pixel_height_per_square;
	yintercept += isRayFacingDown ? mlx->map->pixel_height_per_square : 0;
	double xintercept = mlx->player->pixel_x + (yintercept - mlx->player->pixel_y) / tan(ray_angle);
		
	double ystep = mlx->map->pixel_height_per_square;
	ystep *= isRayFacingDown ? 1 : -1;
		
	double xstep = mlx->map->pixel_height_per_square / tan(ray_angle);
	xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;
	xstep *= (!isRayFacingRight && xstep > 0) ? -1 : 1;
		
	double nextHorzTouchX = xintercept;
	double nextHorzTouchY = yintercept;
	
	int foundHorzWallHit = 0;

	while (nextHorzTouchX >= 0 && nextHorzTouchX < mlx->map->width * mlx->map->pixel_width_per_square &&
		   nextHorzTouchY >= 0 && nextHorzTouchY < mlx->map->height * mlx->map->pixel_height_per_square) {
		if (IsWall(mlx, nextHorzTouchX, nextHorzTouchY - (isRayFacingDown ? 0 : 1))) {
			*horzHitX = nextHorzTouchX;
			*horzHitY = nextHorzTouchY;
			foundHorzWallHit = 1;
			break;
		} else {
			nextHorzTouchX += xstep;
			nextHorzTouchY += ystep;
		}
	}
	double horzHitDistance = foundHorzWallHit
		? distance_between_points(mlx->player->pixel_x, mlx->player->pixel_y, *horzHitX, *horzHitY)
		: INFINITY;
		
	return horzHitDistance;
}

void cub_cast_single_ray(t_mlx *mlx, float ray_angle, int ray_num, const float distance_to_proj_plane)
{
	int isRayFacingDown = ray_angle > 0 && ray_angle < M_PI;
	int isRayFacingRight = ray_angle < 0.5 * M_PI || ray_angle > 1.5 * M_PI;

	double horzHitX = 0;
	double horzHitY = 0;
	double vertHitX = 0;
	double vertHitY = 0;
	
	int foundVertWallHit = 0;

	double horzHitDistance = tt(mlx, isRayFacingDown, isRayFacingRight, ray_angle, &horzHitX, &horzHitY);
		
	double xintercept_vert = floor(mlx->player->pixel_x / mlx->map->pixel_width_per_square) * mlx->map->pixel_width_per_square;
	xintercept_vert += isRayFacingRight ? mlx->map->pixel_width_per_square : 0;
	double yintercept_vert = mlx->player->pixel_y + (xintercept_vert - mlx->player->pixel_x) * tan(ray_angle);
		
	double xstep_vert = mlx->map->pixel_width_per_square;
	xstep_vert *= isRayFacingRight ? 1 : -1;
		
	double ystep_vert = mlx->map->pixel_width_per_square * tan(ray_angle);
	ystep_vert *= (isRayFacingDown && ystep_vert < 0) ? -1 : 1;
	ystep_vert *= (!isRayFacingDown && ystep_vert > 0) ? -1 : 1;

	double nextVertTouchX = xintercept_vert;
	double nextVertTouchY = yintercept_vert;

	while (nextVertTouchX >= 0 && nextVertTouchX < mlx->map->width * mlx->map->pixel_width_per_square &&
		   nextVertTouchY >= 0 && nextVertTouchY < mlx->map->height * mlx->map->pixel_height_per_square) {
		if (IsWall(mlx, nextVertTouchX - (isRayFacingRight ? 0 : 1), nextVertTouchY)) {
			vertHitX = nextVertTouchX;
			vertHitY = nextVertTouchY;
			foundVertWallHit = 1;
			break;
		} else {
			nextVertTouchX += xstep_vert;
			nextVertTouchY += ystep_vert;
		}
	}
	double vertHitDistance = foundVertWallHit
		? distance_between_points(mlx->player->pixel_x, mlx->player->pixel_y, vertHitX, vertHitY)
		: INFINITY;

	int hitVertical = (vertHitDistance < horzHitDistance);

	mlx_texture_t *texture = cub_select_texture(mlx, horzHitDistance, vertHitDistance, isRayFacingDown, isRayFacingRight);

	double perpDistance = min(horzHitDistance, vertHitDistance) * cos(ray_angle - mlx->player->angle);
	double wallSliceHeight = (perpDistance > 0) ? (TILE_SIZE / perpDistance) * distance_to_proj_plane : SCREEN_HEIGHT;
	int drawStart = max(0, SCREEN_HEIGHT / 2 - (int)(wallSliceHeight / 2));
	int drawEnd = min(SCREEN_HEIGHT - 1, SCREEN_HEIGHT / 2 + (int)(wallSliceHeight / 2));

	double wallHitPos = hitVertical ? vertHitY : horzHitX;
	
	cub_draw(mlx, ray_num, drawStart, drawEnd, texture, wallHitPos, wallSliceHeight);
}

void	cub_cast_rays(t_mlx *mlx)
{
	float	ray_angle;
	float	angle_increment;
	float	distance_to_proj_plane;
	int		ray_num;
	
	ray_angle = mlx->player->angle - (FOV / 2.0 * (M_PI / 180.0));
	angle_increment = FOV / (float)SCREEN_WIDTH * (M_PI / 180.0);
	distance_to_proj_plane = (SCREEN_WIDTH / 2.0) / tan(FOV / 2.0 * (M_PI / 180.0));
	ray_num = 0;
	while (ray_num < SCREEN_WIDTH) {
		ray_angle = fmod(ray_angle, 2.0 * M_PI);
		if (ray_angle < 0)
			ray_angle += 2.0 * M_PI;
		cub_cast_single_ray(mlx, ray_angle, ray_num, distance_to_proj_plane);
		ray_angle += angle_increment;
		ray_num++;
	}
}
