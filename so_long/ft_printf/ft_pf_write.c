/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf_utils.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: tvan-bee <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/02 10:23:28 by tvan-bee      #+#    #+#                 */
/*   Updated: 2022/11/02 10:23:29 by tvan-bee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/**
 * Prints each char of the parsed string.
 * @param s The parsed string.
 * @return The length of the total printed chars in the string.
 */
int	ft_pf_putstr(char *s)
{
	int	i;

	if (s == NULL)
		return (ft_pf_putstr("(null)"));
	i = 0;
	while (s[i])
		write(1, &s[i++], 1);
	if (i == 0)
		return (0);
	return (i);
}

/**
 * Prints the parsed char.
 * @param c The parsed char.
 * @return The length of the printed char (which is '1').
 */
int	ft_pf_putchar(char c)
{
	write(1, &c, 1);
	return (1);
}
