#include <elf.h>
#include <stdio.h>

#include <stdlib.h>
#include <sys/machelf.h>
void printELFHeader(Elf32_Ehdr ehdr);
void printProgramHeader(Elf32_Phdr phdr);
void printSectionHeader(Elf32_Shdr shdr);
void sec_flags(unsigned int flags);
void elf_flags(unsigned int flags);
void printSymTable(Elf32_Sym sym);


int main(int argc, char **argv){
	FILE *fp;
	Elf32_Ehdr ehdr;
	Elf32_Phdr phdr;
	Elf32_Shdr shdr;
	
	Elf32_Shdr string_shdr;
	Elf32_Shdr strtab_shdr;
	Elf32_Sym sym;
	Elf32_Versym ver;
	int i;
	
	//1.dynsym 2.symtab
	int dyntab[3][5],str_offset,str_size,shstr_offset,shstr_size,dystr_offset,dystr_size;

	//check command line argument
	if (argc != 2) {
		fprintf(stderr, "usage: %s filename\n", argv[0]);
		exit(1);
	}

	//open the file
	fp = fopen(argv[1], "r");

	//if we cannot open the file
	if (fp == 0) {
		fprintf(stderr, "%s: can't open %s\n",argv[0], argv[1]);
 		exit(1);
	}

	//if we cannot read the ehdr
	if ( fread( &ehdr, sizeof(ehdr), 1, fp ) != 1 ) {
		fprintf(stderr, "%s: couldn't read %s\n", argv[0], argv[1]);
		exit(1);
	}

	//file read, begin printing information
	printELFHeader(ehdr);

	/* And more */
	for ( i = 0; i < ehdr.e_phnum; i++ ) {
		printf("\n");
		printf("Program Header[%d]:\n", i);
		fseek( fp, ehdr.e_phoff + i * ehdr.e_phentsize, SEEK_SET );
		if ( fread( &phdr, sizeof(phdr), 1, fp ) != 1 ) {
			fprintf(stderr, "%s: couldn't read %s\n", argv[0], argv[1]);
			exit(1);
		}
		printProgramHeader(phdr);

	}

	fseek( fp, ehdr.e_shoff+ehdr.e_shstrndx*ehdr.e_shentsize, SEEK_SET);
	if ( fread( &string_shdr, sizeof(string_shdr), 1, fp ) != 1 ) {
		fprintf(stderr, "%s: couldn't read %s\n", argv[0], argv[1]);
		exit(1);
	}
	char *name=(char *)malloc(string_shdr.sh_size);
	fseek(fp,string_shdr.sh_offset,SEEK_SET);
	fread(name,string_shdr.sh_size,1,fp);
	
	Elf32_Shdr shdrTab[ehdr.e_shnum];
	
	for ( i = 0; i < ehdr.e_shnum; i++ ) {
		fseek( fp, ehdr.e_shoff + i * ehdr.e_shentsize, SEEK_SET );
		if ( fread( &shdrTab[i], sizeof(shdr), 1, fp ) != 1 ) {
			fprintf(stderr, "%s: couldn't read %s\n", argv[0], argv[1]);
			exit(1);
		}
		shdr=shdrTab[i];
		if(shdr.sh_size!=0){
			printf("\nSection Header[%d]:", i);
			
			printf("  sh_name: %s\n",name+shdr.sh_name);		
			if(strcmp(name+shdr.sh_name,".dynsym")==0){
				dyntab[0][0]=1;dyntab[0][1]=shdr.sh_addr;
				dyntab[0][2]=shdr.sh_offset;dyntab[0][3]=shdr.sh_size;
				dyntab[0][4]=shdr.sh_entsize;
			}else if(strcmp(name+shdr.sh_name,".symtab")==0){
				dyntab[1][0]=1;dyntab[1][1]=shdr.sh_addr;
				dyntab[1][2]=shdr.sh_offset;dyntab[1][3]=shdr.sh_size;
				dyntab[1][4]=shdr.sh_entsize;
			}else if(strcmp(name+shdr.sh_name,".strtab")==0){
				str_offset=shdr.sh_offset;str_size=shdr.sh_size;
			}else if(strcmp(name+shdr.sh_name,".shstrtab")==0){
				shstr_offset=shdr.sh_offset;shstr_size=shdr.sh_size;
			}else if(strcmp(name+shdr.sh_name,".dynstr")==0){
				dystr_offset=shdr.sh_offset;dystr_size=shdr.sh_size;
			}else if(strcmp(name+shdr.sh_name,".SUNW_versym")==0){
				dyntab[2][0]=1;dyntab[2][1]=shdr.sh_addr;
				dyntab[2][2]=shdr.sh_offset;dyntab[2][3]=shdr.sh_size;
				dyntab[2][4]=shdr.sh_entsize;
			}
			printSectionHeader(shdr);
		}
	}

	//read the string table
	char *nametab=(char *)malloc(dystr_size);
	fseek(fp,dystr_offset,SEEK_SET);
	fread(nametab,dystr_size,1,fp);
	
	char *strtab=(char *)malloc(shstr_size);
	fseek(fp,shstr_offset,SEEK_SET);
	fread(strtab,shstr_size,1,fp);
	
  if(dyntab[0][0]==1){
		printf("\nSymbol Table Section:  .dynsym\n");
		printf("index value size type bind oth ver shndx name\n");
		int totalnum;
		if(dyntab[0][4]==0){totalnum=0;}
		else{totalnum=dyntab[0][3]/dyntab[0][4];}
		
		for(i=0;i<totalnum;i++){
			fseek( fp, dyntab[0][2] + i * dyntab[0][4], SEEK_SET );
			if ( fread( &sym, sizeof(sym), 1, fp ) != 1 ) {
				fprintf(stderr, "%s: couldn't read %s\n", argv[0], argv[1]);
				exit(1);
			}
			if(dyntab[2][0]==1){
				fseek( fp, dyntab[2][2] + i * dyntab[2][4], SEEK_SET );
				if ( fread( &ver, sizeof(ver), 1, fp ) != 1 ) {
					fprintf(stderr, "%s: couldn't read %s\n", argv[0], argv[1]);
					exit(1);
				}
			}
			
			printf(" [%d] ",i);
			printSymTable(sym);
			
			//print version
			if(dyntab[2][0]==1){
				printf(" %d",ver);
			}else{
				printf(" 0");
			}
			
			if(sym.st_shndx<=0){
				printf(" UNDEF");
			}else if(sym.st_shndx>=ehdr.e_shnum){
				printf(" ABS");
			}else{
				if(strcmp(strtab+shdrTab[sym.st_shndx].sh_name,".debug_pubnames")==0){
					printf(" .debug_pubname");
				}else if(strcmp(strtab+shdrTab[sym.st_shndx].sh_name,".data.rel.local")==0){
					printf(" .data.rel.loca");
				}else{
					printf(" %s",strtab+shdrTab[sym.st_shndx].sh_name);
				}
			}
			printf(" %s\n",nametab+sym.st_name);
		}
	}
	
	char *nmtab=(char *)malloc(str_size);
	fseek(fp,str_offset,SEEK_SET);
	fread(nmtab,str_size,1,fp);
	
	if(dyntab[1][0]==1){
		printf("\nSymbol Table Section:  .symtab\n");
		printf("index value size type bind oth ver shndx name\n");
		int totalnum;
		if(dyntab[1][4]==0){totalnum=0;}
		else{totalnum=dyntab[1][3]/dyntab[1][4];}
		
		for(i=0;i<totalnum;i++){
			fseek( fp, dyntab[1][2] + i * dyntab[1][4], SEEK_SET );
			if ( fread( &sym, sizeof(sym), 1, fp ) != 1 ) {
				fprintf(stderr, "%s: couldn't read %s\n", argv[0], argv[1]);
				exit(1);
			}
			printf(" [%d] ",i);
			printSymTable(sym);
			
			//print version
			printf(" 0");
			
			if(sym.st_shndx<=0){
				printf(" UNDEF");
			}else if(sym.st_shndx>=ehdr.e_shnum){
				printf(" ABS");
			}else{
				if(strcmp(strtab+shdrTab[sym.st_shndx].sh_name,".debug_pubnames")==0){
					printf(" .debug_pubname");
				}else if(strcmp(strtab+shdrTab[sym.st_shndx].sh_name,".data.rel.local")==0){
					printf(" .data.rel.loca");
				}else{
					printf(" %s",strtab+shdrTab[sym.st_shndx].sh_name);
				}
			}
			
			printf(" %s\n",nmtab+sym.st_name);
		}
	}

	exit(0);
}

