/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 11:53:17 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/13 22:43:52 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

// Checks if a character is allowed
bool is_allowed_char(char c) {
    return c == '0' || c == '1' || c == 'N' || c == 'E' || c == 'W' || c == 'S' || c == '\n' || c == '\0';
}

// Removes white spaces and checks characters
void cub_check_characters(t_map *map) {
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; map->field[i][j] != '\0'; j++) {
            // Skipping whitespace characters
            if (map->field[i][j] == ' ' || map->field[i][j] == '\t') {
                continue;
            }

            if (!is_allowed_char(map->field[i][j])) {
                printf("Error\nCharacter: '%c' is not allowed.\n", map->field[i][j]);
                exit(1);
            }
        }
    }
}

void cub_set_player_location(t_mlx *mlx, int *count, int x, int y, char direction)
{
	*count += 1;
	mlx->map->player_start_y = y;
	mlx->map->player_start_x = x;
	mlx->player->direction = direction;
}

void cub_check_for_player(t_mlx *mlx) {
    int count;

	count = 0;
    for (int y = 0; y < mlx->map->height; y++)
	{
        for (int x = 0; mlx->map->field[y][x] != '\0'; x++)
		{
            if (mlx->map->field[y][x] == 'N')
				cub_set_player_location(mlx, &count, x, y, 'N');
            if (mlx->map->field[y][x] == 'E')
				cub_set_player_location(mlx, &count, x, y, 'E');
            if (mlx->map->field[y][x] == 'S')
				cub_set_player_location(mlx, &count, x, y, 'S');
            if (mlx->map->field[y][x] == 'W')
				cub_set_player_location(mlx, &count, x, y, 'W');
        }
    }
    if (count == 0) {
        printf("Error\nMissing player start position.\n");
		exit(1);
    } else if (count > 1) {
        printf("Error\nMultiple player start positions detected.\n");
		exit(1);
    }
}

void	cub_game_loop(void *parsed_mlx)
{
	t_mlx *mlx;

	mlx = parsed_mlx;
	mlx_delete_image(mlx->mlx_p, mlx->img);
	mlx->img = mlx_new_image(mlx->mlx_p, SCREEN_WIDTH, SCREEN_HEIGHT);
	cub_player_update_frame(mlx, 0, 0);
	cub_cast_rays(mlx);
	mlx_image_to_window(mlx->mlx_p, mlx->img, 0, 0);
}

void	cub_start_game(t_map *map)
{
	t_mlx mlx;

	mlx.map = map;
	mlx.player = ft_calloc(1, sizeof(t_player));
	mlx.key = ft_calloc(1, sizeof(t_key));
	mlx.mlx_p = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D", 0);
	cub_check_for_player(&mlx);
	cub_set_player_start_position(&mlx);
	mlx_loop_hook(mlx.mlx_p, &cub_game_loop, &mlx);
	mlx_key_hook(mlx.mlx_p, &cub_check_key, &mlx);
	mlx_loop(mlx.mlx_p);
	cub_exit(&mlx);
}

void v()
{
	system("leaks cub3D");
}

int main(int argc, char **argv) {
	t_map *map;

	// atexit(v); // Remove this before eval
	if (argc != 2) {
		printf("Usage:%s <map_file>\n", argv[0]);
		return (1);
	}
	map = cub_load_map_values(argv[1]);
	cub_validate_map(map, argv[1]);
	cub_check_characters(map);
	cub_start_game(map);
	return (0);
}
