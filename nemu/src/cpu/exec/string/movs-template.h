#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_m_, SUFFIX)) {
	cpu.edi=swaddr_read(cpu.esi,DATA_BYTE);
	print_asm("movs");
	return 1;
}
#include "cpu/exec/template-end.h"