void printSymTable(Elf32_Sym sym){
	char *type_str[16]={"NOTY","OBJT","FUNC","SECT","FILE","COMM","TLS"};
	type_str[10]="LOOS";type_str[12]="HIOS";type_str[13]="LOPR";type_str[15]="HIPR";
	char *bind_str[16]={"LOCL","GLOB","WEAK"};
	bind_str[10]="LOOS";bind_str[12]="HIOS";bind_str[13]="LOPR";bind_str[15]="HIPR";
	char *other_str[4]={"D","I","H","P"};
	
	int TYPEINT=(sym.st_info) & 0xf, BINDINT=(sym.st_info) >> 4;
	
	printf("0x%08x 0x%08x %s %s %s",sym.st_value,sym.st_size,type_str[TYPEINT],bind_str[BINDINT],other_str[(sym.st_other) & 0x3]);
}

void printELFHeader(Elf32_Ehdr ehdr){
	char *ei_class[4]={"ELFCLASSNONE","ELFCLASS32","ELFCLASS64","ELFCLASSNUM"};
	char *ei_data[4]={"ELFDATANONE","ELFDATA2LSB","ELFDATA2MSB","ELFDATANUM"};
	char *ei_osabi[256]={"ELFOSABI_NONE","ELFOSABI_HPUX","ELFOSABI_NETBSD",\
	"ELFOSABI_LINUX","ELFOSABI_UNKNOWN4","ELFOSABI_UNKNOWN5","ELFOSABI_SOLARIS",\
	"ELFOSABI_AIX","ELFOSABI_IRIX","ELFOSABI_FREEBSD","ELFOSABI_TRU64",\
	"ELFOSABI_MODESTO","ELFOSABI_OPENBSD","ELFOSABI_OPENVMS","ELFOSABI_NSK","ELFOSABI_AROS"};
	ei_osabi[97]="ELFOSABI_ARM";ei_osabi[255]="ELFOSABI_STANDALONE";
	char *machine[96]={"EM_NONE","EM_M32","EM_SPARC","EM_386","EM_68K","EM_88K","EM_486","EM_860",\
	"EM_MIPS","EM_S370","EM_MIPS_RS3_LE","EM_RS6000","EM_UNKNOWN12","EM_UNKNOWN13",\
	"EM_UNKNOWN14","EM_PA_RISC","EM_nCUBE","EM_VPP500","EM_SPARC32PLUS","EM_960",\
	"EM_PPC","EM_PPC64","EM_S390","EM_UNKNOWN23","EM_UNKNOWN24",\
	"EM_UNKNOWN25","EM_UNKNOWN26","EM_UNKNOWN27","EM_UNKNOWN28","EM_UNKNOWN29",\
	"EM_UNKNOWN30","EM_UNKNOWN31","EM_UNKNOWN32","EM_UNKNOWN33","EM_UNKNOWN34",\
	"EM_UNKNOWN35","EM_V800","EM_FR20","EM_RH32","EM_RCE","EM_ARM","EM_ALPHA",\
	"EM_SH","EM_SPARCV9","EM_TRICORE","EM_ARC","EM_H8_300","EM_H8_300H","EM_H8S",\
	"EM_H8_500","EM_IA_64","EM_MIPS_X","EM_COLDFIRE","EM_68HC12","EM_MMA","EM_PCP",\
	"EM_NCPU","EM_NDR1","EM_STARCORE","EM_ME16","EM_ST100","EM_TINYJ","EM_AMD64",\
	"EM_PDSP","EM_UNKNOWN64","EM_UNKNOWN65","EM_FX66","EM_ST9PLUS","EM_ST7",\
	"EM_68HC16","EM_68HC11","EM_68HC08","EM_68HC05","EM_SVX","EM_ST19",\
	"EM_VAX","EM_CRIS","EM_JAVELIN","EM_FIREPATH","EM_ZSP","EM_MMIX","EM_HUANY",\
	"EM_PRISM","EM_AVR","EM_FR30","EM_D10V","EM_D30V","EM_V850","EM_M32R",\
	"EM_MN10300","EM_MN10200","EM_PJ","EM_OPENRISC","EM_ARC_A5","EM_XTENSA","EM_NUM"};
	char *version[3]={"EV_NONE","EV_CURRENT","EV_NUM"};
	char *type[6]={"ET_NONE","ET_REL","ET_EXEC","ET_DYN","ET_CORE","ET_NUM"};
	char *abiversion[3]={"EAV_SUNW_NONE","EAV_SUNW_CURRENT","EAV_SUNW_NUM"};

	//START PRINTING
	printf("\nELF Header\n");
	printf("  ei_magic: { 0x%x, %c, %c, %c}\n",ehdr.e_ident[0],ehdr.e_ident[1],ehdr.e_ident[2],ehdr.e_ident[3]);
	printf("  ei_class: %s ei_data: %s\n",ei_class[ehdr.e_ident[4]],ei_data[ehdr.e_ident[5]]);
	if(ehdr.e_ident[8]==0){
		printf("  ei_osabi: %s ei_abiversion: 0\n",ei_osabi[ehdr.e_ident[7]]);
	}else{
		printf("  ei_osabi: %s ei_abiversion: %s\n",ei_osabi[ehdr.e_ident[7]],abiversion[ehdr.e_ident[8]]);
	}
	
	printf("  e_machine: %s e_version: %s\n",machine[ehdr.e_machine],version[ehdr.e_version]);
	
	if(ehdr.e_type<=5){
		printf("  e_type: %s\n",type[ehdr.e_type]);
	}else if(ehdr.e_type==0xfe00){
		printf("  e_type: ET_LOOS\n");
	}else if(ehdr.e_type==0xfeff){
		printf("  e_type: ET_LOSUNW\n");
	}else if(ehdr.e_type==0xff00){
		printf("  e_type: ET_LOPROC\n");
	}else if(ehdr.e_type==0xffff){
		printf("  e_type: ET_HIPROC\n");
	}

	if(ehdr.e_flags==0){
		printf("  e_flags:    0\n");
	}else{
		printf("  e_flags:    [ ");
		elf_flags(ehdr.e_flags);
		printf("]\n");
	}
	printf("  e_entry:%#22x  e_ehsize:%7d  e_shstrndx:  %d\n",ehdr.e_entry,ehdr.e_ehsize,ehdr.e_shstrndx);
	printf("  e_shoff:%#22x  e_shentsize:%4d  e_shnum:     %d\n",ehdr.e_shoff,ehdr.e_shentsize,ehdr.e_shnum);
	printf("  e_phoff:%#22x  e_phentsize:%4d  e_phnum:     %d\n",ehdr.e_phoff,ehdr.e_phentsize,ehdr.e_phnum);
}

