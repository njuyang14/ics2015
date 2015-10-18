#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute() {
	printf("src=%x\n",op_src->val);
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(rm2r)
#include "cpu/exec/template-end.h"
