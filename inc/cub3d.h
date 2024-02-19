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

#ifndef CUB3D_H
# define CUB3D_H

# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <fcntl.h>

//# include "mlx/mlx.h"
# include "../libft/libft.h"
//# include "ft_printf/ft_printf.h"
# include "../get_next_line/get_next_line.h"

int error(char *msg);
int check_input(char *input);

#endif
