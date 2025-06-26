/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouhliss <wouhliss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 18:45:33 by ybelatar          #+#    #+#             */
/*   Updated: 2024/02/12 03:47:19 by wouhliss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DPRINTF_H

# define FT_DPRINTF_H

# include <stdarg.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

typedef struct s_print
{
	char	buff[BUFFER_SIZE];
	char	*str;
	int		fd;
	int		len;
	int		size;
	int		type;
}			t_print;

int			ft_printf(const char *str, ...);
char		*ft_sprintf(char *buff, const char *s, ...);
int			ft_dprintf(int fd, const char *str, ...);
char		*ft_mprintf(const char *s, ...);

void		ft_flushbuff(t_print *print);
void		ft_putchar(const char c, t_print *print);
void		ft_putstr(const char *str, t_print *print);
void		ft_putnbr(int nb, t_print *print);
void		ft_putunbr(unsigned int nb, t_print *print);
void		ft_putnbrbase(size_t nb, t_print *print, const char *base,
				size_t base_len);
void		ft_putaddr(void *addr, t_print *print);
void		ft_count_chars(t_print *print, const char *s, va_list args);
void		ft_parseflag(char c, va_list arg, t_print *print);

#endif
