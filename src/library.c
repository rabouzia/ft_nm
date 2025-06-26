#include "ft_nm.h"

/*void	remove_first(t_env **env)
{
	t_env	*second;
	t_env	*cur;
	t_env	*tmp;

	cur = *env;
	second = (*env)->next;
	tmp = (*env);
	*env = second;
	free_node(tmp);
	return ;
}

void	remove_last(t_env *env)
{
	t_env	*last;

	last = env;
	while (last->next->next)
		last = last->next;
	env = ft_envlast(env);
	free_node(env);
	last->next = NULL;
}

void	remove_node(t_env *env, char *to_delete)
{
	t_env	*tmp;
	t_env	*cur;

	cur = env;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		if (ft_strcmp(cur->key, to_delete) == 0)
			break ;
	}
	tmp->next = (cur)->next;
	free_node(cur);
	return ;
}*/


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

// void ft_resprint(t_nm *nm, int ac)
// {
// 	t_res *res = nm->res;
// 	if ((ac > 2 && !nm->is_opt) || (ac > 3 && nm->is_opt)) 
// 		printf("\n%s:\n", res->filename);
// 	while (res)
// 	{
// 		if (res->trash)
// 		{
// 			res = res->next;
// 			continue;
// 		}
// 		if (res->letter == 'U' || res->letter == 'w') 
// 		{	if (nm->elf.is_64)
// 				printf("%16c", ' ');
// 			else
// 				printf("%8c", ' ');
// 		}
// 		else 
// 		{	
// 			if (nm->elf.is_64)
// 				printf("%016lx", res->addr);
// 			else
// 				printf("%08x", (unsigned int)res->addr);	
// 		}
// 		printf(" %c %s\n", res->letter, res->symbol);
// 		res = res->next;
// 	}
// }

// void print_reverse(t_nm * nm)
// {
// 	int len = ft_nmlen(nm);
// 	int i = len;
// 	// t_nm *tmp = ft_nmlast(nm);
// 	while (len >= 0)
// 	{
// 		tmp = ft_nmnlast(nm, i);
// 		printf("%016lx   %s\n", nm->addr, nm->sym`bol);
// 		i--;
// 		len--;
// 	}
// }

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
		printf("\n%s:\n", printable->filename);

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
					printf("%16c", ' ');
				else
					printf("%8c", ' ');
			}
			else 
			{	
				if (nm->elf.is_64)
					printf("%016lx", res->addr);
				else
					printf("%08x", (unsigned int)res->addr);	
			}
			printf(" %c %s\n", res->letter, res->symbol);
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
						printf("%16c", ' ');
					else
						printf("%8c", ' ');
				} 
				else {
					if (nm->elf.is_64)
						printf("%016lx", res->addr);
					else
						printf("%08x", (unsigned int)res->addr);
				}
				printf(" %c %s\n", res->letter, res->symbol);
			}

		}
}



int	ft_unset(t_res **res, char **arg)
{
	int	i;

	i = 1;
	while (arg[i])
	{
		if (!delete_node(res, arg[i]))
			i++;
		else
			i++;
	}
	return (1);
}

bool	delete_node(t_res **res, char *to_delete)
{
	int	flag;
	int	len = 0;

	flag = search_res(*res, to_delete);
	if (flag == 0)
		return (0);
	if (flag == 1)
		remove_first(res);
	else if (flag == len + 1)
		remove_last(*res);
	else
		remove_node(*res, to_delete);
	return (1);
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

void	remove_first(t_res **res)
{
	t_res	*second;
	// t_res	*cur;
	t_res	*tmp;

	// cur = *res;
	second = (*res)->next;
	tmp = (*res);
	*res = second;
	free_node(tmp);
	return ;
}

void	remove_last(t_res *res)
{
	t_res	*last;

	last = res;
	while (last->next->next)
		last = last->next;
	res = ft_reslast(res);
	free_node(res);
	last->next = NULL;
}

void	remove_node(t_res *res, char *to_delete)
{
	t_res	*tmp;
	t_res	*cur;
	tmp = NULL;
	cur = res;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		if (strcmp(cur->symbol, to_delete) == 0)
			break ;
	}
	tmp->next = (cur)->next;
	free_node(cur);
	return ;
}


void	free_node(t_res *res)
{
	free(res->symbol);
	free(res);
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


void get_flag(t_res *res)
{
    while (res)
    {
        if (res->addr == 0)
            res->letter = 'T';
        res = res->next;
    }
}


int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0')
	{
		if (i < (n - 1))
			i++;
		else
			return (0);
	}
	return ((unsigned char)s1[i] - (unsigned char) s2[i]);
}


char	*ft_strdup(const char *src)
{
	int		i;
	char	*r;

	r = (char *)malloc(sizeof(char) * (strlen(src) + 1));
	if (!r)
		return (NULL);
	i = 0;
	while (src[i])
	{
		r[i] = src[i];
		i++;
	}
	r[i] = 0;
	return (r);
}
