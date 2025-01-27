/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzoldik <rzoldik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 22:15:32 by rabouzia          #+#    #+#             */
/*   Updated: 2025/01/27 16:16:52 by rzoldik          ###   ########.fr       */
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
#include <sys/mman.h>
#include <unistd.h>

int main(int ac, char **av) {
	if (ac != 2)
		return 0;
    int fd = open(av[1], O_RDONLY);  // Open the file in read-only mode
    if (fd == -1) {
        perror("Failed to open file");
        return 1;
    }

    // Get the size of the file
    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1) {
        perror("Failed to get file size");
        close(fd);
        return 1;
    }

    // Map the file into memory
    char *mapped = mmap(NULL, file_size, PROT_READ, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("Failed to map file");
        close(fd);
        return 1;
    }

    // Print the entire content of the file byte-by-byte
    for (off_t i = 0; i < file_size; i++) {
        printf("%c", mapped[i]);
    }
    printf("\n");

    // Unmap the file and close the file descriptor
    if (munmap(mapped, file_size) == -1) {
        perror("Failed to unmap file");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}


/*
	- decode les binaire (.o)
		- utiliser mmap
*/