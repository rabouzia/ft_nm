#include "ft_nm.h"

void ft_clean(t_nm *nm)
{
	if (nm->fdata)
	munmap(nm->fdata, nm->fsize);
	if (nm->res)
	ft_resclear(&nm->res);
	
	memset(nm, 0, sizeof(t_nm));
	close(nm->fd);
}


void ft_end(t_nm *nm, char *msg)
{
	if (nm->fdata)
		munmap(nm->fdata, nm->fsize);
	if (nm->res)
		ft_resclear(&nm->res);
	if (nm->fd != -1)
		close(nm->fd);
	if (msg)
		fprintf(stdout,"%s", msg);
	exit(1);
}

// void ft_corrupt(t_nm *nm, char *msg)
// {
// 	if (nm->fdata)
// 		munmap(nm->fdata, nm->fsize);
// 	if (nm->res)
// 		ft_resclear(&nm->res);
// 	if (nm->fd != -1)
// 		close(nm->fd);
// 	fprintf(stderr,CORRUPT_MSG1, msg);
// 	fprintf(stderr,CORRUPT_MSG2, msg);
// 	exit(1);
// }

void ft_sym(t_nm *nm, char *msg)
{
	if (nm->fdata)
		munmap(nm->fdata, nm->fsize);
	if (nm->res)
		ft_resclear(&nm->res);
	if (nm->fd != -1)
		close(nm->fd);
	fprintf(stderr,NO_SYMBOLS, msg);
	exit(1);
}


void not_elf(t_nm *nm,char *filename)
{
	if (nm->fdata)
		munmap(nm->fdata, nm->fsize);
	if (nm->res)
		ft_resclear(&nm->res);
	if (nm->fd != -1)
		close(nm->fd);
	if (filename)
		fprintf(stderr,NO_ELF, filename);
	exit(1);
}


int check_elf(t_nm *nm, char *av)
{
	struct stat st;

	nm->fd = open(av, O_RDONLY);
	if (nm->fd == -1)
	{
		perror("nm: run");
		ft_end(nm, 0);
	}
	if (fstat(nm->fd, &st) < 0 || S_ISDIR(st.st_mode))
        return fprintf(stderr, "nm: Warning: '%s' is a directory\n", av);

	nm->fsize = st.st_size;
	if (nm->fsize == 0)
		ft_end(nm, ELF_ERR);

	nm->fdata = mmap(NULL, nm->fsize, PROT_READ, MAP_PRIVATE, nm->fd, 0);
	if (nm->fdata == MAP_FAILED)
	{
		fprintf(stderr, "nm: Warning: '%s': Unable to map file\n", av);
		ft_end(nm, 0);
	}
		unsigned char *e_ident = (unsigned char *)nm->fdata;
	if (nm->fsize < EI_NIDENT || 
	e_ident[EI_MAG0] != ELFMAG0 || 
	e_ident[EI_MAG1] != ELFMAG1 || 
	e_ident[EI_MAG2] != ELFMAG2 || 
	e_ident[EI_MAG3] != ELFMAG3 ||
	(e_ident[EI_CLASS] != ELFCLASS32 && e_ident[EI_CLASS] != ELFCLASS64) ||
	(e_ident[EI_DATA] != ELFDATA2LSB && e_ident[EI_DATA] != ELFDATA2MSB) ||
	e_ident[EI_VERSION] != EV_CURRENT)
{
	fprintf(stderr, "nm: %s: file format not recognized\n", av);
	ft_end(nm, 0);
}
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

	const char *at = strchr(s, '@');
	if (!at) return NULL;

	size_t len = at - s;
	char *res = malloc(len + 1);
	if (!res) return NULL;

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
			free(to_free);
		}
		else
		{
			prev = cur;
			cur = cur->next;
		}
	}
}

void clean_duplicate_addr(t_res **res) {
	t_res *curr = *res;
	while (curr && curr->next) {
		if (curr->addr == curr->next->addr &&
		    curr->letter == curr->next->letter &&
		    strncmp(curr->symbol, curr->next->symbol, 19) == 0 &&
		    strncmp(curr->symbol, "__PRETTY_FUNCTION__", 19) == 0) {
			t_res *to_delete = curr->next;
			curr->next = to_delete->next;
			free(to_delete->symbol);
			free(to_delete);
			continue;
		}
		curr = curr->next;
	}
}


bool info_clean(t_nm *nm)
{
	if (!nm->opt.p)
		ft_nmsort(nm, nm->res);
	clean_double(&nm->res);
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

