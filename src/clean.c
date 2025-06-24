#include "ft_nm.h"

void ft_clean(t_nm *nm)
{
	if (nm->fdata)
		munmap(nm->fdata, nm->fsize);
	// ft_resclear(&nm->res);
	memset(nm, 0, sizeof(t_nm));
	close(nm->fd);
}


void ft_end(t_nm *nm, char *msg)
{
	if (nm->fdata)
		munmap(nm->fdata, nm->fsize);
	ft_resclear(&nm->res);
	close(nm->fd);
	fprintf(stderr, "%s", msg);
	exit(1);
}

int check_elf(t_nm *nm, char *av)
{
	struct stat st;

	nm->fd = open(av, O_RDONLY);
	if (nm->fd == -1)
		ft_end(nm, OP_ERR);
	if (fstat(nm->fd, &st) < 0)
		ft_end(nm, FSTAT_ERR);

	nm->fsize = st.st_size;
	nm->fdata = mmap(NULL, nm->fsize, PROT_READ, MAP_PRIVATE, nm->fd, 0);
	if (nm->fdata == MAP_FAILED)
		ft_end(nm, MMAP_ERR);

	unsigned char *e_ident = (unsigned char *)nm->fdata;
	if (e_ident[EI_MAG0] != ELFMAG0 || e_ident[EI_MAG1] != ELFMAG1 || e_ident[EI_MAG2] != ELFMAG2 || e_ident[EI_MAG3] != ELFMAG3)
		ft_end(nm, NO_ELF);

	return 1;
}

t_res *	search_word(t_res *res, char *key)
{
	while (res)
	{
		if (strcmp(res->symbol, key) == 0)
		return (res);
		res = res->next;
	}
	return (NULL);
}

char *extract_str(const char *s)
{
	if (!s) return NULL;

	size_t len = 0;
	while (s[len] && s[len] != '@')
		len++;

	char *res = malloc(len + 1);
	if (!res)
		return NULL;

	memcpy(res, s, len);
	res[len] = '\0';
	return res;
}


void clean_double(t_res **res)
{
	t_res *cur = *res;

	while (cur)
	{
		if (!strchr(cur->symbol, '@'))
		{
			t_res *cand = *res;
			while (cand)
			{
				if (strchr(cand->symbol, '@'))
				{
					char *base = extract_str(cand->symbol);
					if (base && ignore_underscore(cur->symbol, base) == 0)
					{
						cur->trash = 1;
						free(base);
						break;
					}
					free(base);
				}
				cand = cand->next;
			}
		}
		cur = cur->next;
	}
}

void remove_double(t_res **res)
{
	t_res *cur = *res;
	t_res *prev = NULL;
	
	while (cur)
	{
		if (cur->trash)
		{
			if (prev)
			prev->next = cur->next;
			else
			*res = cur->next;
			
			t_res *to_free = cur;
			cur = cur->next;
			// free(to_free->symbol);
			free(to_free);
		}
		else
		{
			prev = cur;
			cur = cur->next;
		}
	}
}

bool info_clean(t_nm *nm)
{
	ft_nmsort(nm->res);
	clean_double(&nm->res);
	ft_check_same(&nm->res);
	remove_double(&nm->res);
	return 1;
}


int ignore_underscore(const char* a, const char* b)
{
	int i = 0, j = 0;
    while (a[i] == '_')
		i++;
    while (b[j] == '_')
		j++;
    while (a[i] && b[j]) 
	{
        if (a[i] != b[j]) 
			return a[i] - b[j];
        i++;
        j++;
    }
    return a[i] - b[j];
}

