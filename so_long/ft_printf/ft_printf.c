/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tvan-bee <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/28 12:20:43 by tvan-bee      #+#    #+#                 */
/*   Updated: 2022/12/21 14:44:03 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../libft/libft.h"

/**
 * Processes and prints each conversion using the needed function.
 * @param args The parsed arguments.
 * @param conv The parsed conversion.
 * @return The length of the total printed chars in the string.
 */
static int	ft_conv_conv(va_list args, char conv)
{
	if (conv == 'c')
		return (ft_pf_putchar(va_arg(args, int)));
	else if (conv == 's')
		return (ft_pf_putstr(va_arg(args, char *)));
	else if (conv == 'p')
		return (ft_pf_itop(va_arg(args, unsigned long long)));
	else if (conv == 'd' || conv == 'i')
		return (ft_pf_itoa(va_arg(args, int)));
	else if (conv == 'u')
		return (ft_pf_unsigned_itoa(va_arg(args, int)));
	else if (conv == 'x')
		return (ft_pf_itoh(va_arg(args, unsigned int), 'x'));
	else if (conv == 'X')
		return (ft_pf_itoh(va_arg(args, unsigned int), 'X'));
	else if (conv == '%')
		return (ft_pf_putchar('%'));
	return (0);
}

/**
 * Handles each conversion and prints the given strings/chars in the format.
 * @see ft_conv_conv()
 * @param format The parsed format (contains conversions and\or strings/chars).
 * @return The length of the total printed chars in the format.
 */
int	ft_printf(const char *format, ...)
{
	int		i;
	int		len;
	va_list	args;

	va_start(args, format);
	i = 0;
	len = 0;
	while (format[i])
	{
		if (format[i] == '%' && (i + 1) <= (int)ft_strlen(format))
		{
			len += ft_conv_conv(args, format[i + 1]);
			i++;
		}
		else
			len += ft_pf_putchar(format[i]);
		if (format[i] != '\0')
			i++;
	}
	va_end(args);
	return (len);
}
