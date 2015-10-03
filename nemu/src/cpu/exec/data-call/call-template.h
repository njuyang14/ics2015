#include "cpu/exec/template-start.h"

#define instr call

/*static void do_execute() {
        OPERAND_W(op_dest, op_src->val);
        print_asm_template2();
}*/

//make_instr_helper(rel32)

make_helper(concat(call_rel32_, SUFFIX)){
        
        return 5; 
}

#include "cpu/exec/template-end.h"
