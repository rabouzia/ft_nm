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
		if (ehdr->e_shnum == 0 || ehdr->e_shoff == 0 || ehdr->e_shentsize == 0)
			not_elf(nm, av);
		nm->elf.shdr = shdr;
		if (ehdr->e_shstrndx >= ehdr->e_shnum)
			ft_sym(nm, av);
		if ((size_t)ehdr->e_shoff + ehdr->e_shnum * sizeof(Elf64_Shdr) > nm->fsize)
			ft_corrupt(nm, av);
		for (int i = 0; i < ehdr->e_shnum; i++)
		{
			if (shdr[i].sh_type == SHT_SYMTAB)
			{
				nm->elf.symtab = (void *)((char *)nm->fdata + shdr[i].sh_offset);
				nm->elf.symbols_nb = shdr[i].sh_size / shdr[i].sh_entsize;
				nm->elf.strtab_section = &shdr[shdr[i].sh_link];
				nm->elf.strtab = (char *)nm->fdata + ((Elf64_Shdr *)nm->elf.strtab_section)->sh_offset;

				for (size_t j = 0; j < nm->elf.symbols_nb; j++)
				{
					Elf64_Sym *sym = &((Elf64_Sym *)nm->elf.symtab)[j];
					if (!sym->st_name)
						continue;

					const char *symname = nm->elf.strtab + sym->st_name;
					if (!symname || symname[0] == '\0')
						continue;

					char *name = ft_strdup(symname);
					if (!name)
						continue;

					if (strncmp(name, "__PRETTY_FUNCTION__", 19) == 0 &&
						(ELF64_ST_TYPE(sym->st_info) == STT_NOTYPE || ELF64_ST_TYPE(sym->st_info) == STT_OBJECT) &&
						sym->st_shndx == SHN_ABS)
					{
						free(name);
						continue;
					}

					char type = get_symbol_letter64(*sym, (Elf64_Shdr *)nm->elf.shdr);
					if (!nm->opt.a && (name[0] == '\0' || name[0] == '$')) { free(name); continue; }
					if (ELF64_ST_TYPE(sym->st_info) == STT_SECTION || ELF64_ST_TYPE(sym->st_info) == STT_FILE) { free(name); continue; }
					if (nm->opt.g) {
	unsigned char bind = ELF64_ST_BIND(sym->st_info);
	if (bind != STB_GLOBAL && bind != STB_WEAK) {
		free(name);
		continue;
	}
}
					if (nm->opt.u && sym->st_shndx != SHN_UNDEF) { free(name); continue; }

					ft_resaddback(&nm->res, ft_resnew(name, sym->st_value, type));
				}
			}
		}
	}
	else
	{
		Elf32_Ehdr *ehdr = (Elf32_Ehdr *)nm->fdata;
		Elf32_Shdr *shdr = (Elf32_Shdr *)((char *)nm->fdata + ehdr->e_shoff);
		if (ehdr->e_shnum == 0 || ehdr->e_shoff == 0 || ehdr->e_shentsize == 0)
			not_elf(nm, av);
		nm->elf.shdr = shdr;
		if (ehdr->e_shstrndx >= ehdr->e_shnum)
			return 0;
		if ((size_t)ehdr->e_shoff + ehdr->e_shnum * sizeof(Elf32_Shdr) > nm->fsize)
			ft_sym(nm, av);
		for (int i = 0; i < ehdr->e_shnum; i++)
		{
			if (shdr[i].sh_type == SHT_SYMTAB)
			{
				Elf32_Sym *symtab = (Elf32_Sym *)((char *)nm->fdata + shdr[i].sh_offset);
				int symcount = shdr[i].sh_size / shdr[i].sh_entsize;
				Elf32_Shdr *strtab_hdr = &shdr[shdr[i].sh_link];
				char *strtab = (char *)nm->fdata + strtab_hdr->sh_offset;

				for (int j = 0; j < symcount; j++)
				{
					Elf32_Sym *sym = &symtab[j];
					if (!sym->st_name)
						continue;

					const char *symname = strtab + sym->st_name;
					if (!symname || symname[0] == '\0')
						continue;

					if (strncmp(symname, "__PRETTY_FUNCTION__", 19) == 0 &&
						(ELF32_ST_TYPE(sym->st_info) == STT_NOTYPE || ELF32_ST_TYPE(sym->st_info) == STT_OBJECT) &&
						sym->st_shndx == SHN_ABS)
						continue;

					char *name = ft_strdup(symname);
					if (!name)
						continue;

					char type = get_symbol_letter32(*sym, (Elf32_Shdr *)nm->elf.shdr);
					if (!nm->opt.a && (name[0] == '\0' || name[0] == '$')) { free(name); continue; }
					if (ELF32_ST_TYPE(sym->st_info) == STT_SECTION || ELF32_ST_TYPE(sym->st_info) == STT_FILE) { free(name); continue; }
if (nm->opt.g) {
	unsigned char bind = ELF64_ST_BIND(sym->st_info);
	if (bind != STB_GLOBAL && bind != STB_WEAK) {
		free(name);
		continue;
	}
}					if (nm->opt.u && sym->st_shndx != SHN_UNDEF) { free(name); continue; }

					ft_resaddback(&nm->res, ft_resnew(name, sym->st_value, type));
				}
			}
		}
	}

	if (!nm->res)
		ft_sym(nm, av);
	nm->res->filename = ft_strdup(av);
	if (!nm->res->filename)
		return 0;
	return 1;
}
#define NO_AOUT "nm: 'a.out': No such file\n"
#define NO_F "nm: '%s': No such file\n"

int main(int ac, char **av) 
{
	static t_nm nm = {0};
	int i = 1;
	if (ac >= 2)
	{
		nm.is_opt = check_opt(av[i], &nm.opt);
		if (nm.is_opt)
			i++;
	}
	if (ac - nm.is_opt > ARG_MAX)
		return (ft_printf("nm: Too many arguments\n"), 0);
	if (ac < 2 || (nm.is_opt && ac < 3))
		{
			int fd = open("a.out", O_RDONLY);
			if (fd == -1)
				return (ft_printf(NO_AOUT), 0);
				// perror();
			// ft_printf("%d\n", fd);
			close(fd);
			if (!check_elf(&nm, "a.out") || !parse_elf(&nm, "a.out") || !info_clean(&nm))
				return (0);
			ft_resprint(&nm, 2);
			ft_clean(&nm);
			return (0);
		}
	while(av[i])
	{
		if (!check_elf(&nm, av[i]))
			return(0);
		if (!parse_elf(&nm, av[i]))
			return(0);
		if (!info_clean(&nm))
			return(0);
		// ft_resprint(nm);
		// if (nm.opt.g)
		// 	print_maj(&nm);
		// else
		ft_resprint(&nm, ac);
		free(nm.res->filename);
		ft_clean(&nm);
		nm = (t_nm){0};
		i++;
	}
}



/*
	remplacer 
	- les printf par ft_printf
	- les strchr par ft_strchr
	- les strcmp par ft_strcmp
	- les strncmp par ft_strncmp
*/