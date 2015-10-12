#include "cpu/exec/template-start.h"



make_helper(concat(jbe_rel_,SUFFIX)){
#define instr jbe
	        if(cpu.EFLAGS.CF==1||cpu.EFLAGS.ZF==1){
				swaddr_t temp_addr=instr_fetch(cpu.eip+1,DATA_BYTE);
				cpu.eip+=temp_addr;
            }
			printf("eip=%x\n",cpu.eip);
			print_asm_template1();
			return 2;
}

#include "cpu/exec/template-end.h"
