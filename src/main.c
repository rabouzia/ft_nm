#include "ft_nm.h"
#include <ctype.h>

int parse_elf(t_nm *nm, char *av)
{
	if (!nm || !nm->fdata)
		return 0;

	nm->elf.is_64 = (((unsigned char *)nm->fdata)[EI_CLASS] == ELFCLASS64);
	nm->elf.ehdr = nm->fdata;

	if (nm->elf.is_64)
	{
		Elf64_Ehdr *ehdr = (Elf64_Ehdr *)nm->fdata;
		Elf64_Shdr *shdr = (Elf64_Shdr *)((char *)nm->fdata + ehdr->e_shoff);
		nm->elf.shdr = shdr;
		if (ehdr->e_shstrndx >= ehdr->e_shnum) return 0;
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
					char *name = ft_strdup(nm->elf.strtab + sym->st_name);
					if (!name)
						continue;
					char type = get_symbol_letter64(*sym, (Elf64_Shdr *)nm->elf.shdr);
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
		Elf32_Shdr *shdr = (Elf32_Shdr *)((char *)nm->fdata + ehdr->e_shoff);
		nm->elf.shdr = shdr;
		if (ehdr->e_shstrndx >= ehdr->e_shnum) return 0;
		// char *shstrtab = (char *)nm->fdata + shdr[ehdr->e_shstrndx].sh_offset;

		for (int i = 0; i < ehdr->e_shnum; i++)
		{
			if (shdr[i].sh_type == SHT_SYMTAB || shdr[i].sh_type == SHT_DYNSYM)
			{
				Elf32_Sym *symtab = (Elf32_Sym *)((char *)nm->fdata + shdr[i].sh_offset);
				int symcount = shdr[i].sh_size / shdr[i].sh_entsize;
				Elf32_Shdr *strtab_hdr = &shdr[shdr[i].sh_link];
				char *strtab = (char *)nm->fdata + strtab_hdr->sh_offset;

				for (int j = 0; j < symcount; j++)
				{
					Elf32_Sym *sym = &symtab[j];
					if (!sym->st_name) continue;
					char *name = ft_strdup(strtab + sym->st_name);
					if (!name)
						continue;
					char type = get_symbol_letter32(*sym, (Elf32_Shdr *)nm->elf.shdr);
					if (!nm->opt.a && (name[0] == '\0' || name[0] == '$')) continue;
					if (ELF32_ST_TYPE(sym->st_info) == STT_SECTION || ELF32_ST_TYPE(sym->st_info) == STT_FILE) continue;
					if (nm->opt.g && ELF32_ST_BIND(sym->st_info) != STB_GLOBAL) continue;
					if (nm->opt.u && sym->st_shndx != SHN_UNDEF) continue;
					ft_resaddback(&nm->res, ft_resnew(name, sym->st_value, type));
				}
			}
		}
	}
	nm->res->filename = ft_strdup(av);
	if (!nm->res->filename)
		return 0;
	return 1;
}

int main(int ac, char **av) 
{
	static t_nm nm = {0};
	if (ac > ARG_MAX || ac < 2)
		return(0);
	int i = 1;
	nm.is_opt = check_opt(av[i], &nm.opt);
	if (nm.is_opt)
	    i = 2;
	while(av[i])
	{
		if (!check_elf(&nm, av[i]))
			return(0);
		if (!parse_elf(&nm, av[i]))
			return(0);
		if (!info_clean(&nm))
			return(0);
		ft_resprint(&nm, ac);
		ft_clean(&nm);
		i++;
	}
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