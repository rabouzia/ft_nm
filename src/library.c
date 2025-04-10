#include "ft_nm.h"


void	ft_nmaddback(t_nm **head, t_nm *new)
{
	if (!head)
		return ;
	if (*head)
		ft_nmlast(*head)->next = new;
	else
		*head = new;
}

t_nm	*ft_nmnew(char *symbol, Elf64_Addr addr, char c)
{
	t_nm	*nm;

	nm = malloc(sizeof(t_nm));
	if (!nm)
		return (NULL);
	nm->symbol = symbol;
	nm->addr = addr;
    nm->letter = c;
	nm->next = NULL;
	return (nm);
}

t_nm	*ft_nmlast(t_nm *head)
{
	while (head->next)
		head = head->next;
	return (head);
}

void ft_check_same(t_nm **nm)
{
    (void)nm;
}

void	ft_nmclear(t_nm **nm)
{
	t_nm	*tmp;

	while ((*nm))
	{
		tmp = (*nm)->next;
		// free((*nm)->symbol);
		// free((*nm)->value);
		free((*nm));
		(*nm) = tmp;
	}
}

void ft_nmprint(t_nm *nm)
{
    while(nm)
    {
        if (nm->letter != '-'){
            printf("%18c %s\n", nm->letter, nm->symbol);
        }
        else
          printf("%016lx   %s\n", nm->addr, nm->symbol);

            // printf("\n", nm->addr);
        nm = nm->next;
    }
}

int	ft_unset(t_nm **nm, char **arg)
{
	int	i;

	i = 1;
	while (arg[i])
	{
		if (!delete_node(nm, arg[i]))
			i++;
		else
			i++;
	}
	return (1);
}

bool	delete_node(t_nm **nm, char *to_delete)
{
	int	flag;
	int	len = 0;

	flag = search_nm(*nm, to_delete);
	if (flag == 0)
		return (0);
	if (flag == 1)
		remove_first(nm);
	else if (flag == len + 1)
		remove_last(*nm);
	else
		remove_node(*nm, to_delete);
	return (1);
}

int	search_nm(t_nm *nm, char *key)
{
	int	i;

	i = 1;
	while (nm)
	{
		if (strcmp(nm->symbol, key) == 0)
			return (i);
		nm = nm->next;
		i++;
	}
	return (0);
}

void	remove_first(t_nm **nm)
{
	t_nm	*second;
	t_nm	*cur;
	t_nm	*tmp;

	cur = *nm;
	second = (*nm)->next;
	tmp = (*nm);
	*nm = second;
	free_node(tmp);
	return ;
}

void	remove_last(t_nm *nm)
{
	t_nm	*last;

	last = nm;
	while (last->next->next)
		last = last->next;
	nm = ft_nmlast(nm);
	free_node(nm);
	last->next = NULL;
}

void	remove_node(t_nm *nm, char *to_delete)
{
	t_nm	*tmp;
	t_nm	*cur;

	cur = nm;
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


void	free_node(t_nm *nm)
{
	free(nm->symbol);
	free(nm);
}

int ft_nmlen(t_nm *nm)
{
	int i  = 0;
	while(nm)
	{
		i++;
		nm= nm->next;
	}
	return i;
}


void get_flag(t_nm *nm)
{
    while (nm)
    {
        if (nm->addr == 0)
            nm->letter = 'T';
        nm = nm->next;
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