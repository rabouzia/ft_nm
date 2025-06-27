#include "ft_nm.h"


int check_opt(char *av, t_opt *opt)
{
	if (!av || !opt)
		return 0;
	if (av[0] == '-' && av[1] != '\0') 
	{
		switch (av[1])
		{
		case 'a':
			opt->a = true;
			return 1;
		case 'r':
			opt->r = true;
			return 1;
		case 'g':
			opt->g = true;
			return 1;
		case 'u':
			opt->u = true;
			return 1;
		case 'p':
			opt->p = true;
			return 1;
		default:
			return (fprintf(stderr, WRONG_OPT), 0);
		}	
	}
	return 0;
}