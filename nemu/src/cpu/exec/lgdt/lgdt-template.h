#include "cpu/exec/template-start.h"

#define instr lgdt

#if DATA_BYTE==2||DATA_BYTE==4
make_helper(concat(lgdt_i_, SUFFIX)) {
	uint16_t limit=instr_fetch(eip+1,1);
	uint32_t base;
	if(DATA_BYTE==2){
		base=instr_fetch(eip+2,2);
		cpu.gdtr.limit=limit;
		cpu.gdtr.base=base&0x00ffffff;
	}
	else{
		base=instr_fetch(eip+2,4);
		cpu.gdtr.limit=limit;
		cpu.gdtr.base=base;
	}
	return 5;
}

#endif

#include "cpu/exec/template-end.h"
