#include "ft_nm.h"
#include <ctype.h>

int main(int ac, char **av) 
{
	static t_nm nm = {0};
	int i = 1;
	if (ac >= 2)
	{
		nm.is_opt = check_opt(av[i], &nm.opt);
		if (nm.is_opt)
			i++;
	}
	if (ac - nm.is_opt > ARG_MAX)
		return (fprintf(stderr,"nm: Too many arguments\n"), 0);
	if (ac < 2 || (nm.is_opt && ac < 3))
		{
			int fd = open("a.out", O_RDONLY);
			if (fd == -1)
				return (fprintf(stderr,NO_AOUT), 0);
			close(fd);
			if (!check_elf(&nm, "a.out") || !parse_elf(&nm, "a.out") || !info_clean(&nm))
				return (0);
			ft_resprint(&nm, 2);
			ft_clean(&nm);
			return (0);
		}
	while(av[i])
	{
		if (!check_elf(&nm, av[i]))
			return(0);
		if (!parse_elf(&nm, av[i]))
			return(0);
		if (!info_clean(&nm))
			return(0);
		// ft_resprint(nm);
		// if (nm.opt.g)
		// 	print_maj(&nm);
		// else
		ft_resprint(&nm, ac);
		free(nm.res->filename);
		ft_clean(&nm);
		nm = (t_nm){0};
		i++;
	}
}



/*
-a     Display all symbol table entries, including those inserted for use by debuggers.
-g     Display only global (external) symbols.
	remove unauthorised functions
*/
/*
- check the case of adding char to the binary
*/