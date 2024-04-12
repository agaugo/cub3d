/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/07 17:30:23 by trstn4        #+#    #+#                 */
/*   Updated: 2024/04/12 14:20:52 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

int	get_rgba(int r, int g, int b, int a)
{
	return (r << 24 | g << 16 | b << 8 | a << 0);
}

#include "../../inc/cub3d.h"

#define RAY_LENGTH 100000  // Maximum length of the ray


void my_mlx_pixel_put(t_mlx *mlx, int x, int y, int color) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        mlx_put_pixel(mlx->img, x, y, color);
    }
}

// Additional function needed to calculate distance between two points
double distance_between_points(double x1, double y1, double x2, double y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void draw_line(t_mlx *mlx, int x0, int y0, int x1, int y1, int color) {
    int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2; // error value e_xy

    for (;;) { // loop
        my_mlx_pixel_put(mlx, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}


int IsWall(t_mlx *mlx, int x, int y) {
    // Convert the x and y pixel coordinates to map grid coordinates
    int mapGridX = x / mlx->map->pixel_width_per_square;
    int mapGridY = y / mlx->map->pixel_height_per_square;

    // Check if the calculated map grid coordinates are out of bounds
    if (mapGridX < 0 || mapGridX >= mlx->map->width || mapGridY < 0 || mapGridY >= mlx->map->height) {
        // Out of bounds of the map is considered as a wall
        return 1;
    }

    // Check the map at the grid coordinates for a wall (assuming '1' represents a wall)
    if (mlx->map->field[mapGridY][mapGridX] == '1') {
        // Wall is present at these map grid coordinates
        return 1;
    }

    // No wall is present
    return 0;
}


void draw_rays(t_mlx *mlx) {
    float ray_angle = mlx->player->angle - (FOV / 2.0 * (M_PI / 180.0));
    float angle_increment = FOV / (float)SCREEN_WIDTH * (M_PI / 180.0);
    const float DIST_TO_PROJ_PLANE = (SCREEN_WIDTH / 2.0) / tan(FOV / 2.0 * (M_PI / 180.0));

    for (int ray_num = 0; ray_num < SCREEN_WIDTH; ray_num++) {
        int wallHit = 0;
        // int wallColor = 0xFFFFFF;

        // Normalize the angle
        ray_angle = fmod(ray_angle, 2.0 * M_PI);
        if (ray_angle < 0) ray_angle += 2.0 * M_PI;

        // Check if ray is facing up or down, left or right
        int isRayFacingDown = ray_angle > 0 && ray_angle < M_PI;
        int isRayFacingRight = ray_angle < 0.5 * M_PI || ray_angle > 1.5 * M_PI;

        // Horizontal grid intersection check
        double horzHitX = 0;
        double horzHitY = 0;
        int foundHorzWallHit = 0;
        
        // Calculate horizontal distance to the first intersection...
        double yintercept = floor(mlx->player->pixel_y / mlx->map->pixel_height_per_square) * mlx->map->pixel_height_per_square;
        yintercept += isRayFacingDown ? mlx->map->pixel_height_per_square : 0;
        double xintercept = mlx->player->pixel_x + (yintercept - mlx->player->pixel_y) / tan(ray_angle);
        
        // Calculate the increments for each step
        double ystep = mlx->map->pixel_height_per_square;
        ystep *= isRayFacingDown ? 1 : -1;
        
        double xstep = mlx->map->pixel_height_per_square / tan(ray_angle);
        xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;
        xstep *= (!isRayFacingRight && xstep > 0) ? -1 : 1;
        
        double nextHorzTouchX = xintercept;
        double nextHorzTouchY = yintercept;

        // Perform horizontal checks
        while (nextHorzTouchX >= 0 && nextHorzTouchX <= SCREEN_WIDTH && nextHorzTouchY >= 0 && nextHorzTouchY <= SCREEN_HEIGHT) {
            // Check for wall hit
            if (IsWall(mlx, nextHorzTouchX, isRayFacingDown ? nextHorzTouchY : nextHorzTouchY - 1)) {
                // Found a wall hit
                horzHitX = nextHorzTouchX;
                horzHitY = nextHorzTouchY;
                foundHorzWallHit = 1;
                break;
            } else {
                nextHorzTouchX += xstep;
                nextHorzTouchY += ystep;
            }
        }

        // Vertical grid intersection check
        double vertHitX = 0;
        double vertHitY = 0;
        int foundVertWallHit = 0;
        
        // Calculate vertical distance to the first intersection...
        double xintercept_vert = floor(mlx->player->pixel_x / mlx->map->pixel_width_per_square) * mlx->map->pixel_width_per_square;
        xintercept_vert += isRayFacingRight ? mlx->map->pixel_width_per_square : 0;
        double yintercept_vert = mlx->player->pixel_y + (xintercept_vert - mlx->player->pixel_x) * tan(ray_angle);
        
        // Calculate the increments for each step
        double xstep_vert = mlx->map->pixel_width_per_square;
        xstep_vert *= isRayFacingRight ? 1 : -1;
        
        double ystep_vert = mlx->map->pixel_width_per_square * tan(ray_angle);
        ystep_vert *= (isRayFacingDown && ystep_vert < 0) ? -1 : 1;
        ystep_vert *= (!isRayFacingDown && ystep_vert > 0) ? -1 : 1;

        double nextVertTouchX = xintercept_vert;
        double nextVertTouchY = yintercept_vert;

        // Perform vertical checks
        while (nextVertTouchX >= 0 && nextVertTouchX <= SCREEN_WIDTH && nextVertTouchY >= 0 && nextVertTouchY <= SCREEN_HEIGHT) {
            // Check for wall hit
            if (IsWall(mlx, isRayFacingRight ? nextVertTouchX : nextVertTouchX - 1, nextVertTouchY)) {
                // Found a wall hit
                vertHitX = nextVertTouchX;
                vertHitY = nextVertTouchY;
                foundVertWallHit = 1;
                break;
            } else {
                nextVertTouchX += xstep_vert;
                nextVertTouchY += ystep_vert;
            }
        }

        // Calculate distances to the horizontal and vertical wall hit
        double horzHitDistance = foundHorzWallHit
            ? distance_between_points(mlx->player->pixel_x, mlx->player->pixel_y, horzHitX, horzHitY)
            : INFINITY;
        double vertHitDistance = foundVertWallHit
            ? distance_between_points(mlx->player->pixel_x, mlx->player->pixel_y, vertHitX, vertHitY)
            : INFINITY;

        // Determine which distance is shorter
        double rayDistance = (horzHitDistance < vertHitDistance) ? horzHitDistance : vertHitDistance;
        // wallColor = (horzHitDistance < vertHitDistance)
        //     ? ((isRayFacingDown) ? 0xFF0000FF : 0x00FF00FF) // Horizontal - North or South
        //     : ((isRayFacingRight) ? 0x0000FFFF : 0xFFFF00FF); // Vertical - East or West

        // After you calculate the distance to the wall (horzHitDistance and vertHitDistance)
        double perpDistance = (horzHitDistance < vertHitDistance) ? horzHitDistance : vertHitDistance;
        perpDistance *= cos(ray_angle - mlx->player->angle); // Correct fish-eye effect

        // Determine the height of the wall slice
        double wallSliceHeight = (perpDistance > 0) ? (mlx->map->pixel_height_per_square / perpDistance) * DIST_TO_PROJ_PLANE : SCREEN_HEIGHT;

        // Calculate the top and bottom positions for the wall slice
        int drawStart = -wallSliceHeight / 2 + SCREEN_HEIGHT / 2;
        drawStart = drawStart < 0 ? 0 : drawStart;
        int drawEnd = wallSliceHeight / 2 + SCREEN_HEIGHT / 2;
        drawEnd = drawEnd >= SCREEN_HEIGHT ? SCREEN_HEIGHT - 1 : drawEnd;

        // Determine wallColor based on the side hit
        int wallColor = (horzHitDistance < vertHitDistance)
            ? ((isRayFacingDown) ? 0xFF0000FF : 0x00FF00FF) // Horizontal - North or South
            : ((isRayFacingRight) ? 0x0000FFFF : 0xFFFF00FF); // Vertical - East or West

        // Draw vertical column for the wall slice
        for(int y = drawStart; y < drawEnd; y++) {
            my_mlx_pixel_put(mlx, ray_num, y, wallColor);
        }

        // Increment the ray angle for the next column
        ray_angle += angle_increment;

    }
}


// Function to draw a small square representing the player
void draw_player(t_mlx *mlx, int player_x, int player_y, int size, int color) {
    for (int y = -size; y <= size; y++) {
        for (int x = -size; x <= size; x++) {
            my_mlx_pixel_put(mlx, player_x + x, player_y + y, color);
        }
    }
}

// void draw_map(t_mlx *mlx) {
//     int x, y;
//     int color;

//     // // Draw the map
//     // for (int map_y = 0; map_y < mlx->map->height; map_y++) {
//     //     for (int map_x = 0; map_x < mlx->map->width; map_x++) {
//     //         char cell = mlx->map->field[map_y][map_x];
//     //         color = (cell == '1') ? 0xFF0000 : (cell == '0') || (cell == 'P') ? 0x0000FF : 0xFFFF00; // Red for '1', blue for '0'
//     //         for (y = 0; y < mlx->map->pixel_height_per_square; y++) {
//     //             for (x = 0; x < mlx->map->pixel_width_per_square; x++) {
//     //                 my_mlx_pixel_put(mlx, map_x * mlx->map->pixel_width_per_square + x, map_y * pixel_height_per_square + y, color);
//     //             }
//     //         }
//     //     }
//     // }

//     // // Draw the player as a small square instead of a single pixel
//     // int player_size = 5; // half the width/height of the player square
//     // draw_player(mlx, mlx->player->pixel_x, mlx->player->pixel_y, player_size, 0xB5B5B5FF); // Yellow color for the player

// 	draw_rays(mlx);
// }


// // void calculate_pixel_size(t_mlx *mlx) {
// //     int mlx->map->pixel_width_per_square;
// //     int mlx->map->pixel_height_per_square;

// //     // Calculate the pixel size of each square
// //     mlx->map->pixel_width_per_square = SCREEN_WIDTH / mlx->map->width;
// //     pixel_height_per_square = SCREEN_HEIGHT / mlx->map->height;

// //     // Print the calculated pixel size for debugging
// //     printf("Pixel Width per Square: %d\n", mlx->map->pixel_width_per_square);
// //     printf("Pixel Height per Square: %d\n", pixel_height_per_square);
// // }

// void cub_game_loop(void *ml) // game loop
// {
// 	t_mlx *mlx;

// 	mlx = ml; // cast to the mlx structure
// 	mlx_delete_image(mlx->mlx_p, mlx->img); // delete the image
// 	mlx->img = mlx_new_image(mlx->mlx_p, SCREEN_WIDTH, SCREEN_HEIGHT); // create new image
// 	// cub_hook(mlx, 0, 0); // hook the player

// 	draw_map(ml);
	
// 	mlx_image_to_window(mlx->mlx_p, mlx->img, 0, 0);
// }

// void cub_start_game(t_map *map) // start the game
// {
// 	t_mlx mlx;

// 	mlx.map = map; // init the mlx structure
// 	mlx.player = calloc(1, sizeof(t_player)); // init the player structure i'm using calloc to initialize the variables to zero
// 	mlx.ray = calloc(1, sizeof(t_ray)); // init the ray structure
// 	mlx.mlx_p = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D", 0); // init the mlx pointer

// 	mlx.player->pixel_x = 156;
// 	mlx.player->pixel_y = 212;

// 	mlx_loop_hook(mlx.mlx_p, &cub_game_loop, &mlx); // game loop
// 	mlx_key_hook(mlx.mlx_p, &key_event, &mlx); // key press and release
// 	mlx_loop(mlx.mlx_p); // mlx loop
// 	cub_exit(&mlx); // exit the game
// }
