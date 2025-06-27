#include "ft_nm.h"

char get_symbol_letter32(Elf32_Sym sym, Elf32_Shdr *sections)
{
    unsigned char bind = ELF32_ST_BIND(sym.st_info);
    unsigned char type = ELF32_ST_TYPE(sym.st_info);
    Elf32_Half shndx = sym.st_shndx;

    if (ELF32_ST_TYPE(sym.st_info) == STT_GNU_IFUNC)
        return 'i';

    if (bind == STB_WEAK) {
        if (shndx == SHN_UNDEF)
            return (type == STT_OBJECT) ? 'v' : 'w';
        else
            return (type == STT_OBJECT) ? 'V' : 'W';
    }

    if (shndx == SHN_UNDEF)
        return 'U';
    if (shndx == SHN_ABS)
        return (bind == STB_LOCAL) ? 'a' : 'A';
    if (shndx == SHN_COMMON)
        return 'C';
    if (shndx >= SHN_LORESERVE)
        return '?';

    Elf32_Shdr sec = sections[shndx];
    Elf32_Word flags = sec.sh_flags;
    Elf32_Word type_sec = sec.sh_type;

    if (type_sec == SHT_NOBITS && (flags & SHF_ALLOC) && (flags & SHF_WRITE))
        return (bind == STB_LOCAL) ? 'b' : 'B';
    if ((flags & SHF_ALLOC) && (flags & SHF_WRITE))
        return (bind == STB_LOCAL) ? 'd' : 'D';
    if (flags & SHF_EXECINSTR)
        return (bind == STB_LOCAL) ? 't' : 'T';
    if ((flags & SHF_ALLOC) && !(flags & SHF_WRITE)) {
        if (type == STT_OBJECT && bind == STB_LOCAL)
            return 'r';
        return (bind == STB_LOCAL) ? 'r' : 'R';
    }

    return '?';
}

char get_symbol_letter64(Elf64_Sym sym, Elf64_Shdr *sections)
{
    unsigned char bind = ELF64_ST_BIND(sym.st_info);
    unsigned char type = ELF64_ST_TYPE(sym.st_info);
    Elf64_Half shndx = sym.st_shndx;

    if (ELF64_ST_TYPE(sym.st_info) == STT_GNU_IFUNC)
        return 'i';

    if (bind == STB_WEAK) {
        if (shndx == SHN_UNDEF)
            return (type == STT_OBJECT) ? 'v' : 'w';
        else
            return (type == STT_OBJECT) ? 'V' : 'W';
    }

    if (shndx == SHN_UNDEF)
        return 'U';
    if (shndx == SHN_ABS)
        return (bind == STB_LOCAL) ? 'a' : 'A';
    if (shndx == SHN_COMMON)
        return 'C';
    if (shndx >= SHN_LORESERVE)
        return '?';

    Elf64_Shdr sec = sections[shndx];
    Elf64_Xword flags = sec.sh_flags;
    Elf64_Word type_sec = sec.sh_type;

    if (type_sec == SHT_NOBITS && (flags & SHF_ALLOC) && (flags & SHF_WRITE))
        return (bind == STB_LOCAL) ? 'b' : 'B';
    if ((flags & SHF_ALLOC) && (flags & SHF_WRITE))
        return (bind == STB_LOCAL) ? 'd' : 'D';
    if (flags & SHF_EXECINSTR)
        return (bind == STB_LOCAL) ? 't' : 'T';
    if ((flags & SHF_ALLOC) && !(flags & SHF_WRITE)) {
        if (type == STT_OBJECT && bind == STB_LOCAL)
            return 'r';
        return (bind == STB_LOCAL) ? 'r' : 'R';
    }

    return '?';
}

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
		{
			fprintf(stderr,NO_SYMBOLS, av);
			ft_end(nm, 0);
		}		
	
		if ((size_t)ehdr->e_shoff + ehdr->e_shnum * sizeof(Elf64_Shdr) > nm->fsize)
		{		fprintf(stderr,CORRUPT_MSG1, av);
				fprintf(stderr,CORRUPT_MSG2, av);
				ft_end(nm, 0);
		}
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
		{
			fprintf(stderr,NO_SYMBOLS, av);
			ft_end(nm, 0);
		}
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
					}					
						if (nm->opt.u && sym->st_shndx != SHN_UNDEF) { free(name); continue; }

								ft_resaddback(&nm->res, ft_resnew(name, sym->st_value, type));
							}
					}
		}
	}

	if (!nm->res)
		{
			fprintf(stderr,NO_SYMBOLS, av);
			ft_end(nm, 0);
		}
	nm->res->filename = ft_strdup(av);
	if (!nm->res->filename)
		return 0;
	return 1;
}