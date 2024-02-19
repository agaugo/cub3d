/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_check_if_map_valid.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/21 16:23:08 by trstn4        #+#    #+#                 */
/*   Updated: 2023/03/21 19:58:33 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	free_split_memory(char **sub_array, int i)
{
	while (i-- > 0)
		free(sub_array[i]);
	free(sub_array);
}

static void	ft_is_rectangle(t_game *game)
{
	int		i;
	char	**lined;

	i = 0;
	lined = ft_split(game->ms->map, '\n');
	if (!lined)
		return ;
	while (i < game->ms->height)
	{
		if ((int)ft_strlen(lined[i]) != game->ms->width)
		{
			ft_printf("Error\nThe map must be rectangular.\n");
			free_split_memory(lined, i);
			exit(0);
		}
		i++;
	}
	free_split_memory(lined, game->ms->height);
}

static void	ft_has_right_characters(t_game *game)
{
	int	i;

	i = 0;
	while (game->ms->map[i])
	{
		if (game->ms->map[i] != '1' && game->ms->map[i] != '0'
			&& game->ms->map[i] != 'C' && game->ms->map[i] != 'P'
			&& game->ms->map[i] != 'E' && game->ms->map[i] != '\n')
		{
			ft_printf("Error\nThe map can be composed of only these 5 "
				"characters: '0', '1', 'C', 'E' and 'P'.\n");
			exit(0);
		}
		i++;
	}
}

static void	ft_has_requirements(t_game *game)
{
	int	i;
	int	exit2;
	int	player;
	int	collectable;

	exit2 = 0;
	player = 0;
	collectable = 0;
	i = 0;
	while (game->ms->map[i])
	{
		if (game->ms->map[i] == 'C')
			collectable++;
		if (game->ms->map[i] == 'E')
			exit2++;
		if (game->ms->map[i] == 'P')
			player++;
		i++;
	}
	if (player != 1 || exit2 != 1 || collectable == 0)
	{
		ft_printf("Error\nThe map must contain 1 exit, at least 1 collectible, "
			"and 1 starting position to be valid.\n");
		exit(0);
	}
}

void	ft_check_if_map_valid(t_game *game)
{
	ft_has_requirements(game);
	ft_has_right_characters(game);
	ft_is_rectangle(game);
	ft_has_closed_walls(game);
}
