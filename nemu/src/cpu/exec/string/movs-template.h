#include "cpu/exec/template-start.h"

#define instr movs

/*static void do_execute() {
	        //OPERAND_W(cpu.edi,swaddr_read(cpu.esi,DATA_BYTE));
			print_asm_template1();
}*/

make_helper(concat(movs_m_, SUFFIX)) {
	cpu.edi=swaddr_read(cpu.esi,DATA_BYTE);
	return 1;
}
#include "cpu/exec/template-end.h"
