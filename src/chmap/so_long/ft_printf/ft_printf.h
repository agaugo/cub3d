/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tvan-bee <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/02 10:22:49 by tvan-bee      #+#    #+#                 */
/*   Updated: 2022/12/21 16:10:25 by trstn4        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>

int		ft_printf(const char *format, ...);

int		ft_pf_putchar(char c);
int		ft_pf_putstr(char *s);

int		ft_pf_itop(unsigned long long ptr);

int		ft_pf_itoa(int n);
int		ft_pf_unsigned_itoa(unsigned int n);

int		ft_pf_itoh(unsigned int num, char sign);

#endif
