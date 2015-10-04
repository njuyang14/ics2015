#include "cpu/exec/template-start.h"

#define instr push

/*static void do_execute() {
        OPERAND_W(op_dest, op_src->val);
        print_asm_template2();
}*/

//make_instr_helper(r)

make_helper(concat(push_r_, SUFFIX)){
        //swaddr_t temp_addr=instr_fetch(cpu.eip+1,4);
        //cpu.eip=cpu.eip+temp_addr;
        print_asm_template1();
        return 1;
}

#include "cpu/exec/template-end.h"
