#include "cpu/exec/template-start.h"

#define instr setcc


make_helper(concat(setcc_setne_,SUFFIX)){
	            if(cpu.EFLAGS.ZF==0){
				cpu.gpr[0]._16=1;									
				}
				else
					cpu.gpr[0]._16=0; 
				print_asm("setne %%al");
				return 2;
}

#include "cpu/exec/template-end.h"

