/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   so_long.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/21 16:09:30 by trstn4        #+#    #+#                 */
/*   Updated: 2023/03/21 17:03:55 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "mlx/mlx.h"
# include "libft/libft.h"
# include "ft_printf/ft_printf.h"
# include "get_next_line/get_next_line.h"
# include <fcntl.h>

# define IMG_SIZE 32

typedef struct s_player
{
	int	x;
	int	y;
	int	moves;
	int	collected;
}				t_player;

typedef struct s_mapstats
{
	int		width;
	int		height;
	int		collectables;
	char	*map;
}				t_mapstats;

typedef struct s_vars
{
	void	*mlx;
	void	*win;
}				t_vars;

typedef struct s_images
{
	void	*wall;
	void	*ground;
	void	*exit;
	void	*collectable;
	void	*player;
}				t_images;

typedef struct s_game
{
	t_vars		*vars;
	t_mapstats	*ms;
	t_player	*player;
	t_images	*images;
}				t_game;

char	*ft_load_map(t_game *game, char *path);
void	ft_check_if_map_valid(t_game *game);
void	ft_put_images(t_game *game);
void	ft_move_tile(t_game *game, int x, int y);
void	ft_has_a_valid_path(t_game *game, int player_x, int player_y);
char	**ft_make_2d_map(t_game *game);
void	ft_free_2d_map(char **map_2d, int height);
int		ft_exit(t_game *game);
void	ft_has_closed_walls(t_game *game);

#endif
