/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramzerk <ramzerk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 00:29:40 by ramzerk           #+#    #+#             */
/*   Updated: 2025/04/07 02:26:34 by ramzerk          ###   ########.fr       */
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

typedef struct s_nm
{
    char letter;
    Elf64_Addr addr;
    char *symbol;
    bool defined;
    struct  s_nm *next;
}               t_nm;


void	ft_nmaddback(t_nm **head, t_nm *new);
t_nm	*ft_nmnew(char *symbol, Elf64_Addr addr, char c);
t_nm	*ft_nmlast(t_nm *head);
void    ft_check_same(t_nm **nm);
void	ft_nmclear(t_nm **nm);
void    ft_nmprint(t_nm *nm);
void	free_node(t_nm *nm);
void	remove_node(t_nm *nm, char *to_delete);
void	remove_last(t_nm *nm);
void	remove_first(t_nm **nm);
int	    search_nm(t_nm *nm, char *key);
bool	delete_node(t_nm **nm, char *to_delete);
void get_flag(t_nm *nm);




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
