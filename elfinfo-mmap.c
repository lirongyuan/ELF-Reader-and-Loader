
#include <elf.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <stdlib.h>
int
main(argc, argv)
     int argc;
     char **argv;
{
	int fd;
	Elf32_Ehdr *ehdr;
	Elf32_Phdr *phdr;
	Elf32_Shdr *shdr;
	int i;

	if (argc != 2) {
		fprintf(stderr, "usage: %s filename\n", argv[0]);
		exit(1);
	}

	fd = open(argv[1], O_RDONLY);

	if (fd < 0) {
		fprintf(stderr, "%s: can't open %s\n",argv[0], argv[1]);
		perror("open");
 		exit(1);
	}

	// Get length of file
	struct stat st;
	fstat(fd, &st);
	int fileLen = st.st_size;

	// mmap file. The beginning of the file is ehdr
	ehdr = (Elf32_Ehdr *) mmap(0, fileLen, PROT_READ, MAP_PRIVATE, fd, 0);
	if (ehdr == (Elf32_Ehdr *) (MAP_FAILED)) {
		perror("mmap");
		exit(1);
	}

	printf("\n");
	printf("            ELF INFORMATION FOR \"%s\"\n\n", argv[1]);

	printf("ELF-Header --------------------\n");

	printf("Ident bytes:");
	for ( i = 0; i < EI_NIDENT; i++ ) {
		if ( i % 8 == 0 ) {
			printf("\n\t");
		}
		printf("%d:0x%x ", i, ehdr->e_ident[ i ] );
	}
	printf("\n");

	printf("File type:                    %d\n", ehdr->e_type );
	printf("Target machine:               %d\n", ehdr->e_machine );
	printf("File version:                 %d\n", ehdr->e_version );
	printf("Start address:                0x%x\n", ehdr->e_entry );

	/* And more */
	for ( i = 0; i < ehdr->e_phnum; i++ ) {
		printf("\n");
		printf("Program Header %d -------------\n", i);
		phdr = (Elf32_Phdr *) ((char*) ehdr + ehdr->e_phoff + i * ehdr->e_phentsize);

		printf("Entry type:                    %d\n", phdr->p_type );
		printf("File offset:                   %d\n", phdr->p_offset );
		printf("Virtual address:               0x%x\n", phdr->p_vaddr );
		printf("File size:                     %d\n", phdr->p_filesz );
		printf("memory size:                   %d\n", phdr->p_memsz );
		printf("memory/file alignment:         0x%x\n", phdr->p_align );
	}

	for ( i = 0; i < ehdr->e_shnum; i++ ) {
		printf("\n");
		printf("Section Header %d -------------\n", i);
		shdr = (Elf32_Shdr *) ((char*)ehdr+ehdr->e_shoff + i * ehdr->e_shentsize);

		printf("section name:                    %d\n", shdr->sh_name );
		printf("section type:                    %d\n", shdr->sh_type );
		printf("section flags:                   %d\n", shdr->sh_flags );
		printf("Virtual address:               0x%x\n", shdr->sh_addr );
	}

	exit(0);
}
