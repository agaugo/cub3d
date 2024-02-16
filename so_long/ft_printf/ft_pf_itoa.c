/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_itoa.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tvan-bee <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/02 10:22:26 by tvan-bee      #+#    #+#                 */
/*   Updated: 2022/11/02 10:22:28 by tvan-bee      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/**
 * Counts the length of the parsed number by dividing it by base 10 and adding
 * len '+1' until 0 is reached.
 * @param n The parsed number.
 * @return The total length of the number.
 */
static int	ft_count_unsigned_num_len(unsigned int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n && ++len)
		n /= 10;
	return (len);
}

/**
 * Converts a number to string type by dividing the number and taking the value
 * of modulus and converting that value to char (flips the number to positive
 * if the number is negative). Then prints and frees the number.
 * @param n The parsed number.
 * @return The total length of the number.
 */
int	ft_pf_unsigned_itoa(unsigned int n)
{
	int			len;
	int			cp_len;
	char		*char_num;

	len = ft_count_unsigned_num_len(n);
	cp_len = len;
	char_num = (char *)malloc(sizeof(char) * (len + 1));
	if (!char_num)
		return (0);
	char_num[len] = '\0';
	if (n == 0)
		char_num[0] = '0';
	if (n < 0)
		char_num[0] = '-';
	while (n)
	{
		if (n > 0)
			char_num[--len] = n % 10 + '0';
		else
			char_num[--len] = n % 10 * -1 + '0';
		n /= 10;
	}
	ft_pf_putstr(char_num);
	free(char_num);
	return (cp_len);
}

/**
 * Counts the length of the parsed number by dividing it by base 10 and adding
 * len '+1' until 0 is reached.
 * @param n The parsed number.
 * @return The total length of the number.
 */
static int	ft_count_num_len(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n && ++len)
		n /= 10;
	return (len);
}

/**
 * Converts a number to string type by dividing the number and taking the value
 * of modulus and converting that value to char (flips the number to positive
 * if the number is negative). Then prints and frees the number.
 * @param n The parsed number.
 * @return The total length of the number.
 */
int	ft_pf_itoa(int n)
{
	int			len;
	int			cp_len;
	char		*char_num;

	len = ft_count_num_len(n);
	cp_len = len;
	char_num = (char *)malloc(sizeof(char) * (len + 1));
	if (!char_num)
		return (0);
	char_num[len] = '\0';
	if (n == 0)
		char_num[0] = '0';
	if (n < 0)
		char_num[0] = '-';
	while (n)
	{
		if (n > 0)
			char_num[--len] = n % 10 + '0';
		else
			char_num[--len] = n % 10 * -1 + '0';
		n /= 10;
	}
	ft_pf_putstr(char_num);
	free(char_num);
	return (cp_len);
}
