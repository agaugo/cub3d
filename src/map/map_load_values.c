/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   map_load_values.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 12:02:26 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/13 23:50:00 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3D.h"
#include <ctype.h>

int	rgb_to_hex(int r, int g, int b)
{
	return (r << 24 | g << 16 | b << 8 | 255 << 0);
}

unsigned int parse_rgb_string_to_hex(char *rgb_string) {
    char **rgb_parts = ft_split(rgb_string, ',');
    int r, g, b;

    if (!rgb_parts) return 0; // Error handling or return a default color

    r = ft_atoi(rgb_parts[0]);
    g = ft_atoi(rgb_parts[1]);
    b = ft_atoi(rgb_parts[2]);

    unsigned int hex_color = rgb_to_hex(r, g, b);

    // Free the split parts
    for (int i = 0; rgb_parts[i] != NULL; i++) {
        free(rgb_parts[i]);
    }
    free(rgb_parts);

    return hex_color;
}

void cub_set_player_start_position(t_mlx *mlx) {
    mlx->player->pixel_x = mlx->map->player_start_x * mlx->map->tile_size + (mlx->map->tile_size / 2);
    mlx->player->pixel_y = mlx->map->player_start_y * mlx->map->tile_size + (mlx->map->tile_size / 2);
    if (mlx->player->direction == 'N')
		mlx->player->angle = 3 * M_PI / 2;
    if (mlx->player->direction == 'E')
		mlx->player->angle = 0;
    if (mlx->player->direction == 'S')
        mlx->player->angle = M_PI / 2;
    if (mlx->player->direction == 'W')
		mlx->player->angle = M_PI;
}

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

// Utility function to trim spaces and tabs and return a new allocated string
char *trim_whitespace(char *str) {
    char *start, *end, *result;

    // Trim leading space
    while(isspace((unsigned char)*str)) str++;
    start = str;

    if(*str == 0)  // All spaces?
        return strdup(""); // return an empty string

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Copy the trimmed string
    result = (char *)malloc(end - start + 2);
    if(result) {
        strncpy(result, start, end - start + 1);
        result[end - start + 1] = '\0';
    }

    return result;
}

char *extract_value(char *line) {
    char *value_start = strchr(line, ' ');
    if (!value_start) return strdup(""); // Return an empty string if no space found
    return trim_whitespace(value_start + 1);
}

void store_identifier_value(char *line, t_map *dt) {
    if (strncmp(line, "NO", 2) == 0)
        dt->id_no = extract_value(line);
    else if (strncmp(line, "SO", 2) == 0)
        dt->id_so = extract_value(line);
    else if (strncmp(line, "WE", 2) == 0)
        dt->id_we = extract_value(line);
    else if (strncmp(line, "EA", 2) == 0)
        dt->id_ea = extract_value(line);
    else if (strncmp(line, "F", 1) == 0)
        dt->id_f = extract_value(line);
    else if (strncmp(line, "C", 1) == 0)
        dt->id_c = extract_value(line);
    else
        printf("Error\nInvalid identifier found.\n");
}

t_map *cub_load_map_values(char *file) {
    int fd, i = 0, max_width = 0;
    char *line;
    t_map *dt = ft_calloc(1, sizeof(t_map));
    if (!dt) return NULL;

    fd = open(file, O_RDONLY);
    if (fd < 0) {
        free(dt);
        return NULL;
    }

    dt->field = ft_calloc(1, sizeof(char *)); // For the map lines
    if (!dt->field) {
        free(dt);
        close(fd);
        return NULL;
    }

    int is_map_line = 0; // Flag to check if current lines are part of the map
    int count = 0;
    while ((line = get_next_line(fd))) {
        if (!is_map_line && count == 6) {
            is_map_line = 1; // Map starts
        }

        if (!is_map_line) {
            store_identifier_value(line, dt); // Store identifier values
            free(line); // Free line after storing value
        } else {
            // Process map line as before
            char **temp = memory_realloc(dt->field, (i + 2) * sizeof(char *));
            if (!temp) {
                // Handle allocation failure; free previously allocated memory
                while (i-- > 0) free(dt->field[i]);
                free(dt->field);
                free(dt);
                close(fd);
                return NULL;
            }
            dt->field = temp;
            dt->field[i++] = line; // Save map line and increment counter
            int line_length = strlen(line);
            if (line_length > max_width) max_width = line_length;
        }
    }

    printf("C:%s\n", dt->id_c);
    printf("F:%s\n", dt->id_f);
    printf("NO:%s\n", dt->id_no);
    printf("EA:%s\n", dt->id_ea);
    printf("SO:%s\n", dt->id_so);
    printf("WE:%s\n", dt->id_we);

    dt->color_ceiling = parse_rgb_string_to_hex(dt->id_c);
    dt->color_floor = parse_rgb_string_to_hex(dt->id_f);

    printf("FLOOR COLOR:%d\n", dt->color_floor);
    printf("CEILING COLOR:%d\n", dt->color_ceiling);

    dt->field[i] = NULL; // Null-terminate the array of lines
    dt->height = i;
    dt->width = max_width;
    dt->tile_size = cub_get_tile_size(dt);

    close(fd);
    return dt;
}

