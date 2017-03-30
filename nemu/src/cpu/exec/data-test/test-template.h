#include "cpu/exec/template-start.h"

#define instr test

static void do_execute() {
        //OPERAND_W(op_dest,op_src->val&op_dest->val);
        DATA_TYPE temp=op_src->val&op_dest->val;
        cpu.EFLAGS.ZF=(temp==0)?1:0;
        cpu.EFLAGS.CF=0;
        cpu.EFLAGS.OF=0;
	   	DATA_TYPE_S result_s=temp;
		cpu.EFLAGS.SF=(result_s>>(DATA_BYTE*8-1))&1;
        print_asm_template2();
}

make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(i2a)


#include "cpu/exec/template-end.h"
