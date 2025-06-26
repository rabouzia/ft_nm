/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dprintf_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 18:53:09 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/11 09:35:46 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

void	ft_flushbuff(t_print *print)
{
	if (!print->size)
		return ;
	print->len += write(print->fd, print->buff, print->size);
	print->size = 0;
}

void	ft_putchar(const char c, t_print *print)
{
	if (print->type == 2)
	{
		print->size++;
		return ;
	}
	if (print->type == 3)
	{
		print->str[print->len++] = c;
		return ;
	}
	if (print->size + 1 == BUFFER_SIZE)
		ft_flushbuff(print);
	print->buff[print->size++] = c;
}

void	ft_putstr(const char *str, t_print *print)
{
	const char	*s;

	if (!str)
	{
		ft_putstr("(null)", print);
		return ;
	}
	s = str;
	while (*s)
	{
		ft_putchar(*s, print);
		++s;
	}
}

void	ft_putnbr(int nb, t_print *print)
{
	unsigned int	n;

	n = nb;
	if (nb < 0)
	{
		n = -nb;
		ft_putchar('-', print);
	}
	if (n >= 10)
		ft_putnbr(n / 10, print);
	ft_putchar('0' + n % 10, print);
}
