/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_player_move.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/15 11:58:40 by trstn4        #+#    #+#                 */
/*   Updated: 2023/03/21 17:01:49 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	ft_handle_collectables(t_game *game, int sum, int x, int y)
{
	if (game->ms->map[sum] == 'C')
	{
		game->ms->map[sum] = '0';
		game->player->collected++;
	}
	if (game->ms->map[sum] == 'E' && game->ms->collectables
		== game->player->collected)
	{
		mlx_put_image_to_window(game->vars->mlx, game->vars->win,
			game->images->ground, game->player->x * IMG_SIZE, game->player->y
			* IMG_SIZE);
		mlx_put_image_to_window(game->vars->mlx, game->vars->win,
			game->images->player, (game->player->x + x) * IMG_SIZE,
			(game->player->y + y) * IMG_SIZE);
		mlx_destroy_window(game->vars->mlx, game->vars->win);
		exit(0);
	}
}

void	ft_move_tile(t_game *game, int x, int y)
{
	int		sum;

	sum = (game->player->y + y) * (game->ms->width + 1) + game->player->x + x;
	if (game->ms->map[sum] == '1' || (game->ms->map[sum] == 'E'
			&& game->ms->collectables != game->player->collected))
		return ;
	ft_handle_collectables(game, sum, x, y);
	mlx_put_image_to_window(game->vars->mlx, game->vars->win,
		game->images->ground, (game->player->x + x) * IMG_SIZE,
		(game->player->y + y) * IMG_SIZE);
	mlx_put_image_to_window(game->vars->mlx, game->vars->win,
		game->images->ground, game->player->x * IMG_SIZE, game->player->y
		* IMG_SIZE);
	mlx_put_image_to_window(game->vars->mlx, game->vars->win,
		game->images->player, (game->player->x + x) * IMG_SIZE,
		(game->player->y + y) * IMG_SIZE);
	game->player->x += x;
	game->player->y += y;
	game->player->moves++;
	ft_printf("%i\n", game->player->moves);
}
