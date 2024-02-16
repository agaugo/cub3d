/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_load_map.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/23 10:12:13 by trstn4        #+#    #+#                 */
/*   Updated: 2023/03/21 16:43:32 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	ft_add_size_stats(t_game *game, char *map)
{
	int	i;
	int	height;
	int	width;

	i = 0;
	width = 0;
	while (map[i] != '\n')
	{
		width++;
		i++;
	}
	i = 0;
	height = 1;
	while (map[i] != '\0')
	{
		if (map[i] == '\n')
			height++;
		i++;
	}
	game->ms->height = height;
	game->ms->width = width;
}

static char	*ft_make_map(t_game *game, char *path)
{
	int		fd;
	char	*line;
	char	*temp;
	char	*map;

	fd = open(path, O_RDONLY);
	line = "\0";
	map = (char *)malloc(1);
	if (!map)
		return (NULL);
	map[0] = '\0';
	while (line)
	{
		line = get_next_line(fd);
		if (line)
		{
			temp = map;
			map = ft_strjoin(temp, line);
			free(temp);
		}
		free(line);
	}
	ft_add_size_stats(game, map);
	game->ms->map = map;
	return (map);
}

char	*ft_load_map(t_game *game, char *path)
{
	char	*map;
	char	*extension;
	int		path_len;
	int		ext_len;

	extension = ".ber";
	path_len = ft_strlen(path);
	ext_len = ft_strlen(extension);
	if (path_len < ext_len || !ft_strnstr(path + path_len - ext_len,
			extension, path_len))
	{
		ft_printf("Error\nThe path does not end with '.ber'.\n");
		exit(0);
	}
	map = ft_make_map(game, path);
	return (map);
}
