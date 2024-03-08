/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   map_load_values.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 12:02:26 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/08 15:38:29 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3D.h"

int cub_get_tile_size(t_map *map)
{
    int tile_width;
    int tile_height;
    int tile_size;

    tile_width = (SCREEN_WIDTH / map->width);
    tile_height = (SCREEN_HEIGHT / map->height);
    if (tile_width < tile_height)
        tile_size = tile_width;
    else
        tile_size = tile_height;
    return (tile_size);
}

t_map *cub_load_map_values(char *file) {
    int fd, i = 0, max_width = 0;
    char *line;
    t_map *dt = ft_calloc(1, sizeof(t_map));
	
    if (!dt)
		return NULL;

    fd = open(file, O_RDONLY);
    if (fd < 0) {
        free(dt);
        return NULL;
    }

    dt->field = ft_calloc(1, sizeof(char *)); // Start with allocation for 1 line, will realloc as needed
    if (!dt->field) {
        free(dt);
        close(fd);
        return NULL;
    }

    while ((line = get_next_line(fd))) {
        // Dynamically adjust the size of map
        char **temp = memory_realloc(dt->field, (i + 2) * sizeof(char *));
        if (!temp) {
            // Handle allocation failure; free previously allocated memory
            while (i-- > 0) free(dt->field[i]);
            free(dt->field);
            free(dt);
            free(line); // Don't forget to free the current line
            close(fd);
            return NULL;
        }
        dt->field = temp;

        dt->field[i] = line;
        int line_length = ft_strlen(line);
        if (line_length > max_width) max_width = line_length;

        char *p = ft_strchr(line, 'P');
        if (p) {
            dt->player_start_y = i;
            dt->player_start_x = (int)(p - line);
        }

        i++;
    }
    dt->field[i] = NULL; // Null-terminate the array of lines
    dt->height = i; // Number of lines read is the height of the map
    dt->width = max_width; // The longest line defines the width of the map
    dt->tile_size = cub_get_tile_size(dt);
    
    close(fd);
    return dt;
}
