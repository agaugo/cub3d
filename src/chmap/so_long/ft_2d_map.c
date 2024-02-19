/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_2d_map.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/20 14:11:43 by trstn4        #+#    #+#                 */
/*   Updated: 2023/03/21 16:57:28 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	ft_set_2d_map(char *map, char **map_2d, int height, int width)
{
	int	i;
	int	j;
	int	index;

	i = 0;
	j = 0;
	index = 0;
	while (j < height)
	{
		while (i < width)
		{
			map_2d[j][i] = map[index];
			i++;
			index++;
		}
		j++;
		i = 0;
		index++;
	}
}

void	ft_free_2d_map(char **map, int height)
{
	int	i;

	i = 0;
	while (i < height)
	{
		free(map[i]);
		i++;
	}
	free(map);
}

char	**ft_make_2d_map(t_game *game)
{
	char	**map;
	int		i;

	map = malloc(game->ms->height * sizeof(char *));
	if (!map)
		return (NULL);
	i = 0;
	while (i < game->ms->height)
	{
		map[i] = malloc(game->ms->width * sizeof(char));
		if (!map[i])
			return (NULL);
		i++;
	}
	ft_set_2d_map(game->ms->map, map, game->ms->height, game->ms->width);
	return (map);
}
