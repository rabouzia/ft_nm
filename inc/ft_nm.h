#pragma once


//################ LIB ################
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h> // munmap, mmap
#include <fcntl.h>    // open
#include <sys/stat.h> // fstat
#include <elf.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

//################ STR MACRO ################

#define OP_ERR "Failed to open file\n"
#define ARG_ERR "Usage ./ft_nm [-flag] <binary>\n"
#define FSTAT_ERR "fstat error\n"
#define MMAP_ERR "mmap error\n"
#define NO_ELF "nm: %s: file format not recognized\n" 
#define CHECK_ERR "Error: Invalid ELF file\n"
#define NO_FILE "Erreur : aucun fichier spécifié.\n"
#define UKN_OPT "Erreur : option inconnue.\n"
#define PARSE_ERR "parse err"
#define ELF_ERR ""
#define ARG_MAX 2097152
#define CORRUPT_MSG1 "bfd plugin: %s: file too short\n"
#define CORRUPT_MSG2 "nm: %s: file format not recognized\n"
#define NO_SYMBOLS "nm: %s: no symbols\n"
#define NOT_REG_ERR "nm: not a regular file\n"
#define NO_RECOG "nm: %s: file format not recognized\n"

//################ FCT MACRO ################

#define CHECK_OFFSET(ptr, offset) \
	if ((char *) ptr + offset > (char* ) nm->fdata + nm->fsize) { \
		fprintf(stderr, "Error: offset out of bounds\n"); \
		exit(EXIT_FAILURE); \
	}

//################ STRUCT ################

typedef struct s_res
{
	char *filename;
    char letter;
    Elf64_Addr addr;
    char *symbol;
    bool defined;
  	bool trash;
	struct  s_res *next;
}               t_res;

typedef struct s_opt
{
	bool a;
	bool r;
	bool g;
	bool u;
	bool p;
}			 t_opt;

typedef struct s_elf
{
	void *ehdr;          // Elf32_Ehdr ou Elf64_Ehdr
	void *shdr;          // Elf32_Shdr ou Elf64_Shdr
	void *symtab;        // Elf32_Sym ou Elf64_Sym
	void *strtab_section;
	char *strtab;
	int symtab_index;
	size_t symbols_nb;
	int is_64;
} t_elf;

typedef struct s_nm
{
	int ac;
	char **av;
	int fd;
	char *name;
	size_t fsize;
	t_res *res;
	bool is_opt;
	t_opt opt;
	t_elf elf;
	void *fdata;
}			t_nm;



//##################### PARSER #################

char get_symbol_letter64(Elf64_Sym sym, Elf64_Shdr *sections);
char get_symbol_letter32(Elf32_Sym sym, Elf32_Shdr *sections);
bool info_clean(t_nm *nm);
int check_elf(t_nm *nm, char *av);
void ft_sym(t_nm *nm, char *msg);
void not_elf(t_nm *nm,char *filename);
int ignore_underscore(const char* a, const char* b);
void print_reverse(t_nm * nm);
int ft_reslen(t_res *res);
void print_maj(t_nm *nm);

//##################### LST UTILS #################

int	    search_res(t_res *res, char *key);
void 	get_flag(t_res *res);
bool	delete_node(t_res **res, char *to_delete);
void	ft_resaddback(t_res **head, t_res *new);
t_res	*ft_resnew(char *symbol, Elf64_Addr addr, char c);
t_res	*ft_reslast(t_res *head);
void    ft_check_same(t_res **res);
void	ft_resclear(t_res **res);
void    ft_resprint(t_nm *res, int ac);
void	free_node(t_res *res);
void	remove_node(t_res *res, char *to_delete);
void	remove_last(t_res *res);
void	remove_first(t_res **res);
void 	ft_nmsort(t_nm *nm, t_res* head);
char	*ft_strdup(const char *src);
void ft_end(t_nm *nm, char *msg);
void ft_clean(t_nm *nm);
int check_opt(char *av, t_opt *opt);
void ft_corrupt(t_nm *nm, char *msg);

//##################### LIBRARY #################

int	ft_strncmp(const char *s1, const char *s2, size_t n);

// struct stat {
//     dev_t     st_dev;     /* ID of device containing file */
//     ino_t     st_ino;     /* inode number */
//     mode_t    st_mode;    /* protection */
//     nlink_t   st_nlink;   /* number of hard links */
//     uid_t     st_uid;     /* user ID of owner */
//     gid_t     st_gid;     /* group ID of owner */
//     dev_t     st_rdev;    /* device ID (if special file) */
//     off_t     st_size;    /* total size, in bytes */
//     blksize_t st_blksize; /* blocksize for file system I/O */
//     blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
//     time_t    st_atime;   /* time of last access */
//     time_t    st_mtime;   /* time of last modification */
//     time_t    st_ctime;   /* time of last status change */
// };
