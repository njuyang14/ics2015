#include "cpu/exec/template-start.h"

#define instr jmp

/*make_helper(concat(jmp_rel_, SUFFIX)){
        swaddr_t temp_addr=instr_fetch(cpu.eip+1,DATA_BYTE);
        cpu.eip=cpu.eip+temp_addr;
		if(DATA_BYTE==2)cpu.eip=cpu.eip&0x0000FFFF;
        print_asm("jmp %x",cpu.eip+DATA_BYTE+1);
        return DATA_BYTE+1;
}

make_helper(concat(jmp_rm_, SUFFIX)){
	swaddr_t temp_addr=cpu.eax;//instr_fetch(cpu.eip+1,1);
	cpu.eip=temp_addr;
	if(DATA_BYTE==2)cpu.eip=cpu.eip&0x0000FFFF;
	print_asm("jmp %x",cpu.eip+DATA_BYTE+1);
	return 2;
}

make_helper(concat(jmp_m16_, SUFFIX)){
	swaddr_t temp_addr=instr_fetch(cpu.eip+1,DATA_BYTE);
	cpu.eip=temp_addr;
	if(DATA_BYTE==2)cpu.eip=cpu.eip&0x0000FFFF;
	print_asm("jmp %x",cpu.eip+DATA_BYTE+1);
	return DATA_BYTE+1;
}

make_helper(concat(jmp_ptr_, SUFFIX)){
	swaddr_t temp_addr=instr_fetch(cpu.eip+1,DATA_BYTE);
	cpu.eip=temp_addr;
	if(DATA_BYTE==2)cpu.eip=cpu.eip&0x0000FFFF;
	print_asm("jmp %x",cpu.eip+DATA_BYTE+1);
	return DATA_BYTE+1;
}*/
static void do_execute() {
	swaddr_t opcode=instr_fetch(cpu.eip,1);
    DATA_TYPE_S addr=instr_fetch(cpu.eip+1,DATA_BYTE);
	if(opcode==0xeb||opcode==0xe9){
		cpu.eip=cpu.eip+addr;
		if(DATA_BYTE==2)cpu.eip=cpu.eip&0x0000FFFF;
	}
	/*else if(opcode==0xea){
		uint32_t segr = instr_fetch(cpu.eip+DATA_BYTE+1,2);
		cpu.cs=segr;
	    cpu.eip=addr;
	    if(DATA_BYTE==2)cpu.eip=cpu.eip&0x0000ffff;	
	}*/
	else{
		cpu.eip=op_src->val;
		if(op_src->type==OP_TYPE_REG)
			cpu.eip-=2;
		else
			cpu.eip-=7;
		//printf("eip=\n",cpu.eip);
		//To do: reason of cpu.eip-2.
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
	print_asm("ljmp $%x 0x%x",segr,cpu.eip);
	return DATA_BYTE+3;
}









#include "cpu/exec/template-end.h"
