#include "cpu/exec/template-start.h"

#define instr movzxb
static void do_execute() {
	DATA_TYPE addr=op_src->val;
	addr=(addr<<24)>>24;
	addr&=0x000000FF;
	OPERAND_W(op_dest, addr);
	print_asm_template2();
}
make_instr_helper(rm2r);
#undef instr

#define instr movzxw
static void do_execute() {
	    DATA_TYPE addr=op_src->val;
		addr=(addr<<16)>>16;
		addr&=0x0000FFFF;
		OPERAND_W(op_dest, addr);
		print_asm_template2();
}
make_instr_helper(rm2r);
#undef instr
#include "cpu/exec/template-end.h"
