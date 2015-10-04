#include "cpu/exec/template-start.h"

#define instr je

/*static void do_execute() {
        if(cpu.EFLAGS.ZF==1){
                 swaddr_t temp_addr=instr_fetch(cpu.eip+1,1);
                 cpu.eip=cpu.eip+temp_addr;
        }
        print_asm_template1();
}*/

//make_instr_helper(rel8)

make_helper(concat(je_rel8_, SUFFIX)){
        if(cpu.EFLAGS.ZF==1){
                 swaddr_t temp_addr=instr_fetch(cpu.eip+1,1);
                 cpu.eip=cpu.eip+temp_addr;
        }
        print_asm_template1();
        return 2;
}

#include "cpu/exec/template-end.h"
