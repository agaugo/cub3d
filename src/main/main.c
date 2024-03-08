/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 11:53:17 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/08 19:57:00 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/cub3D.h"

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
	mlx.mlx_p = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D", 0);
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

	atexit(v); // Remove this before eval
	if (argc != 2) {
		printf("Usage: %s <map_file>\n", argv[0]);
		return (1);
	}
	cub_validate_map(argv[1]);
	map = cub_load_map_values(argv[1]);
	cub_start_game(map);
	return (0);
}
