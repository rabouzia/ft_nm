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



int check_opt(char *av, t_opt *opt)
{
	if (av[0] == '-' && av[1] != '\0') 
	{
		if (av[1] == 'a')
			opt->a = true;
		else if (av[1] == 'r')
			opt->r = true;
		else if (av[1] == 'g')
			opt->g = true;	
		else if (av[1] == 'u')
			opt->u = true;
		else if (av[1] == 'p')
			opt->p = true;
	} 
	else
		return 0;
	return 1; // Options valides
}