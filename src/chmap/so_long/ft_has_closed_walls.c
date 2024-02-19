/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_has_closed_walls.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/20 15:21:23 by trstn4        #+#    #+#                 */
/*   Updated: 2023/03/21 16:40:36 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	ft_has_top_and_bottom_walls(t_game *game, char	**map)
{
	int	i;

	i = 0;
	while (i < game->ms->width)
	{
		if (map[0][i] != '1' || map[game->ms->height - 1][i] != '1')
		{
			ft_free_2d_map(map, game->ms->height);
			ft_printf("Error\nThe map must be closed/surrounded by walls.\n");
			exit(0);
		}
		i++;
	}
}

static void	ft_has_side_walls(t_game *game, char	**map)
{
	int	i;

	i = 0;
	while (i < game->ms->height)
	{
		if (map[i][0] != '1' || map[i][game->ms->width - 1] != '1')
		{
			ft_free_2d_map(map, game->ms->height);
			ft_printf("Error\nThe map must be closed/surrounded by walls.\n");
			exit(0);
		}
		i++;
	}
}

void	ft_has_closed_walls(t_game *game)
{
	char	**map;

	map = ft_make_2d_map(game);
	ft_has_top_and_bottom_walls(game, map);
	ft_has_side_walls(game, map);
	ft_free_2d_map(map, game->ms->height);
}
