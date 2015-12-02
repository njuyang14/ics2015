#include "cpu/exec/template-start.h"

#define instr mov

static void do_execute() {
	Log("hehe");
	uint8_t opcode = instr_fetch(cpu.eip+1,1);
	if(opcode==0x20){
		OPERAND_W(op_dest,cpu.cr0.val);
	}
	else if(opcode==0x22){
		Log("%x",opcode);
		cpu.cr0.val=op_src->val;
		Log("...");
	}
	else
	    OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)
make_instr_helper(rm)

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	int sreg=3;
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX));

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	int sreg=3;
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#include "cpu/exec/template-end.h"
