#include "cpu/exec/template-start.h"

#define instr lgdt

#if DATA_BYTE==2||DATA_BYTE==4
make_helper(concat(lgdt_i_, SUFFIX)) {
	uint32_t addr=instr_fetch(eip+2,4);
	uint32_t base;
	if(DATA_BYTE==2){
		base=instr_fetch(instr_fetch(eip+2,2),4);
		cpu.gdtr.limit=instr_fetch(addr,2);
		cpu.gdtr.base=instr_fetch(addr+2,3)&0x00ffffff;
	}
	else{
		base=instr_fetch(eip+2,4);
		cpu.gdtr.limit=instr_fetch(addr,2);
		cpu.gdtr.base=instr_fetch(addr+2,4);
	}
    print_asm("lgdt 0x%x",base);
	return 6;
}

#endif

#include "cpu/exec/template-end.h"
