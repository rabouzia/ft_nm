#include "ft_nm.h"

int main(int ac, char **av) {
	if (ac != 2 || ac != 3)
		return(dprintf(2, ARG_ERR));

	int fd = open(av[1], O_RDONLY);
    if (fd == -1) 
		return(dprintf(2, OP_ERR));
	
	struct stat st;
	
	if (fstat(fd, &st) < 0) 
		return(close(fd), dprintf(2, FSTAT_ERR));

	size_t file_size = st.st_size;
	printf("%zu", file_size);
	
	void *file_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (file_data == MAP_FAILED) 
		return(close(fd), dprintf(2, MMAP_ERR));

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