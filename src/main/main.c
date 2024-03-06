/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 11:53:17 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/06 13:47:23 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/cub3D.h"

void v()
{
	system("leaks cub3D");
}

int main(int argc, char **argv) {
    t_map *map;

    // atexit(v); // Remove this before eval
    if (argc != 2) {
        printf("Usage: %s <map_file>\n", argv[0]);
        return (1);
    }
    cub_validate_map(argv[1]);
    map = cub_load_map_values(argv[1]);
    if (!map) {
        printf("Error\nFailed to initialize data from file.\n");
        return (1);
    }
    cub_start_game(map);
    return (0);
}
