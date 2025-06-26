/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 14:56:35 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/11 21:55:20 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

void	ft_parseflag(char c, va_list arg, t_print *print)
{
	if (c == 'c')
		ft_putchar(va_arg(arg, int), print);
	else if (c == 's')
		ft_putstr(va_arg(arg, char *), print);
	else if (c == 'p')
		ft_putaddr(va_arg(arg, void *), print);
	else if (c == 'd' || c == 'i')
		ft_putnbr(va_arg(arg, int), print);
	else if (c == 'u')
		ft_putunbr(va_arg(arg, unsigned int), print);
	else if (c == 'x')
		ft_putnbrbase(va_arg(arg, unsigned int), print, "0123456789abcdef", 16);
	else if (c == 'X')
		ft_putnbrbase(va_arg(arg, unsigned int), print, "0123456789ABCDEF", 16);
	else
	{
		ft_putchar('%', print);
		ft_putchar(c, print);
	}
}

char	*ft_mprintf(const char *s, ...)
{
	va_list		args;
	t_print		print;

	va_start(args, s);
	ft_count_chars(&print, s, args);
	va_end(args);
	print.type = 3;
	print.str = malloc(print.size + 1);
	if (!print.str)
		return (0);
	va_start(args, s);
	while (*s)
	{
		if (*s == '%' && *(s + 1) && *(s + 1) != '%')
			ft_parseflag(*(++s), args, &print);
		else if (*s == '%' && *(s + 1) == '%')
			ft_putchar(*(++s), &print);
		else
			ft_putchar(*s, &print);
		++s;
	}
	va_end(args);
	print.str[print.len] = 0;
	return (print.str);
}

char	*ft_sprintf(char *buff, const char *s, ...)
{
	const char	*str;
	va_list		args;
	t_print		print;

	str = s;
	print.type = 3;
	print.len = 0;
	print.size = 0;
	print.str = buff;
	va_start(args, s);
	while (*str)
	{
		if (*str == '%' && *(str + 1) && *(str + 1) != '%')
			ft_parseflag(*(++str), args, &print);
		else if (*str == '%' && *(str + 1) == '%')
			ft_putchar(*(++str), &print);
		else
			ft_putchar(*str, &print);
		++str;
	}
	va_end(args);
	print.str[print.len] = 0;
	return (print.str);
}

int	ft_dprintf(int fd, const char *s, ...)
{
	t_print	print;
	va_list	args;

	print.fd = fd;
	print.len = 0;
	print.size = 0;
	print.type = 1;
	va_start(args, s);
	while (*s)
	{
		if (*s == '%' && *(s + 1) && *(s + 1) != '%')
			ft_parseflag(*(++s), args, &print);
		else if (*s == '%' && *(s + 1) == '%')
			ft_putchar(*(++s), &print);
		else
			ft_putchar(*s, &print);
		++s;
	}
	va_end(args);
	ft_flushbuff(&print);
	return (print.len);
}

int	ft_printf(const char *s, ...)
{
	t_print	print;
	va_list	args;

	print.fd = 1;
	print.len = 0;
	print.size = 0;
	print.type = 0;
	va_start(args, s);
	while (*s)
	{
		if (*s == '%' && *(s + 1) && *(s + 1) != '%')
			ft_parseflag(*(++s), args, &print);
		else if (*s == '%' && *(s + 1) == '%')
			ft_putchar(*(++s), &print);
		else
			ft_putchar(*s, &print);
		++s;
	}
	va_end(args);
	ft_flushbuff(&print);
	return (print.len);
}
