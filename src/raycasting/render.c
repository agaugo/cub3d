/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/07 17:30:23 by trstn4        #+#    #+#                 */
/*   Updated: 2024/04/19 12:37:33 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	get_rgba(int r, int g, int b, int a)
{
	return (r << 24 | g << 16 | b << 8 | a << 0);
}

#define RAY_LENGTH 100000

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

void cub_cast_single_ray(t_mlx *mlx, float ray_angle, int ray_num, const float DIST_TO_PROJ_PLANE)
{
    int isRayFacingDown = ray_angle > 0 && ray_angle < M_PI;
    int isRayFacingRight = ray_angle < 0.5 * M_PI || ray_angle > 1.5 * M_PI;

    double horzHitX = 0;
    double horzHitY = 0;
    int foundHorzWallHit = 0;
        
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

    while (nextHorzTouchX >= 0 && nextHorzTouchX <= SCREEN_WIDTH && nextHorzTouchY >= 0 && nextHorzTouchY <= SCREEN_HEIGHT) {
        if (IsWall(mlx, nextHorzTouchX, isRayFacingDown ? nextHorzTouchY : nextHorzTouchY - 1)) {
            horzHitX = nextHorzTouchX;
            horzHitY = nextHorzTouchY;
            foundHorzWallHit = 1;
            break;
        } else {
            nextHorzTouchX += xstep;
            nextHorzTouchY += ystep;
        }
    }

    double vertHitX = 0;
    double vertHitY = 0;
    int foundVertWallHit = 0;
        
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

    while (nextVertTouchX >= 0 && nextVertTouchX <= SCREEN_WIDTH && nextVertTouchY >= 0 && nextVertTouchY <= SCREEN_HEIGHT) {
        if (IsWall(mlx, isRayFacingRight ? nextVertTouchX : nextVertTouchX - 1, nextVertTouchY)) {
            vertHitX = nextVertTouchX;
            vertHitY = nextVertTouchY;
            foundVertWallHit = 1;
            break;
        } else {
            nextVertTouchX += xstep_vert;
            nextVertTouchY += ystep_vert;
        }
    }

    double horzHitDistance = foundHorzWallHit
        ? distance_between_points(mlx->player->pixel_x, mlx->player->pixel_y, horzHitX, horzHitY)
        : INFINITY;
    double vertHitDistance = foundVertWallHit
        ? distance_between_points(mlx->player->pixel_x, mlx->player->pixel_y, vertHitX, vertHitY)
        : INFINITY;

    double perpDistance = (horzHitDistance < vertHitDistance) ? horzHitDistance : vertHitDistance;
    perpDistance *= cos(ray_angle - mlx->player->angle);

    double wallSliceHeight = (perpDistance > 0) ? (mlx->map->pixel_height_per_square / perpDistance) * DIST_TO_PROJ_PLANE : SCREEN_HEIGHT;

    int drawStart = -wallSliceHeight / 2 + SCREEN_HEIGHT / 2;
    drawStart = drawStart < 0 ? 0 : drawStart;
    int drawEnd = wallSliceHeight / 2 + SCREEN_HEIGHT / 2;
    drawEnd = drawEnd >= SCREEN_HEIGHT ? SCREEN_HEIGHT - 1 : drawEnd;

    int wallColor = (horzHitDistance < vertHitDistance)
        ? ((isRayFacingDown) ? 0xFF0000FF : 0x00FF00FF)
        : ((isRayFacingRight) ? 0x0000FFFF : 0xFFFF00FF);

    for (int y = drawStart; y < drawEnd; y++) {
        my_mlx_pixel_put(mlx, ray_num, y, wallColor);
    }
}

void cub_cast_rays(t_mlx *mlx) {
    float ray_angle = mlx->player->angle - (FOV / 2.0 * (M_PI / 180.0));
    float angle_increment = FOV / (float)SCREEN_WIDTH * (M_PI / 180.0);
    const float DIST_TO_PROJ_PLANE = (SCREEN_WIDTH / 2.0) / tan(FOV / 2.0 * (M_PI / 180.0));

    for (int ray_num = 0; ray_num < SCREEN_WIDTH; ray_num++) {
        ray_angle = fmod(ray_angle, 2.0 * M_PI);
        if (ray_angle < 0)
            ray_angle += 2.0 * M_PI;
        cub_cast_single_ray(mlx, ray_angle, ray_num, DIST_TO_PROJ_PLANE);
        ray_angle += angle_increment;
    }
}
