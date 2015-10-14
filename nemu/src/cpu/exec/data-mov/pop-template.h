#include "cpu/exec/template-start.h"

#define instr pop

//#if DATA_BYTE==2||DATA_BYTE==4

static void do_execute() {
	    OPERAND_W(op_dest,MEM_R(cpu.esp));
		cpu.esp=cpu.esp+DATA_BYTE;
		print_asm_template1();

}

make_helper(concat(pop_m_, SUFFIX)) {
		return 2;
}

make_instr_helper(r)

//#endif


#include "cpu/exec/template-end.h"
