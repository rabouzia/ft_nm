#include "ft_nm.h"
#include <ctype.h>

char get_symbol_letter(Elf64_Sym sym, Elf64_Shdr *sections) {
    unsigned char bind = ELF64_ST_BIND(sym.st_info);
    unsigned char type = ELF64_ST_TYPE(sym.st_info);
    Elf64_Half shndx = sym.st_shndx;

    if (shndx == SHN_UNDEF)
        return 'U';
    if (shndx == SHN_ABS)
        return 'A';
    if (shndx == SHN_COMMON)
        return 'C';
    if (bind == STB_WEAK)
	{
        return (type == STT_OBJECT) ? 'v' : 'w';
    }
    if (shndx == SHN_UNDEF || shndx >= SHN_LORESERVE)
        return '?';
    Elf64_Shdr sec = sections[shndx];
    char c = '?';
    if (sec.sh_type == SHT_NOBITS && (sec.sh_flags & SHF_ALLOC) && (sec.sh_flags & SHF_WRITE))
        c = 'B'; // .bss
    else if (sec.sh_flags & SHF_EXECINSTR)
        c = 'T'; // .text
    else if ((sec.sh_flags & SHF_ALLOC) && (sec.sh_flags & SHF_WRITE))
        c = 'D'; // .data
    else if (sec.sh_flags & SHF_ALLOC)
        c = 'R'; // .rodata or other readonly sections
    if (bind == STB_LOCAL && c != '?')
        c = tolower(c);
    return c;
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

void parse_args(int ac, char **av, t_opt *opt) 
{
    int file_set = 0;

    for (int i = 1; i < ac; i++) 
	{
        if (av[i][0] == '-') 
		{
            for (int j = 1; av[i][j]; j++)
			{
                switch (av[i][j])
				{
                    case 'a': opt->a = 1; break;
                    case 'r': opt->r = 1; break;
                    case 'g': opt->g = 1; break;
                    case 'u': opt->u = 1; break;
                    case 'p': opt->p = 1; break;
                    default:
                        fprintf(stderr, "Erreur : option inconnue -%c\n", av[i][j]);
                        exit(EXIT_FAILURE);
                }
            }
        } 
		else if (!file_set) 
		{
			printf("%s\n",av[i]);
            opt->filename = ft_strdup(av[i]);
            file_set = 1;
        } 
		else 
		{
            fprintf(stderr, "Erreur : trop de fichiers spécifiés.\n");
            exit(EXIT_FAILURE);
        }
    }

    if (!opt->filename) {
        fprintf(stderr, "Erreur : aucun fichier spécifié.\n");
        exit(EXIT_FAILURE);
    }
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
void ft_nmsort(t_res* head) 
{
    if (!head) return;
    int swapped;
    t_res* ptr;
    do {
        swapped = 0;
        ptr = head;
        while (ptr->next) 
		{
            if (ignore_underscore(ptr->symbol, ptr->next->symbol) > 0) 
			{
                char* tmp = ptr->symbol;
                ptr->symbol = ptr->next->symbol;
                ptr->next->symbol = tmp;
                swapped = 1;
            }
            ptr = ptr->next;
        }
    } while (swapped);
}

/*
-a     Display all symbol table entries, including those inserted for use by debuggers.
-g     Display only global (external) symbols.
-p     Don't sort; display in symbol-table order.
-u     Display only undefined symbols.
-r     Sort in reverse order.
	remove unauthorised functions
*/



int parse_elf(t_nm * nm)
{
	nm->symtab = NULL; 
	nm->ehdr = (Elf64_Ehdr *)nm->fdata;
	nm->section_headers = (Elf64_Shdr *)((char *)nm->fdata + nm->ehdr->e_shoff);
	nm->section_names = malloc(sizeof(char *) * nm->ehdr->e_shnum);
	
	for (int i = 0; i < nm->ehdr ->e_shnum; i++)
	{
		nm->sh = &nm->section_headers[i];
		char *shstrtab = nm->fdata + nm->section_headers[nm->ehdr->e_shstrndx].sh_offset;
		nm->section_names[i] = shstrtab + nm->sh->sh_name;
		if (nm->sh->sh_type == SHT_SYMTAB || nm->sh->sh_type == SHT_DYNSYM) 
		{
			nm->symtab = (Elf64_Sym *)((char *)nm->fdata + nm->sh->sh_offset);
			int num_syms = nm->sh->sh_size / nm->sh->sh_entsize;
			Elf64_Shdr *strtab_hdr = &nm->section_headers[nm->sh->sh_link];
			char *strtab = (char *)nm->fdata + strtab_hdr->sh_offset;
	
			for (int j = 0; j < num_syms; j++)
			{
				if (nm->symtab[j].st_name != 0)
				{
					char type = get_symbol_letter(nm->symtab[j], nm->section_headers);
					char *name = strtab + nm->symtab[j].st_name;
					if (!nm->opt.a)
					{
						if (name[0] == '\0') continue;
						if (name[0] == '$') continue;
					}
					if (nm->opt.g && ELF64_ST_BIND(nm->symtab[j].st_info) != STB_GLOBAL)
						continue;
					if (nm->opt.u && nm->symtab[j].st_shndx != SHN_UNDEF)
						continue;
					ft_resaddback(&nm->res, ft_resnew(name, nm->symtab[j].st_value, type));
				}
			}
		}
	}
	return 1;
}


int check_elf(t_nm *nm)
{
	int fd = open(nm->opt.filename, O_RDONLY);
    if (fd == -1) 
		return(dprintf(2, OP_ERR));
	
	struct stat st;
	
	if (fstat(fd, &st) < 0) 
		return(close(fd), dprintf(2, FSTAT_ERR));

	size_t file_size = st.st_size;
	
	nm->fdata = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (nm->fdata == MAP_FAILED) 
		return(close(fd), dprintf(2, MMAP_ERR));

	nm->elf_hd = (Elf64_Ehdr *)nm->fdata;

	if (memcmp(nm->elf_hd->e_ident, ELFMAG, SELFMAG) != 0) 
	{
		fprintf(stderr, "Not an ELF file\n");
		munmap(nm->fdata, file_size);
		close(fd);
		exit(1);
	}
	return 1;
}

void clean_double(t_res *res)
{
	t_res *tmp;
	while (res)
	{
		tmp = res->next;
		while(tmp)
		{	
			if (ft_strncmp(res->symbol, tmp->symbol, strlen(res->symbol)) == 0)
				remove_node(res, tmp->symbol);
			res = res->next;
		}
		res = res->next;
	}
	
}

int main(int ac, char **av) 
{
	static t_nm nm = {0};
	
	if (ac < 2)
		return(dprintf(2, ARG_ERR));
	parse_args(ac, av, &nm.opt);
	check_elf(&nm);
	parse_elf(&nm);
	clean_double(nm.res);
	ft_resprint(nm.res);
	ft_resclear(&nm.res);
	free(nm.opt.filename);	
}




/*

*/