void printProgramHeader(Elf32_Phdr phdr){
	char *prog_type[9]={"PT_NULL","PT_LOAD","PT_DYNAMIC","PT_INTERP",\
	"PT_NOTE","PT_SHLIB","PT_PHDR","PT_TLS","PT_NUM"};
	switch(phdr.p_flags){
		case 0:
		printf("    p_vaddr: %#x p_flags: [  ]\n",phdr.p_vaddr);
		break;
		case 1:
		printf("    p_vaddr: %#x p_flags: [ PF_X ]\n",phdr.p_vaddr);
		break;
		case 2:
		printf("    p_vaddr: %#x p_flags: [ PF_W ]\n",phdr.p_vaddr);
		break;
		case 3:
		printf("    p_vaddr: %#x p_flags: [ PF_X PF_W ]\n",phdr.p_vaddr);
		break;
		case 4:
		printf("    p_vaddr: %#x p_flags: [ PF_R ]\n",phdr.p_vaddr);
		break;
		case 5:
		printf("    p_vaddr: %#x p_flags: [ PF_X PF_R ]\n",phdr.p_vaddr);
		break;
		case 6:
		printf("    p_vaddr: %#x p_flags: [ PF_W PF_R ]\n",phdr.p_vaddr);
		break;
		case 7:
		printf("    p_vaddr: %#x p_flags: [ PF_X PF_W PF_R ]\n",phdr.p_vaddr);
	}
	
	if(phdr.p_type<=8){
		printf("    p_paddr: %#x p_type: [ %s ]\n",phdr.p_paddr,prog_type[phdr.p_type]);
	}else if(phdr.p_type==0x60000000){
		printf("    p_paddr: %#x p_type: [ PT_LOSS ]\n",phdr.p_paddr);
	}else if(phdr.p_type==0x6464e550){
		printf("    p_paddr: %#x p_type: [ PT_SUNW_UNWIND ]\n",phdr.p_paddr);
	}else if(phdr.p_type==0x6474e551){
		printf("    p_paddr: %#x p_type: [ PT_GNU_STACK ]\n",phdr.p_paddr);
	}else if(phdr.p_type==0x6474e552){
		printf("    p_paddr: %#x p_type: [ PT_GNU_RELRO ]\n",phdr.p_paddr);
	}else if(phdr.p_type==0x6ffffffa){
		printf("    p_paddr: %#x p_type: [ PT_LOSUNW ]\n",phdr.p_paddr);
	}else if(phdr.p_type==0x6ffffffb){
		printf("    p_paddr: %#x p_type: [ PT_SUNWSTACK ]\n",phdr.p_paddr);
	}else if(phdr.p_type==0x6ffffffc){
		printf("    p_paddr: %#x p_type: [ PT_SUNWDTRACE ]\n",phdr.p_paddr);
	}else if(phdr.p_type==0x6ffffffd){
		printf("    p_paddr: %#x p_type: [ PT_SUNWCAP ]\n",phdr.p_paddr);
	}else if(phdr.p_type==0x6fffffff){
		printf("    p_paddr: %#x p_type: [ PT_HISUNW ]\n",phdr.p_paddr);
	}else if(phdr.p_type==0x70000000){
		printf("    p_paddr: %#x p_type: [ PT_LOPROC ]\n",phdr.p_paddr);
	}else if(phdr.p_type==0x7fffffff){
		printf("    p_paddr: %#x p_type: [ PT_HIPROC ]\n",phdr.p_paddr);
	}
	printf("    p_filesz: %#x p_memsz: %#x\n",phdr.p_filesz,phdr.p_memsz);
    printf("    p_offset: %#x p_align: %#x\n",phdr.p_offset,phdr.p_align);

}

