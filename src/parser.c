#include "ft_nm.h"

char get_symbol_letter32(Elf32_Sym sym, Elf32_Shdr *sections)
{
    unsigned char bind = ELF32_ST_BIND(sym.st_info);
    unsigned char type = ELF32_ST_TYPE(sym.st_info);
    // unsigned char visibility = ELF32_ST_VISIBILITY(sym.st_other);
    Elf32_Half shndx = sym.st_shndx;

    if (shndx == SHN_UNDEF)
        return (bind == STB_WEAK) ? ((type == STT_OBJECT) ? 'v' : 'w') : 'U';

    if (shndx == SHN_ABS)
        return (bind == STB_LOCAL) ? 'a' : 'A';

    if (shndx == SHN_COMMON)
        return 'C';

    if (bind == STB_WEAK)
        return (type == STT_OBJECT) ? 'V' : 'W';

    // if (type == STT_OBJECT && bind == STB_GLOBAL)
    //     return 'D';

    // if (visibility == STV_HIDDEN && shndx != SHN_UNDEF)
    //     return 'h';

    Elf32_Shdr section = sections[shndx];
    Elf32_Word flags = section.sh_flags;
    Elf32_Word shtype = section.sh_type;

    if (shtype == SHT_NOBITS && (flags & SHF_ALLOC) && (flags & SHF_WRITE))
        return (bind == STB_LOCAL) ? 'b' : 'B';

    if ((flags & SHF_ALLOC) && (flags & SHF_WRITE))
        return (bind == STB_LOCAL) ? 'd' : 'D';

    if ((flags & SHF_ALLOC) && (flags & SHF_EXECINSTR))
        return (bind == STB_LOCAL) ? 't' : 'T';

    if ((flags & SHF_ALLOC))
        return (bind == STB_LOCAL) ? 'r' : 'R';

    return '?';
}

char get_symbol_letter64(Elf64_Sym sym, Elf64_Shdr *sections) {
    unsigned char bind = ELF64_ST_BIND(sym.st_info);
    unsigned char type = ELF64_ST_TYPE(sym.st_info);
    Elf64_Half shndx = sym.st_shndx;
    char c = '?';

    // Cas des symboles faibles (weak)
    if (bind == STB_WEAK) {
        if (shndx == SHN_UNDEF)
            return (type == STT_OBJECT) ? 'v' : 'w';
        else
            return (type == STT_OBJECT) ? 'V' : 'W';
    }

    // Cas des symboles particuliers
    if (shndx == SHN_UNDEF)
        return 'U';
    if (shndx == SHN_ABS)
        return 'a';
    if (shndx == SHN_COMMON)
        return 'C';

    // Section invalide
    if (shndx >= SHN_LORESERVE)
        return '?';

    // Section associée
    Elf64_Shdr sec = sections[shndx];
    if (sec.sh_type == SHT_NOBITS && (sec.sh_flags & SHF_ALLOC) && (sec.sh_flags & SHF_WRITE))
        c = 'B'; // .bss
    else if (sec.sh_flags & SHF_EXECINSTR)
        c = 'T'; // .text
    else if ((sec.sh_flags & SHF_ALLOC) && (sec.sh_flags & SHF_WRITE))
        c = 'D'; // .data
    else if (sec.sh_flags & SHF_ALLOC)
        c = 'R'; // .rodata ou autre section readonly

    // Minuscule si symbole local
    if ((bind == STB_LOCAL || bind == STB_WEAK) && c != '?')
    c = tolower(c);

    return c;
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