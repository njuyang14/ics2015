#include "common.h"
#include "nemu.h"
#include <stdlib.h>
#include <elf.h>

char *exec_file = NULL;

static char *strtab = NULL;
static Elf32_Sym *symtab = NULL;
static int nr_symtab_entry;

void load_elf_tables(int argc, char *argv[]) {
	int ret;
	Assert(argc == 2, "run NEMU with format 'nemu [program]'");
	exec_file = argv[1];

	FILE *fp = fopen(exec_file, "rb");
	Assert(fp, "Can not open '%s'", exec_file);

	uint8_t buf[sizeof(Elf32_Ehdr)];
	ret = fread(buf, sizeof(Elf32_Ehdr), 1, fp);
	assert(ret == 1);

	/* The first several bytes contain the ELF header. */
	Elf32_Ehdr *elf = (void *)buf;
	char magic[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};

	/* Check ELF header */
	assert(memcmp(elf->e_ident, magic, 4) == 0);		// magic number
	assert(elf->e_ident[EI_CLASS] == ELFCLASS32);		// 32-bit architecture
	assert(elf->e_ident[EI_DATA] == ELFDATA2LSB);		// littel-endian
	assert(elf->e_ident[EI_VERSION] == EV_CURRENT);		// current version
	assert(elf->e_ident[EI_OSABI] == ELFOSABI_SYSV || 	// UNIX System V ABI
			elf->e_ident[EI_OSABI] == ELFOSABI_LINUX); 	// UNIX - GNU
	assert(elf->e_ident[EI_ABIVERSION] == 0);			// should be 0
	assert(elf->e_type == ET_EXEC);						// executable file
	assert(elf->e_machine == EM_386);					// Intel 80386 architecture
	assert(elf->e_version == EV_CURRENT);				// current version


	/* Load symbol table and string table for future use */

	/* Load section header table */
	uint32_t sh_size = elf->e_shentsize * elf->e_shnum;
	Elf32_Shdr *sh = malloc(sh_size);
	fseek(fp, elf->e_shoff, SEEK_SET);
	ret = fread(sh, sh_size, 1, fp);
	assert(ret == 1);

	/* Load section header string table */
	char *shstrtab = malloc(sh[elf->e_shstrndx].sh_size);
	fseek(fp, sh[elf->e_shstrndx].sh_offset, SEEK_SET);
	ret = fread(shstrtab, sh[elf->e_shstrndx].sh_size, 1, fp);
	assert(ret == 1);

	int i;
	for(i = 0; i < elf->e_shnum; i ++) {
		if(sh[i].sh_type == SHT_SYMTAB && 
				strcmp(shstrtab + sh[i].sh_name, ".symtab") == 0) {
			/* Load symbol table from exec_file */
			symtab = malloc(sh[i].sh_size);
			fseek(fp, sh[i].sh_offset, SEEK_SET);
			ret = fread(symtab, sh[i].sh_size, 1, fp);
			assert(ret == 1);
			nr_symtab_entry = sh[i].sh_size / sizeof(symtab[0]);
		}
		else if(sh[i].sh_type == SHT_STRTAB && 
				strcmp(shstrtab + sh[i].sh_name, ".strtab") == 0) {
			/* Load string table from exec_file */
			strtab = malloc(sh[i].sh_size);
			fseek(fp, sh[i].sh_offset, SEEK_SET);
			ret = fread(strtab, sh[i].sh_size, 1, fp);
			assert(ret == 1);
		}
	}

	free(sh);
	free(shstrtab);

	assert(strtab != NULL && symtab != NULL);

	fclose(fp);
}

uint32_t print_var(char *s){
	int i=0,j=0;
	for(;i<nr_symtab_entry;i++){
		int flag=0;
		for(;s[j]!='\0'&&strtab[symtab[i].st_name+j]!='\0';j++){
			if(strtab[symtab[i].st_name+j]==s[j]){
				flag=1;
			}
			else{
				flag=0;
				break;
			}
		}
		if(flag==1){
			return symtab[i].st_value;
		}
	}

	printf("The var is invalid.\n");
	return 0;
}

void print_stack_frame(){
	int i=0,no=0;
	uint32_t prebp=cpu.ebp;
	uint32_t ret_addr; 
    while(prebp!=0){
	  for(;i<nr_symtab_entry;i++){
		  int func_addr=symtab[i].st_value+symtab[i].st_size;
		  if(no==0&&symtab[i].st_info==18&&cpu.eip<=func_addr&&cpu.eip>=symtab[i].st_value){
			  
			  printf("#%d ",no);
			  no++;
			  printf("%x ",prebp);
			  int name_addr=symtab[i].st_name;
			  int j=0;
			  printf("in ");
			  while(strtab[name_addr+j]!='\0'){
		          printf("%c",strtab[name_addr+j]);
				    j++;
			  }

			  printf("(");
			  int k=0;
			  uint32_t temp=prebp;
			  while(k<4){
				  printf("%x ",swaddr_read(temp+8,4,2));
				  temp+=4;
				  k++;
			  }
			  printf(")\n");
			  break;
		  }
		  else if(no!=0&&symtab[i].st_info==18&&ret_addr<=func_addr&&ret_addr>=symtab[i].st_value){
			  
		       printf("#%d ",no);
               no++;

		       printf("%x ",prebp);

			   int name_addr=symtab[i].st_name;
			   int j=0;
			   printf("in ");
			   while(strtab[name_addr+j]!='\0'){
			       printf("%c",strtab[name_addr+j]);
			       j++;
		       }
		       printf("(");
               int k=0;
			   uint32_t temp=prebp;
			   while(k<4){
				   printf("%x ",swaddr_read(temp+8,4,2));
		           temp+=4;
				   k++;
			   }
               printf(")\n");
			   break;
		  }
	  }
	  ret_addr=swaddr_read(prebp+4,4,2); 
	  prebp=swaddr_read(prebp,4,2);
	  i=0;
	}
}



