void printSectionHeader(Elf32_Shdr shdr){
	char *sec_type[20]={"SHT_NULL","SHT_PROGBITS","SHT_SYMTAB","SHT_STRTAB",\
	"SHT_RELA","SHT_HASH","SHT_DYNAMIC","SHT_NOTE","SHT_NOBITS",\
	"SHT_REL","SHT_SHLIB","SHT_DYNSYM","SHT_UNKNOWN12","SHT_UNKNOWN13",\
	"SHT_INIT_ARRAY","SHT_FINI_ARRAY","SHT_PREINIT_ARRAY","SHT_GROUP",\
	"SHT_SYMTAB_SHNDX","SHT_NUM"};
	if(shdr.sh_flags==0){
		printf("    sh_addr: %#x sh_flags: 0\n",shdr.sh_addr);
	}else{
		printf("    sh_addr: %#x sh_flags: [ ",shdr.sh_addr);
		sec_flags(shdr.sh_flags);
		printf("]\n");
	}
	if(shdr.sh_type<20){
		printf("    sh_size: %#x sh_type: [ %s ]\n",shdr.sh_size,sec_type[shdr.sh_type]);
	}else if(shdr.sh_type==0x60000000){
		printf("    sh_size: %#x sh_type: [ SHT_LOOS ]\n",shdr.sh_size);
	}else if(shdr.sh_type==0x6fffffef){
		printf("    sh_size: %#x sh_type: [ SHT_LOSUNW ]\n",shdr.sh_size);
	}else if(shdr.sh_type==0x6ffffff0){
		printf("    sh_size: %#x sh_type: [ SHT_SUNW_capinfo ]\n",shdr.sh_size);
	}else if(shdr.sh_type==0x6ffffff1){
		printf("    sh_size: %#x sh_type: [ SHT_SUNW_symsort ]\n",shdr.sh_size);
	}else if(shdr.sh_type==0x6ffffff2){
		printf("    sh_size: %#x sh_type: [ SHT_SUNW_tlssort ]\n",shdr.sh_size);
	}else if(shdr.sh_type==0x6ffffff3){
		printf("    sh_size: %#x sh_type: [ SHT_SUNW_LDYNSYM ]\n",shdr.sh_size);
	}else if(shdr.sh_type==0x6ffffff4){
		printf("    sh_size: %#x sh_type: [ SHT_SUNW_dof ]\n",shdr.sh_size);
	}else if(shdr.sh_type==0x6ffffff5){
		printf("    sh_size: %#x sh_type: [ SHT_SUNW_cap ]\n",shdr.sh_size);
	}else if(shdr.sh_type==0x6ffffff6){
		printf("    sh_size: %#x sh_type: [ SHT_SUNW_SIGNATURE ]\n",shdr.sh_size);
	}else if(shdr.sh_type==0x6ffffff7){
		printf("    sh_size: %#x sh_type: [ SHT_SUNW_ANNOTATE ]\n",shdr.sh_size);
	}else if(shdr.sh_type==0x6ffffff8){
		printf("    sh_size: %#x sh_type: [ SHT_SUNW_DEBUGSTR ]\n",shdr.sh_size);
	}else if(shdr.sh_type==0x6ffffff9){
		printf("    sh_size: %#x sh_type: [ SHT_SUNW_DEBUG ]\n",shdr.sh_size);
	}else if(shdr.sh_type==0x6ffffffa){
		printf("    sh_size: %#x sh_type: [ SHT_SUNW_move ]\n",shdr.sh_size);
	}else if(shdr.sh_type==0x6ffffffb){
		printf("    sh_size: %#x sh_type: [ SHT_SUNW_COMDAT ]\n",shdr.sh_size);
	}else if(shdr.sh_type==0x6ffffffc){
		printf("    sh_size: %#x sh_type: [ SHT_SUNW_syminfo ]\n",shdr.sh_size);
	}else if(shdr.sh_type==0x6ffffffd){
		printf("    sh_size: %#x sh_type: [ SHT_SUNW_verdef ]\n",shdr.sh_size);
	}else if(shdr.sh_type==0x6ffffffe){
		printf("    sh_size: %#x sh_type: [ SHT_SUNW_verneed ]\n",shdr.sh_size);
	}else if(shdr.sh_type==0x6fffffff){
		printf("    sh_size: %#x sh_type: [ SHT_SUNW_versym ]\n",shdr.sh_size);
	}

	if(shdr.sh_entsize!=0){
		int num=shdr.sh_size/shdr.sh_entsize;
		if(num==1){
			printf("    sh_offset: %#x sh_entsize: %#x (%d entry)\n",shdr.sh_offset,shdr.sh_entsize,num);
		}else{
			printf("    sh_offset: %#x sh_entsize: %#x (%d entries)\n",shdr.sh_offset,shdr.sh_entsize,num);
		}
	}else{
		printf("    sh_offset: %#x sh_entsize: 0\n",shdr.sh_offset);
	}
	printf("    sh_link: %d sh_info: %d\n",shdr.sh_link,shdr.sh_info);
	printf("    sh_addralign: %#x\n",shdr.sh_addralign);

}


