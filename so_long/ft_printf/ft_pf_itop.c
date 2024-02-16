/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_itop.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tvan-bee <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/03 11:14:10 by tvan-bee      #+#    #+#                 */
/*   Updated: 2022/11/03 11:14:12 by tvan-bee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/**
 * Counts the length of the parsed number by dividing it by base 16 and adding
 * len '+1' until 0 is reached.
 * @param num The parsed number.
 * @return The total length of the number.
 */
static int	ft_ptr_len(uintptr_t num)
{
	int	len;

	len = 0;
	while (num > 0)
	{
		len++;
		num = num / 16;
	}
	return (len);
}

/**
 * Divides and modulates the number if the number is bigger than '16' using
 * recursion. If the number is '9' or lower than '9', if will conver the print
 * the current number in string. If the number is between '10' and '16', the
 * number will be printed in required hexa letter.
 * @param num The parsed number that will be converted to hexadecimal.
 * @param sign The parsed sign that will decide if the lower- or upper-case
 * will be applied.
 */
static void	ft_calc_ptr(uintptr_t num)
{
	if (num >= 16)
	{
		ft_calc_ptr(num / 16);
		ft_calc_ptr(num % 16);
	}
	else
	{
		if (num <= 9)
			ft_pf_putchar((num + '0'));
		else
			ft_pf_putchar((num - 10 + 'a'));
	}
}

/**
 * Handles the parsed number and prints the first pointer adress chars. Then it
 * converts the number to hexadecimal using 'ft_calc_ptr' function.
 * @see ft_calc_ptr()
 * @param num The parsed number that will be converter.
 * @return The total length of the pointer adress.
 */
int	ft_pf_itop(unsigned long long num)
{
	ft_pf_putstr("0x");
	if (num == 0)
		return (ft_pf_putchar('0') + 2);
	else
		ft_calc_ptr(num);
	return (ft_ptr_len(num) + 2);
}
