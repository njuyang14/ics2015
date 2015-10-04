#include "cpu/exec/template-start.h"

#define instr je

/*static void do_execute() {
        OPERAND_W(op_dest, op_src->val);
        print_asm_template2();
}*/

//make_instr_helper(rel8)

make_helper(concat(je_rel8_, SUFFIX)){
        print_asm_template1();
        return 5;
}

#include "cpu/exec/template-end.h"
