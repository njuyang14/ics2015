#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_m_, SUFFIX)) {
	MEM_W(cpu.edi,swaddr_read(cpu.esi,DATA_BYTE));
	if(DATA_BYTE==1){
		if(cpu.EFLAGS.DF==0){
		    cpu.edi++;
		    cpu.esi++;
		}
		else{
			cpu.edi--;
			cpu.edi--;
		}
	}
	else if(DATA_BYTE==2){
		if(cpu.EFLAGS.DF==0){
		    cpu.edi+=2;
			cpu.esi+=2;
		}
		else{
			cpu.edi-=2;
			cpu.edi-=2;
		}
	}
	else if(DATA_BYTE==4){
	    if(cpu.EFLAGS.DF==0){
			cpu.edi+=4;
			cpu.esi+=4;
	    }
	    else{
			cpu.edi-=4;
			cpu.edi-=4;
		}
	}
	print_asm_template2();
	return DATA_BYTE;
}
#include "cpu/exec/template-end.h"
