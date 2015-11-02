#include "cpu/exec/template-start.h"

#define instr call

static void do_execute() {
	swaddr_t opcode=instr_fetch(cpu.eip,1);
	DATA_TYPE_S addr=instr_fetch(cpu.eip+1,DATA_BYTE);
	if(opcode==0xe8){
		cpu.esp-=DATA_BYTE;
		MEM_W(cpu.esp,cpu.eip+DATA_BYTE);
		cpu.eip=cpu.eip+addr;
		if(DATA_BYTE==2)cpu.eip&=0x0000FFFF;
		print_asm("call %x",cpu.eip);
	}
	else{
		cpu.esp-=DATA_BYTE;
		MEM_W(cpu.esp,cpu.eip+DATA_BYTE);
		cpu.eip=op_src->val;
		if(DATA_BYTE==2)cpu.eip&=0x0000FFFF;
        //OPERAND_W(op_dest, op_src->val);
		if(op_src->type==OP_TYPE_REG)cpu.eip-=2;
        print_asm_template1();
	}
}

make_instr_helper(i)
make_instr_helper(rm)

/*make_helper(concat(call_rel32_, SUFFIX)){
        cpu.esp=cpu.esp-4;
        MEM_W(cpu.esp,cpu.eip+DATA_BYTE);
        swaddr_t temp_addr=instr_fetch(cpu.eip+1,4);
        cpu.eip=cpu.eip+temp_addr;

        //snprintf(op_src->str, OP_STR_SIZE, "%x <main>", cpu.eip);
        print_asm("call %x",cpu.eip);  
        return 5; 
}*/

#include "cpu/exec/template-end.h"
