#include "nemu.h"
#include "../memory/tlb.h"

#define ENTRY_START 0x100000

extern uint8_t entry [];
extern uint32_t entry_len;
extern char *exec_file;

void load_elf_tables(int, char *[]);
void init_regex();
void init_wp_list();
void init_ddr3();

FILE *log_fp = NULL;

static void init_log() {
	log_fp = fopen("log.txt", "w");
	Assert(log_fp, "Can not open 'log.txt'");
}

static void welcome() {
	printf("Welcome to NEMU!\nThe executable is %s.\nFor help, type \"help\"\n",
			exec_file);
}

void init_monitor(int argc, char *argv[]) {
	/* Perform some global initialization */

	/* Open the log file. */
	init_log();

	/* Load the string table and symbol table from the ELF file for future use. */
	load_elf_tables(argc, argv);

	/* Compile the regular expressions. */
	init_regex();

	/* Initialize the watchpoint link list. */
	init_wp_list();

	/* Display welcome message. */
	welcome();
}

#ifdef USE_RAMDISK
static void init_ramdisk() {
	int ret;
	const int ramdisk_max_size = 0xa0000;
	FILE *fp = fopen(exec_file, "rb");
	Assert(fp, "Can not open '%s'", exec_file);

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	Assert(file_size < ramdisk_max_size, "file size(%zd) too large", file_size);

	fseek(fp, 0, SEEK_SET);
	ret = fread(hwa_to_va(0), file_size, 1, fp);
	assert(ret == 1);
	fclose(fp);
}
#endif

static void load_entry() {
	int ret;
	FILE *fp = fopen("entry", "rb");
	Assert(fp, "Can not open 'entry'");

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);

	fseek(fp, 0, SEEK_SET);
	ret = fread(hwa_to_va(ENTRY_START), file_size, 1, fp);
	assert(ret == 1);
	fclose(fp);
}

void init_eflags(){
        cpu.EFLAGS.CF=0;
        cpu.EFLAGS.blank1=1;
        cpu.EFLAGS.PF=0;
        cpu.EFLAGS.blank2=0;
        cpu.EFLAGS.AF=0;
        cpu.EFLAGS.blank3=0;
        cpu.EFLAGS.ZF=0;
        cpu.EFLAGS.SF=0;
        cpu.EFLAGS.TF=0;
        cpu.EFLAGS.IF=0;
        cpu.EFLAGS.DF=0;
        cpu.EFLAGS.OF=0;
        cpu.EFLAGS.IOPL=0;
        cpu.EFLAGS.NT=0;
        cpu.EFLAGS.blank4=0;
        cpu.EFLAGS.RF=0;
        cpu.EFLAGS.VM=0;
        cpu.EFLAGS.blank5=0;

}

void init_cr0(){
	cpu.cr0.protect_enable=0;
	cpu.cr0.monitor_coprocessor=0;
	cpu.cr0.emulation=0;
	cpu.cr0.task_switched=0;
	cpu.cr0.extension_type=0;
	cpu.cr0.numeric_error=0;
	cpu.cr0.pad0=0;
	cpu.cr0.write_protect=0;
	cpu.cr0.pad1=0;
	cpu.cr0.alignment_mask=0;
	cpu.cr0.pad2=0;
	cpu.cr0.no_write_through=0;
	cpu.cr0.cache_disable=0;
	cpu.cr0.paging=0;
}

void init_seg(){
	cpu.segdesc[1].base_15_0=0;
	cpu.segdesc[1].base_23_16=0;
	cpu.segdesc[1].base_31_24=0;
	cpu.segdesc[1].limit_15_0=0xffff;
	cpu.segdesc[1].limit_19_16=0xf;
}

void init_cache();

void init_tlb(){
	int i;
	for(i=0;i<64;i++){
		tlb[i].valid=0;
	}
}

void restart() {
	/* Perform some initialization to restart a program */
#ifdef USE_RAMDISK
	/* Read the file with name `argv[1]' into ramdisk. */
	init_ramdisk();
#endif

	/* Read the entry code into memory. */
	load_entry();

	/* Set the initial instruction pointer. */
	cpu.eip = ENTRY_START;

    /* Init the EFLAGS register*/
    init_eflags(); 

    /* Init CR0*/
    init_cr0();	

	init_seg();
    
	/* Initialize DRAM. */
	init_ddr3();
    /*Initialize cache*/
	init_cache();

}
