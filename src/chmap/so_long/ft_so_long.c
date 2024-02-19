/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_so_long.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/24 16:50:55 by trstn4        #+#    #+#                 */
/*   Updated: 2023/03/21 17:06:03 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	ft_exit(t_game *game)
{
	mlx_destroy_window(game->vars->mlx, game->vars->win);
	exit(0);
}

static int	ft_check_for_input(int keycode, t_game *game)
{
	if (keycode == 13)
		ft_move_tile(game, 0, -1);
	if (keycode == 1)
		ft_move_tile(game, 0, 1);
	if (keycode == 0)
		ft_move_tile(game, -1, 0);
	if (keycode == 2)
		ft_move_tile(game, 1, 0);
	if (keycode == 53)
	{
		mlx_destroy_window(game->vars->mlx, game->vars->win);
		exit(0);
	}
	return (0);
}

static void	ft_start_game(t_game game)
{
	game.vars->mlx = mlx_init();
	game.vars->win = mlx_new_window(game.vars->mlx, (game.ms->width * IMG_SIZE),
			(game.ms->height * IMG_SIZE), "so_long");
	ft_put_images(&game);
	ft_has_a_valid_path(&game, game.player->x, game.player->y);
	mlx_hook(game.vars->win, 17, 1L << 2, ft_exit, &game);
	mlx_hook(game.vars->win, 2, 1L << 0, ft_check_for_input, &game);
	mlx_loop(game.vars->mlx);
}

int	main(int argc, char *argv[])
{
	t_game		game;
	t_vars		vars;
	t_mapstats	ms;
	t_player	player;
	t_images	images;

	game.player = &player;
	game.vars = &vars;
	game.ms = &ms;
	game.images = &images;
	game.player->moves = 0;
	game.player->collected = 0;
	game.ms->collectables = 0;
	if (argc == 2)
	{
		ft_load_map(&game, argv[1]);
		ft_check_if_map_valid(&game);
		ft_start_game(game);
	}
	else if (argc > 2)
		ft_printf("%s", "Error\nToo many arguments supplied.\n");
	else
		ft_printf("%s", "Error\nOne argument expected.\n");
	return (0);
}
