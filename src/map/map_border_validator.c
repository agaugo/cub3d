/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   map_border_validator.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 11:53:14 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/12 18:27:25 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

#include <stdio.h>
#include <stdbool.h>

bool check_surroundings(t_check_map *check_map, int x, int y) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0)
                continue;
            int checkX = x + i;
            int checkY = y + j;
            
            // Ensure we're not checking out of bounds
            if (checkX < 0 || checkY < 0 || checkX >= check_map->height || checkY >= check_map->width)
                continue;
            
            char adjacent = check_map->field[checkX][checkY];
            if (adjacent != '1' && adjacent != '.' && adjacent != '\n' && adjacent != '\0') {
                return false;
            }
        }
    }
    return true;
}

int cub_is_border_valid(t_check_map *check_map) {
    for (int i = 0; i < check_map->height; i++) {
        for (int j = 0; j < check_map->width; j++) {
            if (check_map->field[i][j] == '.') {
                if (!check_surroundings(check_map, i, j)) {
                    return (1);
                }
            }
        }
    }
    return (0);
}
