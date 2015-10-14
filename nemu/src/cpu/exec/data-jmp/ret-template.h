#include "cpu/exec/template-start.h"


make_helper(concat(ret_, SUFFIX)) {
        cpu.eip=cpu.esp;
		cpu.esp=cpu.esp+DATA_BYTE;
		if(DATA_BYTE==2)cpu.eip=cpu.eip&0x0000FFFF;
		print_asm("ret\n");
		return 1;
}

make_helper(concat(ret_imm_, SUFFIX)) {
	    swaddr_t addr=instr_fetch(cpu.eip+1,DATA_BYTE);
	    MEM_W(cpu.eip,MEM_R(cpu.esp));
		cpu.esp=cpu.esp+addr;
		if(DATA_BYTE==2)cpu.eip=cpu.eip&0x0000FFFF;
		print_asm_template1();
		return 2;
}


#include "cpu/exec/template-end.h"
