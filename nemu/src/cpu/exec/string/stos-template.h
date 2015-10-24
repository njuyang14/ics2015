#include "cpu/exec/template-start.h"

#define instr stos

make_helper(concat(stos_m_, SUFFIX)) {
    //swaddr_t addr = instr_fetch(eip + 1, DATA_BYTE);
	cpu.edi=REG(R_EAX);
	print_asm("stos %%%s",REG_NAME(R_EAX));
	return DATA_BYTE+1;
}
#include "cpu/exec/template-end.h"
