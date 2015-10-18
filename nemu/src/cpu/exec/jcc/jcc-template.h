#include "cpu/exec/template-start.h"

#define instr jcc


make_helper(concat(jcc_jbe_,SUFFIX)){
	        if(cpu.EFLAGS.CF==1||cpu.EFLAGS.ZF==1){
				int temp_addr=instr_fetch(cpu.eip+1,DATA_BYTE);
				int shift=32-(DATA_BYTE<<3);
				temp_addr=(temp_addr<<shift)>>shift;
				cpu.eip=cpu.eip+temp_addr;
            }
			print_asm("jbe %x",cpu.eip);
			return 2;
}

make_helper(concat(jcc_jne_,SUFFIX)){
	            if(cpu.EFLAGS.ZF==0){
				int temp_addr=instr_fetch(cpu.eip+1,DATA_BYTE);
				int shift=32-(DATA_BYTE<<3);
				temp_addr=(temp_addr<<shift)>>shift;
				cpu.eip=cpu.eip+temp_addr;
		        }
				print_asm("jne %x",cpu.eip);
				return 2;
}

make_helper(concat(jcc_jle_,SUFFIX)){
	            if(cpu.EFLAGS.ZF==1||(cpu.EFLAGS.SF!=cpu.EFLAGS.OF)){
				int temp_addr=instr_fetch(cpu.eip+1,DATA_BYTE);
				int shift=32-(DATA_BYTE<<3);
				temp_addr=(temp_addr<<shift)>>shift;
		        cpu.eip=cpu.eip+temp_addr;
				}
				print_asm("jle %x",cpu.eip);
				return 2;
}










#include "cpu/exec/template-end.h"
