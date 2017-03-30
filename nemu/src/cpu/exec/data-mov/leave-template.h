#include "cpu/exec/template-start.h"

//#define instr leave


make_helper(concat(leave_, SUFFIX)) {
	int sreg=2;
	        if(DATA_BYTE==4){
			cpu.esp=cpu.ebp;
			cpu.ebp=MEM_R(cpu.esp);
			cpu.esp=cpu.esp+4;
			}
			else{
				 cpu.esp=cpu.gpr[5]._16;
				 cpu.ebp=MEM_R(cpu.gpr[5]._16);
				 cpu.esp=cpu.esp+2;
			}
			print_asm("leave");
			return 1;
}

#include "cpu/exec/template-end.h"


