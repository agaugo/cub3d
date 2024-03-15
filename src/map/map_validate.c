/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   map_validate.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 12:03:11 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/15 10:58:00 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

// Checks if a character is allowed
bool cub_is_char_in_here(char c) {
    return c == '0' || c == '1' || c == 'N' || c == 'E' || c == 'W' || c == 'S' || c == '\n' || c == '\0';
}

// Removes white spaces and checks characters
void cub_check_map_characters(t_map *map)
{
    for (int i = 0; i < map->height; i++) {
        for (int j = 0; map->field[i][j] != '\0'; j++) {
            // Skipping whitespace characters
            if (map->field[i][j] == ' ' || map->field[i][j] == '\t') {
                continue;
            }

            if (!cub_is_char_in_here(map->field[i][j])) {
                printf("Error\nCharacter: '%c' is not allowed.\n", map->field[i][j]);
                exit(1);
            }
        }
    }
}

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
	free(check_map->field);
	free(check_map);
	return (0);
}
