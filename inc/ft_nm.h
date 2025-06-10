/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramzerk <ramzerk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 00:29:40 by ramzerk           #+#    #+#             */
/*   Updated: 2025/04/11 22:56:37 by ramzerk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

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
#define OP_ERR "Failed to open file\n"
#define ARG_ERR "Usage ./ft_nm [-flag] <binary>\n"
#define FSTAT_ERR "fstat error\n"
#define MMAP_ERR "mmap error\n"


typedef struct s_res
{
    char letter;
    Elf64_Addr addr;
    char *symbol;
    bool defined;
    struct  s_res *next;
}               t_res;

typedef struct s_opt
{
	bool a;
	bool r;
	bool g;
	bool u;
	bool p;
	char *filename;
}			 t_opt;

typedef struct s_nm
{
	t_res *res;
	t_opt opt;
	Elf64_Ehdr *elf_hd; // elf header
	Elf64_Sym *symtab;
	Elf64_Ehdr *ehdr;
	Elf64_Shdr *section_headers;
	Elf64_Shdr *sh;
	char **section_names;
	void *fdata;
	
}			t_nm;






//##################### LST UTILS #################

int	    search_res(t_res *res, char *key);
void 	get_flag(t_res *res);
bool	delete_node(t_res **res, char *to_delete);
void	ft_resaddback(t_res **head, t_res *new);
t_res	*ft_resnew(char *symbol, Elf64_Addr addr, char c);
t_res	*ft_reslast(t_res *head);
void    ft_check_same(t_res **res);
void	ft_resclear(t_res **res);
void    ft_resprint(t_res *res);
void	free_node(t_res *res);
void	remove_node(t_res *res, char *to_delete);
void	remove_last(t_res *res);
void	remove_first(t_res **res);


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
