#include "ft_nm.h"


char get_symbol_letter(Elf64_Sym sym, char **section_names, int section_count) {
    unsigned char bind = ELF64_ST_BIND(sym.st_info);
    unsigned char type = ELF64_ST_TYPE(sym.st_info);
    Elf64_Half shndx = sym.st_shndx;

    if (shndx == SHN_UNDEF) return 'U';
    if (shndx == SHN_ABS) return 'A';
    if (shndx == SHN_COMMON) return 'C';
    if (bind == STB_WEAK) return (type == STT_OBJECT) ? 'v' : 'w';

    if (shndx >= section_count || section_names[shndx] == NULL)
        return '?';  // invalid section

    const char *secname = section_names[shndx];

    if (strcmp(secname, ".text") == 0)
        return (bind == STB_LOCAL) ? 't' : 'T';
    if (strcmp(secname, ".data") == 0)
        return (bind == STB_LOCAL) ? 'd' : 'D';
    if (strcmp(secname, ".bss") == 0)
        return (bind == STB_LOCAL) ? 'b' : 'B';

    return (bind == STB_LOCAL) ? 'n' : 'N'; // fallback
}


int main(int ac, char **av) {
	t_nm *nm = NULL;
	
	ft_nmprint(nm);
	if (ac != 2)
		return(dprintf(2, ARG_ERR));

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
	
	// for (int i = 0; i < ehdr->e_shnum; i++) 
	// {
	// 	Elf64_Shdr *sh = &section_headers[i];
	// 	printf("Section %d offset: %ld, size: %ld\n", i, sh->sh_offset, sh->sh_size);
	// 	for (int i = 0; i < ehdr->e_shnum; i++){
	// 	if (section_headers[i].sh_type == SHT_SYMTAB) {
	// 		symtab = (Elf64_Sym *)((char *)file_data + section_headers[i].sh_offset);
	// 		break; }
	// 	}
	// // }

	// Elf64_Shdr *section_headers = (Elf64_Shdr *)((char *)file_data + elf_header->e_shoff);
    // Elf64_Sym *symtab = NULL;  // Declare symtab here

    // Check all section headers
    // for (int i = 0; i < elf_header->e_shnum; i++) {
    //     Elf64_Shdr *sh = &section_headers[i];
    //     printf("Section %d: type = %u, offset = %ld, size = %ld\n", i, sh->sh_type, sh->sh_offset, sh->sh_size);

    //     // Look for the symbol table (SHT_SYMTAB or SHT_DYNSYM)
    //     // if (sh->sh_type == SHT_SYMTAB || sh->sh_type == SHT_DYNSYM) {
    //         symtab = (Elf64_Sym *)((char *)file_data + sh->sh_offset);
    //         printf("Found symbol table of type %u at offset %ld\n", sh->sh_type, sh->sh_offset);
    //         // break; // Exit after finding the symbol table
    //     // }
	// 	Elf64_Shdr *strtab_header = &section_headers[elf_header->e_shstrndx];
	// 	char *string_table = (char *)file_data + strtab_header->sh_offset;
		
	// 	printf("%s\n", string_table);
		// for (int i = 0; symtab[i].st_name != 0; i++) {
		// 	// Print symbol name using the string table
		// 	printf("Symbol %d: %s\n", i, string_table + symtab[i].st_name);
		// }
    // }

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
				
				}
				// nm = nm->next;
		}
	}
	
	get_flag(nm);
	ft_nmprint(nm);
	
	ft_nmclear(&nm);
}
}

/*
	
*/