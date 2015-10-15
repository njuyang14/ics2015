#include "cpu/exec/template-start.h"

#define instr setcc


make_helper(concat(setcc_setne_,SUFFIX)){
	            if(cpu.EFLAGS.ZF==0){
				//int temp_addr=instr_fetch(cpu.eip+1,DATA_BYTE);
			    //int shift=32-(DATA_BYTE<<3);
				//temp_addr=(temp_addr<<shift)>>shift;
     			//cpu.eip=cpu.eip+temp_addr;
				cpu.gpr[0]._16=1;									
				}
				else
					cpu.gpr[0]._16=0; 
				print_asm("setne %%al");
				return 2;
}

#include "cpu/exec/template-end.h"

