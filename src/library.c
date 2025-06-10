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
    (void)res;
}

void	ft_resclear(t_res **res)
{
	t_res	*tmp;

	while ((*res))
	{
		tmp = (*res)->next;
		// free((*res)->symbol);
		// free((*res)->value);
		free((*res));
		(*res) = tmp;
	}
}

void ft_resprint(t_res *res)
{
    while(res)
    {
        
		if (res->letter == 'U') 
			printf("%16c " , ' ');
		else 
			printf("%016lx " , res->addr);
		printf(" %c ",res->letter);
		printf("%s\n", res->symbol);
            // printf("\n", res->addr);
        res = res->next;
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


void ft_putchar(char c)
{
	write(1, &c, 1);
}


void ft_putstr_fd(char *str)
{
	int i = 0;
	while(str[i])
	{
		ft_putchar(str[i]);
		i++;
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