void sec_flags(unsigned int flags){
    if(flags<=0){return;}
    else if(flags>=0xf0000000){ sec_flags(flags-0xf0000000); printf("SHF_MASKPROC ");}
    else if(flags>=0x80000000){ sec_flags(flags-0x80000000); printf("SHF_EXCLUDE ");}
    else if(flags>=0x40000000){ sec_flags(flags-0x40000000); printf("SHF_ORDERED ");}
    else if(flags>=0x0ff00000){ sec_flags(flags-0x0ff00000); printf("SHF_MASKOS ");}
    else if(flags>=0x400){ sec_flags(flags-0x400); printf("SHF_TLS ");}
    else if(flags>=0x200){ sec_flags(flags-0x200); printf("SHF_GROUP ");}
    else if(flags>=0x100){ sec_flags(flags-0x100); printf("SHF_OS_NONCONFORMING ");}
    else if(flags>=0x80){ sec_flags(flags-0x80); printf("SHF_LINK_ORDER ");}
    else if(flags>=0x40){ sec_flags(flags-0x40); printf("SHF_INFO_LINK ");}
    else if(flags>=0x20){ sec_flags(flags-0x20); printf("SHF_STRINGS ");}
    else if(flags>=0x10){ sec_flags(flags-0x10); printf("SHF_MERGE ");}
    else if(flags>=0x04){ sec_flags(flags-0x04); printf("SHF_EXECINSTR ");}
    else if(flags>=0x02){ sec_flags(flags-0x02); printf("SHF_ALLOC ");}
    else if(flags>=0x01){sec_flags(flags-0x01); printf("SHF_WRITE ");}
}

void elf_flags(unsigned int flags){
    if(flags<0){return;}
    else if(flags>=0xffff00){ elf_flags(flags-0xffff00); printf("EF_SPARC_EXT_MASK ");}
    else if(flags>=0x800){ elf_flags(flags-0x800); printf("EF_SPARC_SUN_US3 ");}
    else if(flags>=0x400){ elf_flags(flags-0x400); printf("EF_SPARC_HAL_R1 ");}
    else if(flags>=0x200){ elf_flags(flags-0x200); printf("EF_SPARC_SUN_US1 ");}
    else if(flags>=0x100){ elf_flags(flags-0x100); printf("EF_SPARC_32PLUS ");}
    else if(flags>=0x3){ elf_flags(flags-0x3); printf("EF_SPARCV9_MM ");}
    else if(flags>=0x2){ elf_flags(flags-0x2); printf("EF_SPARCV9_RMO ");}
    else if(flags>=0x1){ elf_flags(flags-0x1); printf("EF_SPARCV9_PSO ");}
}






