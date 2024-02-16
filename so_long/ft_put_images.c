/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_put_images.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/03/14 11:29:32 by trstn4        #+#    #+#                 */
/*   Updated: 2023/03/21 16:58:50 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	ft_put_images_1(t_game *game, int i, int o, int j)
{
	if (game->ms->map[o] == '1')
	{
		mlx_put_image_to_window(game->vars->mlx, game->vars->win,
			game->images->wall, (i * IMG_SIZE), (j * IMG_SIZE));
	}
	else
	{
		mlx_put_image_to_window(game->vars->mlx, game->vars->win,
			game->images->ground, (i * IMG_SIZE), (j * IMG_SIZE));
	}
	if (game->ms->map[o] == 'E')
	{
		mlx_put_image_to_window(game->vars->mlx, game->vars->win,
			game->images->exit, (i * IMG_SIZE), (j * IMG_SIZE));
	}
}

static void	ft_put_images_2(t_game *game, int i, int o, int j)
{
	if (game->ms->map[o] == 'P')
	{
		game->player->x = i;
		game->player->y = j;
		mlx_put_image_to_window(game->vars->mlx, game->vars->win,
			game->images->player, (i * IMG_SIZE), (j * IMG_SIZE));
	}
	if (game->ms->map[o] == 'C')
	{
		game->ms->collectables++;
		mlx_put_image_to_window(game->vars->mlx, game->vars->win,
			game->images->collectable, (i * IMG_SIZE), (j * IMG_SIZE));
	}
}

static void	ft_load_images(t_game *game)
{
	int	img_width;
	int	img_height;

	game->images->wall = mlx_xpm_file_to_image(game->vars->mlx,
			"./xpm/wall.xpm", &img_width, &img_height);
	game->images->ground = mlx_xpm_file_to_image(game->vars->mlx,
			"./xpm/ground.xpm", &img_width, &img_height);
	game->images->exit = mlx_xpm_file_to_image(game->vars->mlx,
			"./xpm/exit.xpm", &img_width, &img_height);
	game->images->collectable = mlx_xpm_file_to_image(game->vars->mlx,
			"./xpm/collectable.xpm", &img_width, &img_height);
	game->images->player = mlx_xpm_file_to_image(game->vars->mlx,
			"./xpm/player.xpm", &img_width, &img_height);
}

void	ft_put_images(t_game *game)
{
	int	i;
	int	j;
	int	o;

	i = 0;
	j = 0;
	o = 0;
	ft_load_images(game);
	while (j < game->ms->height)
	{
		while (i < game->ms->width)
		{
			ft_put_images_1(game, i, o, j);
			ft_put_images_2(game, i, o, j);
			i++;
			o++;
		}
		j++;
		i = 0;
		o++;
	}
}
