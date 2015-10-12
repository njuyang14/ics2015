#include "cpu/exec/template-start.h"



make_helper(concat(jbe_rel_,SUFFIX)){
#define instr jbe
	        //cpu.EFLAGS.CF=1;
	        if(cpu.EFLAGS.CF==1||cpu.EFLAGS.ZF==1){
				swaddr_t temp_addr=instr_fetch(cpu.eip+1,DATA_BYTE);
				int shift=DATA_BYTE<<3;
				temp_addr=(temp_addr<<shift)>>shift;
				cpu.eip=cpu.eip+temp_addr;
            }
			print_asm_template1();
			return 2;
}

#include "cpu/exec/template-end.h"
