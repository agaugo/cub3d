/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   map_setup.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 12:20:59 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/13 23:35:50 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3D.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_modified_map(t_check_map *check_map) {
    for (int i = 0; i < check_map->height; i++) {
        printf("%s\n", check_map->field[i]); // Newline added here, after each line is printed
    }
}

void remove_newline(char *line) {
    int len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0'; // Replace newline with null terminator
    }
}

int cub_setup_map_checks(t_map *map, t_check_map *check_map) {
    int longestLineLength = 0;

    // First, calculate the longest line length after removing newlines
    for (int i = 0; i < map->height; i++) {
        remove_newline(map->field[i]); // Remove newline characters
        int length = strlen(map->field[i]);
        if (length > longestLineLength) {
            longestLineLength = length;
        }
    }

    check_map->width = longestLineLength + 2; // Padding on both sides
    check_map->height = map->height + 2; // Top and bottom padding

    check_map->field = (char **)malloc(sizeof(char *) * check_map->height);
    if (!check_map->field) {
        perror("Failed to allocate memory for check_map->field");
        return -1;
    }

    // Top padding
    check_map->field[0] = (char *)calloc(check_map->width + 1, sizeof(char));
    memset(check_map->field[0], '.', check_map->width);

    for (int i = 0; i < map->height; i++) {
        check_map->field[i + 1] = (char *)calloc(check_map->width + 1, sizeof(char));
        check_map->field[i + 1][0] = '.';
        check_map->field[i + 1][check_map->width - 1] = '.';
        
        // Process and copy the line
        for (int j = 0; j < (int)ft_strlen(map->field[i]); j++) {
            check_map->field[i + 1][j + 1] = (map->field[i][j] == ' ') ? '.' : map->field[i][j];
        }
        
        // Extend the line with dots to match the longest line, plus padding
        for (int j = strlen(map->field[i]) + 1; j < check_map->width - 1; j++) {
            check_map->field[i + 1][j] = '.';
        }
    }

    // Bottom padding
    check_map->field[check_map->height - 1] = (char *)calloc(check_map->width + 1, sizeof(char));
    memset(check_map->field[check_map->height - 1], '.', check_map->width);

    // Print the modified map for verification
    print_modified_map(check_map);

    return 0;
}
