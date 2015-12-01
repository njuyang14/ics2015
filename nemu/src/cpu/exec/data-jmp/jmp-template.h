#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute() {
	swaddr_t opcode=instr_fetch(cpu.eip,1);
    DATA_TYPE_S addr=instr_fetch(cpu.eip+1,DATA_BYTE);
	if(opcode==0xeb||opcode==0xe9){
		cpu.eip=cpu.eip+addr;
		if(DATA_BYTE==2)cpu.eip=cpu.eip&0x0000FFFF;
	}
	else{
		cpu.eip=op_src->val;
		if(op_src->type==OP_TYPE_REG)
			cpu.eip-=2;
		else
			cpu.eip-=7;
		//printf("eip=\n",cpu.eip);
	}
	//print_asm("jmp %x",cpu.eip+DATA_BYTE+1);
	print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(rm)

make_helper(concat(jmp_ptr_, SUFFIX)) {
	DATA_TYPE_S addr=instr_fetch(cpu.eip+1,DATA_BYTE);
	swaddr_t segr = instr_fetch(cpu.eip+DATA_BYTE+1,2);
	cpu.cs.selector=segr;
	cpu.eip=addr-7;
	if(DATA_BYTE==2)cpu.eip=cpu.eip&0x0000ffff;
	print_asm("ljmp $0x%x 0x%x",segr,cpu.eip+7);
	return DATA_BYTE+3;
}









#include "cpu/exec/template-end.h"
