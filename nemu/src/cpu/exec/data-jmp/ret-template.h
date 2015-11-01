#include "cpu/exec/template-start.h"
#define instr ret

make_helper(concat(ret_, SUFFIX)) {
        cpu.eip=swaddr_read(cpu.esp,4);
		if(DATA_BYTE==2)
		    cpu.esp=cpu.esp+2;
		else if(DATA_BYTE==4||DATA_BYTE==1)
			cpu.esp=cpu.esp+4;
		if(DATA_BYTE==2)cpu.eip=cpu.eip&0x0000FFFF;
		print_asm("ret");
		return 1;
}

make_helper(concat(ret_imm_, SUFFIX)) {
	    swaddr_t addr=instr_fetch(cpu.eip+1,DATA_BYTE);
	    cpu.eip=swaddr_read(cpu.esp,4)-DATA_BYTE-1;
		cpu.esp=cpu.esp+addr;
		if(DATA_BYTE==2)cpu.eip=cpu.eip&0x0000FFFF;
		print_asm_template1();
		return 3;
}


#include "cpu/exec/template-end.h"
