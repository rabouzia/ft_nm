#include "ft_nm.h"
#include <ctype.h>


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




int parse_elf(t_nm *nm)
{
	if (nm->fdata == NULL) return 0;

	nm->elf.is_64 = (((unsigned char *)nm->fdata)[EI_CLASS] == ELFCLASS64);
	nm->elf.ehdr = nm->fdata;
	if (nm->elf.is_64)
	{
		Elf64_Ehdr *ehdr = (Elf64_Ehdr *)nm->fdata;
		nm->elf.shdr = (void *)((char *)nm->fdata + ehdr->e_shoff);
		if (ehdr->e_shstrndx >= ehdr->e_shnum) return 0;
		Elf64_Shdr *shdr = nm->elf.shdr;
		// char *shstrtab = (char *)nm->fdata + shdr[ehdr->e_shstrndx].sh_offset;
		for (int i = 0; i < ehdr->e_shnum; i++)
		{
			if (shdr[i].sh_type == SHT_SYMTAB || shdr[i].sh_type == SHT_DYNSYM)
			{
				nm->elf.symtab = (void *)((char *)nm->fdata + shdr[i].sh_offset);
				nm->elf.symbols_nb = shdr[i].sh_size / shdr[i].sh_entsize;
				nm->elf.strtab_section = &shdr[shdr[i].sh_link];
				nm->elf.strtab = (char *)nm->fdata + ((Elf64_Shdr *)nm->elf.strtab_section)->sh_offset;
				for (size_t j = 0; j < nm->elf.symbols_nb; j++)
				{
					Elf64_Sym *sym = &((Elf64_Sym *)nm->elf.symtab)[j];
					if (!sym->st_name) continue;
					char *name = nm->elf.strtab + sym->st_name;
					char type = get_symbol_letter(*sym, (Elf64_Shdr *)nm->elf.shdr);
					if (!nm->opt.a && (name[0] == '\0' || name[0] == '$')) continue;
					if (ELF64_ST_TYPE(sym->st_info) == STT_SECTION || ELF64_ST_TYPE(sym->st_info) == STT_FILE) continue;
					if (nm->opt.g && ELF64_ST_BIND(sym->st_info) != STB_GLOBAL) continue;
					if (nm->opt.u && sym->st_shndx != SHN_UNDEF) continue;
					ft_resaddback(&nm->res, ft_resnew(name, sym->st_value, type));
				}
			}
		}
	}
	else
	{
		Elf32_Ehdr *ehdr = (Elf32_Ehdr *)nm->fdata;
		nm->elf.shdr = (void *)((char *)nm->fdata + ehdr->e_shoff);
		if (ehdr->e_shstrndx >= ehdr->e_shnum) return 0;
		Elf32_Shdr *shdr = nm->elf.shdr;
		// char *shstrtab = (char *)nm->fdata + shdr[ehdr->e_shstrndx].sh_offset;
		for (int i = 0; i < ehdr->e_shnum; i++)
		{
			if (shdr[i].sh_type == SHT_SYMTAB || shdr[i].sh_type == SHT_DYNSYM)
			{
				nm->elf.symtab = (void *)((char *)nm->fdata + shdr[i].sh_offset);
				nm->elf.symbols_nb = shdr[i].sh_size / shdr[i].sh_entsize;
				nm->elf.strtab_section = &shdr[shdr[i].sh_link];
				nm->elf.strtab = (char *)nm->fdata + ((Elf32_Shdr *)nm->elf.strtab_section)->sh_offset;
				for (size_t j = 0; j < nm->elf.symbols_nb; j++)
				{
					Elf32_Sym *sym = &((Elf32_Sym *)nm->elf.symtab)[j];
					if (!sym->st_name) continue;
					char *name = nm->elf.strtab + sym->st_name;
					if (!nm->opt.a && (name[0] == '\0' || name[0] == '$')) continue;
					if (ELF32_ST_TYPE(sym->st_info) == STT_SECTION || ELF32_ST_TYPE(sym->st_info) == STT_FILE) continue;
					if (nm->opt.g && ELF32_ST_BIND(sym->st_info) != STB_GLOBAL) continue;
					if (nm->opt.u && sym->st_shndx != SHN_UNDEF) continue;
					ft_resaddback(&nm->res, ft_resnew(name, sym->st_value, '?'));
				}
			}
		}
	}
	return 1;
}

void ft_clean(t_nm *nm, char *msg)
{
	if (nm->fdata)
		munmap(nm->fdata, nm->fsize);
	if (nm->opt.filename)
		free(nm->opt.filename);
	ft_resclear(&nm->res);
	close(nm->fd);
	fprintf(stderr, "%s", msg);
	exit(1);
}

int check_elf(t_nm *nm)
{
	struct stat st;

	nm->fd = open(nm->opt.filename, O_RDONLY);
	if (nm->fd == -1)
		ft_clean(nm, OP_ERR);
	if (fstat(nm->fd, &st) < 0)
		ft_clean(nm, FSTAT_ERR);

	nm->fsize = st.st_size;
	nm->fdata = mmap(NULL, nm->fsize, PROT_READ, MAP_PRIVATE, nm->fd, 0);
	if (nm->fdata == MAP_FAILED)
		ft_clean(nm, MMAP_ERR);

	unsigned char *e_ident = (unsigned char *)nm->fdata;
	if (e_ident[EI_MAG0] != ELFMAG0 || e_ident[EI_MAG1] != ELFMAG1 || e_ident[EI_MAG2] != ELFMAG2 || e_ident[EI_MAG3] != ELFMAG3)
		ft_clean(nm, NO_ELF);

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
	size_t len = 0;
	while (s[len] && s[len] != '@')
	len++;
	
	char *res = malloc(len + 1);
	if (!res)
	return NULL;
	strncpy(res, s, len);
	res[len] = '\0';
	return res;
}

void clean_double(t_res **res)
{
	t_res *cur = *res;
	
	while (cur)
	{
		// Si le symbole est versionné
		if (strchr(cur->symbol, '@'))
		{
			char *base = extract_str(cur->symbol);
			if (!base)
			return;
			
			t_res *cand = *res;
			while (cand)
			{
				// Cible : même nom sans version + type faible ou undefined
				if (!strchr(cand->symbol, '@') &&
				strcmp(cand->symbol, base) == 0 &&
				(cand->letter == 'W' || cand->letter == 'w' || cand->letter == 'U') &&
				cand->trash == 0)
				{
					cand->trash = 1;
					break;
				}
				cand = cand->next;
			}
			free(base);
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

int main(int ac, char **av) 
{
	static t_nm nm = {0};
	
	if (ac < 2)
		return(0);
	if (!parse_args(ac, av, &nm))
		return(0);
	if (!check_elf(&nm))
		return(0);
	if (!parse_elf(&nm))
		return(0);
	if (!info_clean(&nm))
		return(0);
	ft_resprint(nm.res);
}




/*
-a     Display all symbol table entries, including those inserted for use by debuggers.
-g     Display only global (external) symbols.
-p     Don't sort; display in symbol-table order.
-u     Display only undefined symbols.
-r     Sort in reverse order.
	remove unauthorised functions
*/
/*
- check the case of adding char to the binary
*/