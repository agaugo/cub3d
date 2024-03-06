/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   map_load_values.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 12:02:26 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/06 13:47:29 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3D.h"

t_map *cub_load_map_values(char *file) {
    int fd, i = 0, max_width = 0;
    char *line;
    t_map *dt = calloc(1, sizeof(t_map));
	
    if (!dt)
		return NULL;

    fd = open(file, O_RDONLY);
    if (fd < 0) {
        free(dt);
        return NULL;
    }

    dt->field = calloc(1, sizeof(char *)); // Start with allocation for 1 line, will realloc as needed
    if (!dt->field) {
        free(dt);
        close(fd);
        return NULL;
    }

    while ((line = get_next_line(fd))) {
        // Dynamically adjust the size of map
        char **temp = realloc(dt->field, (i + 2) * sizeof(char *));
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
        int line_length = strlen(line);
        if (line_length > max_width) max_width = line_length;

        char *p = strchr(line, 'P');
        if (p) {
            dt->start_y = i;
            dt->start_x = (int)(p - line);
        }

        i++;
    }
    dt->field[i] = NULL; // Null-terminate the array of lines
    dt->height = i; // Number of lines read is the height of the map
    dt->width = max_width; // The longest line defines the width of the map

    close(fd);
    return dt;
}
