/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: hflohil- <hflohil-@codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 17:16:15 by hugo          #+#    #+#                 */
/*   Updated: 2024/02/28 16:57:43 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// Written by Bruh

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../../MLX42/include/MLX42/MLX42.h"

#define WIDTH 256
#define HEIGHT 256

#include <string.h>

#define BLOCK_SIZE 10

void draw_map(mlx_t *mlx, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    int y = 0;
    mlx_image_t* img = mlx_new_image(mlx, WIDTH, HEIGHT);

    while (fgets(line, sizeof(line), file)) {
        for (int x = 0; line[x] && x < WIDTH / BLOCK_SIZE; x++) {
            if (line[x] == '1') {
                // Draw a wall block
                for (int dy = 0; dy < BLOCK_SIZE; dy++) {
                    for (int dx = 0; dx < BLOCK_SIZE; dx++) {
                        mlx_put_pixel(img, x * BLOCK_SIZE + dx, y * BLOCK_SIZE + dy, 0xFF0000FF); // Wall color
                    }
                }
            }
        }
        y++;
    }

    fclose(file);

    // if (mlx_image_to_window(mlx, img, 0, 0) < 0)
    //     ft_error();
}

// Exit the program as failure.
static void ft_error(void)
{
	fprintf(stderr, "%s", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

// Print the window width and height.
static void ft_hook(void* param)
{
	const mlx_t* mlx = param;

	printf("WIDTH: %d | HEIGHT: %d\n", mlx->width, mlx->height);
}

int32_t	main(void)
{
	// MLX allows you to define its core behaviour before startup.
	mlx_set_setting(MLX_MAXIMIZED, true);
	mlx_t* mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
	if (!mlx)
		ft_error();

	/* Do stuff */

    draw_map(mlx, "maps/test.cub");

	// Register a hook and pass mlx as an optional param.
	// NOTE: Do this before calling mlx_loop!
	mlx_loop_hook(mlx, ft_hook, mlx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
