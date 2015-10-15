#include "cpu/exec/template-start.h"


make_helper(concat(ret_, SUFFIX)) {
        cpu.eip=swaddr_read(cpu.esp,DATA_BYTE);
		printf("eip=%x\n",cpu.eip);
		cpu.esp=cpu.esp+DATA_BYTE;
		if(DATA_BYTE==2)cpu.eip=cpu.eip&0x0000FFFF;
		print_asm("ret");
		return 1;
}

make_helper(concat(ret_imm_, SUFFIX)) {
	    swaddr_t addr=instr_fetch(cpu.eip+1,DATA_BYTE);
	    cpu.eip=swaddr_read(cpu.esp,DATA_BYTE);
		cpu.esp=cpu.esp+addr;
		if(DATA_BYTE==2)cpu.eip=cpu.eip&0x0000FFFF;
		print_asm_template1();
		return 3;
}


#include "cpu/exec/template-end.h"
