#include "ft_nm.h"


char get_symbol_letter(Elf64_Sym sym, char **section_names, int section_count)
{
    unsigned char bind = ELF64_ST_BIND(sym.st_info);
    unsigned char type = ELF64_ST_TYPE(sym.st_info);
    Elf64_Half shndx = sym.st_shndx;

    if (shndx == SHN_UNDEF) return 'U';
    if (shndx == SHN_ABS) return 'A';
    if (shndx == SHN_COMMON) return 'C';
    if (bind == STB_WEAK) return (type == STT_OBJECT) ? 'v' : 'w';

    if (shndx >= section_count || section_names[shndx] == NULL)
        return '?';

    const char *secname = section_names[shndx];

    if (strcmp(secname, ".text") == 0)
        return (bind == STB_LOCAL) ? 't' : 'T';
    if (strcmp(secname, ".data") == 0)
        return (bind == STB_LOCAL) ? 'd' : 'D';
    if (strcmp(secname, ".bss") == 0)
        return (bind == STB_LOCAL) ? 'b' : 'B';

    return (bind == STB_LOCAL) ? 'n' : 'N';
}

void parse_args(int ac, char **av, t_opt *opts) 
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
                    case 'a': opts->opt_a = 1; break;
                    case 'r': opts->opt_r = 1; break;
                    case 'g': opts->opt_g = 1; break;
                    case 'u': opts->opt_u = 1; break;
                    case 'p': opts->opt_p = 1; break;
                    default:
                        fprintf(stderr, "Erreur : option inconnue -%c\n", av[i][j]);
                        exit(EXIT_FAILURE);
                }
            }
        } 
		else if (!file_set) 
		{
            opts->filename = av[i];
            file_set = 1;
        } 
		else 
		{
            fprintf(stderr, "Erreur : trop de fichiers spécifiés.\n");
            exit(EXIT_FAILURE);
        }
    }

    if (!opts->filename) {
        fprintf(stderr, "Erreur : aucun fichier spécifié.\n");
        exit(EXIT_FAILURE);
    }
}


int strcmp_ignore_first_underscore(const char* a, const char* b)
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
void ft_nmsort(t_nm* head) 
{
    if (!head) return;
    int swapped;
    t_nm* ptr;
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

int main(int ac, char **av) {
	t_nm *nm = NULL;
	
	ft_nmprint(nm);
	if (ac < 2 )
		return(dprintf(2, ARG_ERR));
	parse_args(ac, av, &nm->options);
	// if (!is_valid_option()) //-a -u or -au 
	// {
	// 		if (-r)
	// 			print_reverse();
	// 		if (-u)
	// 			print_undefined_only();
	// 		if (-p)
	// 			print_nosort(); // jai pas capter
	// 		if (-a)
	// }
	// int i =0;
	// while(strcmp(av[i][0],"-"))
	// 	i++;

	int fd = open(av[1], O_RDONLY);
    if (fd == -1) 
		return(dprintf(2, OP_ERR));
	
	struct stat st;
	
	if (fstat(fd, &st) < 0) 
		return(close(fd), dprintf(2, FSTAT_ERR));

	size_t file_size = st.st_size;
	printf("%zu", file_size);
	
	void *file_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (file_data == MAP_FAILED) 
		return(close(fd), dprintf(2, MMAP_ERR));

	// int i = 0;
	// while(file_data[i])
		// printf("%s\n", (char *) file_data);

	Elf64_Ehdr *elf_header = (Elf64_Ehdr *)file_data;

	if (memcmp(elf_header->e_ident, ELFMAG, SELFMAG) != 0) 
	{
		fprintf(stderr, "Not an ELF file\n");
		munmap(file_data, file_size);
		close(fd);
		exit(1);
	}
	Elf64_Sym *symtab = NULL; 
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)file_data;
	Elf64_Shdr *section_headers = (Elf64_Shdr *)((char *)file_data + ehdr->e_shoff);
	
	char **section_names = malloc(sizeof(char *) * ehdr->e_shnum);
	
	for (int i = 0; i < elf_header->e_shnum; i++) {
		Elf64_Shdr *sh = &section_headers[i];
		char *shstrtab = file_data + section_headers[ehdr->e_shstrndx].sh_offset;
		section_names[i] = shstrtab + sh->sh_name;
		if (sh->sh_type == SHT_SYMTAB || sh->sh_type == SHT_DYNSYM) {
			symtab = (Elf64_Sym *)((char *)file_data + sh->sh_offset);
			int num_syms = sh->sh_size / sh->sh_entsize;
	
			Elf64_Shdr *strtab_hdr = &section_headers[sh->sh_link];
			char *strtab = (char *)file_data + strtab_hdr->sh_offset;
	
			for (int j = 0; j < num_syms; j++) {
				if (symtab[j].st_name != 0) {

					// printf("Symbol : %hu\n",symtab[j].st_shndx);
					ft_nmaddback(&nm, ft_nmnew(strtab + symtab[j].st_name, symtab[j].st_value, get_symbol_letter(symtab[j], section_names, ehdr->e_shnum)));
					// if ()
				}
				// nm = nm->next;
		}
	}
	
	// get_flag(nm);
	
	// ft_nmdouble(nm);
	ft_nmsort(nm);
	

	ft_nmprint(nm);
	
	ft_nmclear(&nm);
}
}
