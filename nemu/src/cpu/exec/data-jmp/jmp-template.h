#include "cpu/exec/template-start.h"

#define instr jmp

/*static void do_execute() {
        //OPERAND_W(op_dest, op_src->val);
        print_asm_template2();
}*/

/*make_instr_helper(rel)
make_instr_helper(rm)
make_instr_helper(m16)
make_instr_helper(ptr)*/

make_helper(concat(jmp_rel_, SUFFIX)){
        swaddr_t temp_addr=instr_fetch(cpu.eip+1,DATA_BYTE);
        cpu.eip=temp_addr;

        print_asm_template1();
        return 2;
}

make_helper(concat(jmp_rm_, SUFFIX)){
	       // swaddr_t temp_addr=instr_fetch(cpu.eip+1,4);
			 //       cpu.eip=cpu.eip+temp_addr;

					        print_asm_template1();
							        return 2;
}

make_helper(concat(jmp_m16_, SUFFIX)){
	        //swaddr_t temp_addr=instr_fetch(cpu.eip+1,4);
			  //      cpu.eip=cpu.eip+temp_addr;

					        print_asm_template1();
							        return 2;
}

make_helper(concat(jmp_ptr_, SUFFIX)){
	        //swaddr_t temp_addr=instr_fetch(cpu.eip+1,4);
			//        cpu.eip=cpu.eip+temp_addr;

					        print_asm_template1();
							        return 2;
}

#include "cpu/exec/template-end.h"
