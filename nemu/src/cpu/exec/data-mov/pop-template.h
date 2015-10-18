#include "cpu/exec/template-start.h"

#define instr pop

//#if DATA_BYTE==2||DATA_BYTE==4

static void do_execute() {
	    printf("src=%x,dest=%x\n",op_src->val,op_dest->val);
	    OPERAND_W(op_src,swaddr_read(cpu.esp,DATA_BYTE));
		cpu.esp=cpu.esp+DATA_BYTE;
		printf("esp=%x\n",cpu.esp);
		print_asm_template1();

}

make_helper(concat(pop_m_, SUFFIX)) {
		return 2;
}

make_instr_helper(r)

//#endif


#include "cpu/exec/template-end.h"
