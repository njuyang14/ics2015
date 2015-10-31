#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute() {
	DATA_TYPE addr=op_src->val;
	swaddr_t opcode=instr_fetch(cpu.eip+1,1);
	if(opcode==0xb6){
	addr=(addr<<24)>>24;
	addr&=0x000000FF;
	}
	else if(opcode==0xb7){
		addr=addr<<16>>16;
		addr&=0x0000FFFF;
	}
	OPERAND_W(op_dest, addr);
	print_asm_template2();
}

make_instr_helper(rm2r)
#include "cpu/exec/template-end.h"
