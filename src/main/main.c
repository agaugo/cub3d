#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "../../MLX42/include/MLX42/MLX42.h"

#define WIDTH 256
#define HEIGHT 256
#define BLOCK_SIZE 10
#define MAP_WIDTH 24
#define MAP_HEIGHT 24

#define WIDTH 256
#define HEIGHT 256
#define SPEED 0.05
#define ROT_SPEED 0.05

typedef struct s_player {
    double posX, posY; // Player's position
    double dirX, dirY; // Player's direction vector
    double planeX, planeY; // Camera plane, perpendicular to direction
} t_player;

t_player player = {12.0, 12.0, -1.0, 0.0, 0.0, 0.66};

void key_event(mlx_key_data_t keydata, void* param) {
    t_player* p = param;
            if (keydata.key == MLX_KEY_W) { // Move forward
                p->posX += p->dirX * SPEED;
                p->posY += p->dirY * SPEED;
            }
            if (keydata.key == MLX_KEY_S) { // Move backward
                p->posX -= p->dirX * SPEED;
                p->posY -= p->dirY * SPEED;
            }
            if (keydata.key == MLX_KEY_A) { // Strafe left
                p->posX -= p->planeX * SPEED;
                p->posY -= p->planeY * SPEED;
            }
            if (keydata.key == MLX_KEY_D) { // Strafe right
                p->posX += p->planeX * SPEED;
                p->posY += p->planeY * SPEED;
            }
            if (keydata.key == MLX_KEY_LEFT) { // Rotate left
                double oldDirX = p->dirX;
                p->dirX = p->dirX * cos(ROT_SPEED) - p->dirY * sin(ROT_SPEED);
                p->dirY = oldDirX * sin(ROT_SPEED) + p->dirY * cos(ROT_SPEED);
                double oldPlaneX = p->planeX;
                p->planeX = p->planeX * cos(ROT_SPEED) - p->planeY * sin(ROT_SPEED);
                p->planeY = oldPlaneX * sin(ROT_SPEED) + p->planeY * cos(ROT_SPEED);
            }
            if (keydata.key == MLX_KEY_RIGHT) { // Rotate right
                double oldDirX = p->dirX;
                p->dirX = p->dirX * cos(-ROT_SPEED) - p->dirY * sin(-ROT_SPEED);
                p->dirY = oldDirX * sin(-ROT_SPEED) + p->dirY * cos(-ROT_SPEED);
                double oldPlaneX = p->planeX;
                p->planeX = p->planeX * cos(-ROT_SPEED) - p->planeY * sin(-ROT_SPEED);
                p->planeY = oldPlaneX * sin(-ROT_SPEED) + p->planeY * cos(-ROT_SPEED);
            }

    printf("Position: (%.2f, %.2f), Direction: (%.2f, %.2f), Plane: (%.2f, %.2f)\n", p->posX, p->posY, p->dirX, p->dirY, p->planeX, p->planeY);
}

int worldMap[MAP_WIDTH][MAP_HEIGHT] = {
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
  // Add more rows with 0 for open space and 1 for walls
  {1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

void draw_map(mlx_t *mlx) {
    mlx_image_t* img = mlx_new_image(mlx, WIDTH, HEIGHT);
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (worldMap[x][y] == 1) {
                // Draw a wall block
                for (int dy = 0; dy < BLOCK_SIZE; dy++) {
                    for (int dx = 0; dx < BLOCK_SIZE; dx++) {
                        mlx_put_pixel(img, x * BLOCK_SIZE + dx, y * BLOCK_SIZE + dy, 0xFF0000FF); // Wall color
                    }
                }
            }
        }
    }
    mlx_image_to_window(mlx, img, 0, 0);
}

void cast_ray(mlx_t *mlx) {
    // Assuming a simplistic ray casting directly to the right
    int mapX = 22; // Starting X position of the ray
    int mapY = 12; // Starting Y position of the ray
    int hit = 0; // Was a wall hit?
    // float dirX = 1; // Direction of the ray
    // float dirY = 0;

    // Draw a line from the player position to the right until a wall is hit
    mlx_image_t* img = mlx_new_image(mlx, WIDTH, HEIGHT);
    while (!hit) {
        if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
            break; // Break out of the loop if out of map bounds
        }
        if (worldMap[mapX][mapY] > 0) {
            hit = 1; // A wall was hit, draw the hit marker here
            for (int y = 0; y < HEIGHT; y++) {
                mlx_put_pixel(img, mapX * BLOCK_SIZE, y, 0xFFFFFFFF); // White color for the ray hit
            }
            break; // Stop the loop once a wall is hit
        }
        mapX += 1; // Move the ray right (simplistic horizontal movement)
    }

    if (hit) {
        mlx_image_to_window(mlx, img, 0, 0); // Display the image only if a hit was detected
    } else {
        mlx_delete_image(mlx, img); // Destroy the image if no hit to avoid memory leak
    }
}

void ft_error() {
    printf("Error\n");
    exit(1);
}

int main(void) {
    mlx_set_setting(MLX_MAXIMIZED, true);
    mlx_t* mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
    if (!mlx)
        ft_error();

    // draw_map(mlx);
    // cast_ray(mlx); // Add this line to cast a ray

    mlx_key_hook(mlx, key_event, &player);

    mlx_loop(mlx);
    mlx_terminate(mlx);
    return (EXIT_SUCCESS);
}
