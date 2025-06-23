#include "ft_nm.h"

char get_symbol_letter(Elf64_Sym sym, Elf64_Shdr *sections) {
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

int parse_args(int ac, char **av, t_nm *nm) 
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
                    case 'a': nm->opt.a = 1; break;
                    case 'r': nm->opt.r = 1; break;
                    case 'g': nm->opt.g = 1; break;
                    case 'u': nm->opt.u = 1; break;
                    case 'p': nm->opt.p = 1; break;
                    default:
                        return (ft_clean(nm, UKN_OPT), 0);
                }
            }
        } 
		else if (!file_set) 
		{
            nm->opt.filename = ft_strdup(av[i]);
            file_set = 1;
        } 
		else 
			return (ft_clean(nm, TOO_MANY_FILES), 0);
    }
	if (!nm->opt.filename)
        return (ft_clean(nm, NO_FILE), 0);
	
	return 1;
}