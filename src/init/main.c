/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: hflohil- <hflohil-@codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/10 17:16:15 by hugo          #+#    #+#                 */
/*   Updated: 2023/12/15 15:39:03 by hugo          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int    error(char *msg)
{
    printf("Error: ");
    printf("%s\n", msg);
    return (0);
}

int     main(int argc, char *argv[])
{
    int fd;

    if (argc != 2)
        return (error("invalid arg"));
    fd = open(argv[1], O_RDONLY);
    if (check_input(argv[1]) == 0 || fd == -1)
        return (error("invalid input"));
    return (0);
}