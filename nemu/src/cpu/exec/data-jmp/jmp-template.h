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
    swaddr_t addr=instr_fetch(cpu.eip+1,DATA_BYTE);
	if(opcode==0xeb||opcode==0xe9){
		cpu.eip=cpu.eip+addr;
		if(DATA_BYTE==2)cpu.eip=cpu.eip&0x0000FFFF;
	}
	print_asm("jmp %x",cpu.eip+DATA_BYTE+1);
}

make_instr_helper(i)
make_instr_helper(rm)











#include "cpu/exec/template-end.h"
