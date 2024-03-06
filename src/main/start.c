/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   start.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 11:53:08 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/06 21:54:04 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3D.h"

#define RAY_LENGTH 100000  // Maximum length of the ray

void my_mlx_pixel_put(t_mlx *mlx, int x, int y, int color) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        mlx_put_pixel(mlx->img, x, y, color);
    }
}

void draw_rays(t_mlx *mlx) {
    float ray_angle = mlx->player->angle - (FOV / 2.0 * (M_PI / 180.0)); // Start at the leftmost angle
    float angle_increment = FOV / (float)SCREEN_WIDTH * (M_PI / 180.0); // Angle increment per pixel

    for (int ray_num = 0; ray_num < SCREEN_WIDTH; ray_num++) {
        float ray_x = mlx->player->pixel_x;
        float ray_y = mlx->player->pixel_y;

        // Normalize the angle
        ray_angle = fmod(ray_angle, 2.0 * M_PI);
        if (ray_angle < 0) {
            ray_angle += 2.0 * M_PI;
        }

        // Step the ray until it hits a wall or reaches the maximum ray length
        for (int i = 0; i < RAY_LENGTH; i++) {
            int map_x = (int)ray_x / pixel_width_per_square;
            int map_y = (int)ray_y / pixel_height_per_square;

            // Check if the ray hit a wall
            if (mlx->map->field[map_y][map_x] == '1') {
                break;
            }

            // Draw the ray
            my_mlx_pixel_put(mlx, (int)ray_x, (int)ray_y, 0xFFFFFF); // White color for the ray

            // Move the ray a little further
            ray_x += cos(ray_angle);
            ray_y += sin(ray_angle);
        }

        // Increment the ray angle for the next pixel
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

void draw_map(t_mlx *mlx) {
    int x, y;
    int color;

    // Draw the map
    for (int map_y = 0; map_y < mlx->map->height; map_y++) {
        for (int map_x = 0; map_x < mlx->map->width; map_x++) {
            char cell = mlx->map->field[map_y][map_x];
            color = (cell == '1') ? 0xFF0000 : (cell == '0') || (cell == 'P') ? 0x0000FF : 0xFFFF00; // Red for '1', blue for '0'
            for (y = 0; y < pixel_height_per_square; y++) {
                for (x = 0; x < pixel_width_per_square; x++) {
                    my_mlx_pixel_put(mlx, map_x * pixel_width_per_square + x, map_y * pixel_height_per_square + y, color);
                }
            }
        }
    }

    // Draw the player as a small square instead of a single pixel
    int player_size = 5; // half the width/height of the player square
    draw_player(mlx, mlx->player->pixel_x, mlx->player->pixel_y, player_size, 0xB5B5B5FF); // Yellow color for the player

	draw_rays(mlx);
}


// void calculate_pixel_size(t_mlx *mlx) {
//     int pixel_width_per_square;
//     int pixel_height_per_square;

//     // Calculate the pixel size of each square
//     pixel_width_per_square = SCREEN_WIDTH / mlx->map->width;
//     pixel_height_per_square = SCREEN_HEIGHT / mlx->map->height;

//     // Print the calculated pixel size for debugging
//     printf("Pixel Width per Square: %d\n", pixel_width_per_square);
//     printf("Pixel Height per Square: %d\n", pixel_height_per_square);
// }

void cub_game_loop(void *ml) // game loop
{
	t_mlx *mlx;

	mlx = ml; // cast to the mlx structure
	mlx_delete_image(mlx->mlx_p, mlx->img); // delete the image
	mlx->img = mlx_new_image(mlx->mlx_p, SCREEN_WIDTH, SCREEN_HEIGHT); // create new image
	// cub_hook(mlx, 0, 0); // hook the player

	draw_map(ml);
	
	mlx_image_to_window(mlx->mlx_p, mlx->img, 0, 0);
}

void cub_start_game(t_map *map) // start the game
{
	t_mlx mlx;

	mlx.map = map; // init the mlx structure
	mlx.player = calloc(1, sizeof(t_player)); // init the player structure i'm using calloc to initialize the variables to zero
	mlx.ray = calloc(1, sizeof(t_ray)); // init the ray structure
	mlx.mlx_p = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D", 0); // init the mlx pointer

	// Calculate player start x and y in pixels.
	// mlx
	// calculate_pixel_size(&mlx);

	mlx_loop_hook(mlx.mlx_p, &cub_game_loop, &mlx); // game loop
	mlx_key_hook(mlx.mlx_p, &key_event, &mlx); // key press and release
	mlx_loop(mlx.mlx_p); // mlx loop
	cub_exit(&mlx); // exit the game
}
