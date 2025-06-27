#include "ft_nm.h"

void	ft_resaddback(t_res **head, t_res *new)
{
	if (!head)
		return ;
	if (*head)
		ft_reslast(*head)->next = new;
	else
		*head = new;
}

t_res	*ft_resnew(char *symbol, Elf64_Addr addr, char c)
{
	t_res	*res;

	res = malloc(sizeof(t_res));
	if (!res)
		return (NULL);
	res->symbol = symbol;
	res->addr = addr;
    res->letter = c;
	res->trash = 0;
	res->next = NULL;
	return (res);
}

t_res	*ft_reslast(t_res *head)
{
	while (head->next)
		head = head->next;
	return (head);
}

void ft_check_same(t_res **res)
{
	t_res *tmp = *res;
	while (tmp)
	{
		t_res *cur = tmp->next;
		while (cur)
		{
			if (!cur->trash &&
				strcmp(tmp->symbol, cur->symbol) == 0 &&
				tmp->addr == cur->addr &&
				tmp->letter == cur->letter)
			{
				cur->trash = 1;
			}
			cur = cur->next;
		}
		tmp = tmp->next;
	}
}

void	ft_resclear(t_res **res)
{
	t_res	*tmp;

	while ((*res))
	{
		tmp = (*res)->next;
		free((*res)->symbol);
		// free((*res)->value);
		free((*res));
		(*res) = tmp;
	}
}


t_res *ft_resnlast(t_res *res, int n)
{
	int len = ft_reslen(res);
	if (n < 0 || n >= len)
		return NULL;

	for (int i = 0; i < n && n; i++)
		res = res->next;
	return res;
}



void ft_resprint(t_nm *nm, int ac)
{
	t_res *res = nm->res;
	t_res *printable = res;

	// Sauter les trash au début pour éviter d'utiliser filename sur un node supprimé
	// while (printable && printable->trash)
	// 	printable = printable->next;

	if (printable && ((ac > 2 && !nm->is_opt) || (ac > 3 && nm->is_opt)))
		fprintf(stdout,"\n%s:\n", printable->filename);

	if (!nm->opt.r)
	{
		while (res)
		{
			if (res->trash)
			{
				res = res->next;
				continue;
			}
			if (res->letter == 'U' || res->letter == 'w'|| res->letter == 'v') 
			{	
				if (nm->elf.is_64)
					fprintf(stdout, "%16c", ' ');
				else
					fprintf(stdout,"%8c", ' ');
			}
			else 
			{	
				if (nm->elf.is_64)
					fprintf(stdout,"%016lx", res->addr );
				else
					fprintf(stdout,"%08x", (unsigned int)res->addr);	
			}
			fprintf(stdout," %c %s\n", res->letter, res->symbol);
			res = res->next;
		}
	}
	else
		{
			int len = ft_reslen(nm->res);
			for (int i = len - 1; i >= 0; i--) {
				t_res *res = ft_resnlast(nm->res, i);
				if (!res || res->trash)
					continue;

				if (res->letter == 'U' || res->letter == 'w' || res->letter == 'v') {
					if (nm->elf.is_64)
						fprintf(stdout,"%16c", ' ');
					else
						fprintf(stdout,"%8c", ' ');
				} 
				else {
					if (nm->elf.is_64)
						fprintf(stdout,"%016lx", res->addr);
					else
						fprintf(stdout,"%08x", (unsigned int)res->addr);
				}
				fprintf(stdout," %c %s\n", res->letter, res->symbol);
			}

		}
}

int	search_res(t_res *res, char *key)
{
	int	i;

	i = 1;
	while (res)
	{
		if (strcmp(res->symbol, key) == 0)
			return (i);
		res = res->next;
		i++;
	}
	return (0);
}

int ft_reslen(t_res *res)
{
	int i  = 0;
	while(res)
	{
		i++;
		res= res->next;
	}
	return i;
}