#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute() {
	    OPERAND_W(op_src,swaddr_read(cpu.esp,DATA_BYTE));
		cpu.esp=cpu.esp+DATA_BYTE;
		//printf("esp=%x\n",cpu.esp);
		print_asm_template1();

}
make_helper(concat(pop_m_, SUFFIX)) {
		return 2;
}
make_instr_helper(r)

#include "cpu/exec/template-end.h"
