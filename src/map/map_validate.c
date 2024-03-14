/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   map_validate.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 12:03:11 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/13 22:45:10 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void cub_is_cub_extension(char *file)
{
	char	*extension;
	int		path_len;
	int		ext_len;

	extension = ".cub";
	path_len = ft_strlen(file);
	ext_len = ft_strlen(extension);
	if (path_len < ext_len || !ft_strnstr(file + path_len - ext_len,
			extension, path_len))
	{
		printf("Error\nThe file does have the '.cub' extension.\n");
		exit(1);
	}
}

int cub_validate_map(t_map *map, char *file)
{
	t_check_map *check_map;
	
	cub_is_cub_extension(file);
	check_map = ft_calloc(1, sizeof(t_check_map));
	cub_setup_map_checks(map, check_map);
    if (cub_is_border_valid(check_map) == 1)
	{
		printf("Error\nThe map must be closed/surrounded by walls.\n");
		exit(1);
	}
	free(check_map->field);
	free(check_map);
	return (0);
}
