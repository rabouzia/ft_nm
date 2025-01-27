/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ramzerk <ramzerk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 22:15:32 by rabouzia          #+#    #+#             */
/*   Updated: 2025/01/27 23:18:50 by ramzerk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"

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

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <elf.h>
#include <string.h>

int main(int ac, char **av) {
	if (ac != 2)
		return 0;
    
	int fd = open(av[1], O_RDONLY);
    if (fd == -1) 
	{
        perror("Failed to open file");
        return 1;
    }
	
	struct stat st;
	
	if (fstat(fd, &st) < 0) 
	{	
		perror("fstat");
		close(fd);
		exit(1);
	}

	size_t file_size = st.st_size;
	printf("%zu", file_size);
	
	void *file_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (file_data == MAP_FAILED) 
	{
    	perror("mmap");
    	close(fd);
    	exit(1);
	}
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
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)mapped;
	Elf64_Shdr *section_headers = (Elf64_Shdr *)((char *)mapped + ehdr->e_shoff);
	
	for (int i = 0; i < ehdr->e_shnum; i++) 
	{
		Elf64_Shdr *sh = &section_headers[i];
		printf("Section %d offset: %ld, size: %ld\n", i, sh->sh_offset, sh->sh_size);
	}

}


/*
	- decode les binaire (.o)
		- utiliser mmap
*/