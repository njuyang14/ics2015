#include "cpu/exec/template-start.h"

#define instr jmp

make_helper(concat(jmp_rel_, SUFFIX)){
        swaddr_t temp_addr=instr_fetch(cpu.eip+1,DATA_BYTE);
        cpu.eip=cpu.eip+temp_addr;
		if(DATA_BYTE==2)cpu.eip=cpu.eip&0x0000FFFF;
        //printf("databute=%d\n",DATA_BYTE);
        print_asm("jmp %x",cpu.eip+DATA_BYTE+1);
        return DATA_BYTE+1;
}

make_helper(concat(jmp_rm_, SUFFIX)){
		print_asm("implement\n");
		return 2;
}

make_helper(concat(jmp_m16_, SUFFIX)){
		print_asm("implement\n");
		return 2;
}

make_helper(concat(jmp_ptr_, SUFFIX)){
		print_asm("implement\n");
		return 2;
}

#include "cpu/exec/template-end.h"
