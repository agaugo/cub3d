/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   map_validate.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/06 12:03:11 by trstn4        #+#    #+#                 */
/*   Updated: 2024/03/07 18:05:49 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3D.h"

int cub_validate_map(char *file)
{
	cub_setup_map_checks(file);
	cub_is_border_valid("output.txt");
	return (0);
}
