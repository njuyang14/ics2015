#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute() {
		DATA_TYPE_S addr=op_src->val;
		swaddr_t opcode=instr_fetch(cpu.eip+1,1);
        if(opcode==0xbe){
		addr=(addr<<24)>>24;
		}
		else if(opcode==0xbf){
			addr=(addr<<16)>>16;
	    }
		OPERAND_W(op_dest, addr);
		print_asm_template2();
}

make_instr_helper(rm2r)
#include "cpu/exec/template-end.h"
