#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute() {
	DATA_TYPE addr=op_src->val;
	int shift=32-(DATA_BYTE<<3);
	addr=(addr<<shift)>>shift;

	if(DATA_BYTE==1)addr&=0x000000FF;
	else if(DATA_BYTE==4)addr&=0x0000FFFF;
	OPERAND_W(op_dest, addr);
	print_asm_template2();
}

make_instr_helper(rm2r)
#include "cpu/exec/template-end.h"
