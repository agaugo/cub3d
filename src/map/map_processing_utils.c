/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   map_processing_utils.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: trstn4 <trstn4@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/03 14:52:36 by trstn4        #+#    #+#                 */
/*   Updated: 2024/04/06 17:43:45 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

char	*trim_whitespace(char *str)
{
	char	*start;
	char	*end;
	char	*result;

	while (isspace((unsigned char)*str))
		str++;
	start = str;
	if (*str == 0)
		return (ft_strdup(""));
	end = str + ft_strlen(str) - 1;
	while (end > str && isspace((unsigned char)*end))
		end--;
	result = (char *)malloc(end - start + 2);
	if (result)
	{
		strncpy(result, start, end - start + 1);
		result[end - start + 1] = '\0';
	}
	return (result);
}

char	*extract_value(char *line)
{
	char	*value_start;

	value_start = ft_strchr(line, ' ');
	if (!value_start)
		return (ft_strdup(""));
	return (trim_whitespace(value_start + 1));
}

void	store_identifier_value(char *line, t_map *map)
{
	if (strncmp(line, "NO", 2) == 0)
		map->id_no = extract_value(line);
	else if (strncmp(line, "SO", 2) == 0)
		map->id_so = extract_value(line);
	else if (strncmp(line, "WE", 2) == 0)
		map->id_we = extract_value(line);
	else if (strncmp(line, "EA", 2) == 0)
		map->id_ea = extract_value(line);
	else if (strncmp(line, "F", 1) == 0)
		map->id_f = extract_value(line);
	else if (strncmp(line, "C", 1) == 0)
		map->id_c = extract_value(line);
}

void	process_map_line(char *line, t_map *map, int *i, int *max_width)
{
	char	**temp;
	int		line_length;

	temp = memory_realloc(map->field, (*i + 2) * sizeof(char *));
	if (!temp)
	{
		perror("Error\nMemory reallocation failed for map line");
		exit(1);
	}
	map->field = temp;
	map->field[(*i)++] = line;
	line_length = ft_strlen(line);
	if (line_length > *max_width)
		*max_width = line_length;
}

void	process_lines(int fd, t_map *map)
{
	char	*line;
	int		is_map_line;
	int		max_width;
	int		i;

	is_map_line = 0;
	max_width = 0;
	i = 0;
	line = get_next_line(fd);
	while (line)
	{
		if (!is_map_line && (line[0] == '1' || line[0] == ' '))
			is_map_line = 1;
		if (!is_map_line)
		{
			store_identifier_value(line, map);
			free(line);
		}
		else
			process_map_line(line, map, &i, &max_width);
		line = get_next_line(fd);
	}
	finalize_map(map, i, max_width);
}
