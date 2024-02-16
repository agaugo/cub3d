/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_has_a_valid_path.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/14 11:29:32 by trstn4        #+#    #+#                 */
/*   Updated: 2023/03/21 16:40:51 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	ft_reached_all_characters(t_game *game, char **map, int i, int j)
{
	if (map[j][i] == 'C')
	{
		ft_printf("Error\nThe given map does not have a valid path.\n");
		ft_free_2d_map(map, game->ms->height);
		ft_exit(game);
	}
	if (map[j][i] == 'E')
	{
		if (map[j + 1][i] != 'V' && map[j - 1][i] != 'V' && map[j][i + 1] != 'V'
			&& map[j][i - 1] != 'V')
		{
			ft_printf("Error\nThe given map does not have a valid path.\n");
			ft_free_2d_map(map, game->ms->height);
			ft_exit(game);
		}
	}
}

static void	ft_floodfill(t_game *game, char **map, int x, int y)
{
	if (x < 0 || x >= game->ms->width || y < 0 || y >= game->ms->height
		|| map[y][x] == 'V' || map[y][x] == '1')
		return ;
	if (map[y][x] == 'E')
		return ;
	map[y][x] = 'V';
	ft_floodfill(game, map, x - 1, y);
	ft_floodfill(game, map, x + 1, y);
	ft_floodfill(game, map, x, y - 1);
	ft_floodfill(game, map, x, y + 1);
}

void	ft_has_a_valid_path(t_game *game, int player_x, int player_y)
{
	int		i;
	int		j;
	char	**map;

	map = ft_make_2d_map(game);
	ft_floodfill(game, map, player_x, player_y);
	j = 0;
	while (j < game->ms->height)
	{
		i = 0;
		while (i < game->ms->width)
		{
			ft_reached_all_characters(game, map, i, j);
			i++;
		}
		j++;
	}
	ft_free_2d_map(map, game->ms->height);
}
