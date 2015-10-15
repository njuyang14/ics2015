#include "cpu/exec/template-start.h"

#define instr jcc


make_helper(concat(jbe_rel_,SUFFIX)){
	        if(cpu.EFLAGS.CF==1||cpu.EFLAGS.ZF==1){
				int temp_addr=instr_fetch(cpu.eip+1,DATA_BYTE);
				int shift=32-(DATA_BYTE<<3);
				temp_addr=(temp_addr<<shift)>>shift;
				cpu.eip=cpu.eip+temp_addr;
            }
			print_asm("jbe %x",cpu.eip);
			return 2;
}

#include "cpu/exec/template-end.h"
