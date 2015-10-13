#include "cpu/exec/template-start.h"

#define instr pop

#if DATA_BYTE==2||DATA_BYTE==4

static void do_execute() {
	    OPERAND_W(op_dest,MEM_R(cpu.esp));

}

make_helper(concat(pop_m_, SUFFIX)) {
	    //swaddr_t addr = instr_fetch(eip + 1, 4);
		//MEM_W(addr, REG(R_EAX));
        do_execute();
		cpu.esp=cpu.esp+4;
		print_asm_template1();
		return 2;
}

make_helper(concat(pop_r_, SUFFIX)) {
	    //swaddr_t addr = instr_fetch(eip + 1, 4);
		//MEM_W(addr, REG(R_EAX));
        do_execute();
		cpu.esp=cpu.esp+4;
		print_asm_template1();
		return 2;
}
#endif


#include "cpu/exec/template-end.h"
