#include "cpu/exec/template-start.h"



make_helper(concat(jbe_rel_,SUFFIX)){
#define instr jbe
	        //cpu.EFLAGS.CF=1;
	        if(cpu.EFLAGS.CF==1||cpu.EFLAGS.ZF==1){
				printf("eip=%x\n",cpu.eip);
				int temp_addr=instr_fetch(cpu.eip+1,DATA_BYTE);
				printf("addr=%x\n",temp_addr);
				int shift=32-(DATA_BYTE<<3);
				printf("%d\n",shift);
				temp_addr=(temp_addr<<shift)>>shift;
				printf("addr=%x\n",temp_addr); 
				cpu.eip=cpu.eip+temp_addr;
				printf("eip=%x\n",cpu.eip); 
            }
			print_asm_template1();
			return 2;
}

#include "cpu/exec/template-end.h"
