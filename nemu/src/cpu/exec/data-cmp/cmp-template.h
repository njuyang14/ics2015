#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute() {
        //OPERAND_W(op_dest, op_src->val);
        DATA_TYPE temp=op_dest->val+~(op_src->val)+1;
		int s_dest=(op_dest->val>>31)&1;
		int s_src=(op_src->val>>31)&1;
		int s_temp=(temp>>31)&1;
		int dest_and_src=!!(s_dest^s_src);
		int dest_and_temp=!!(s_dest^s_temp);
		cpu.EFLAGS.OF=!!(dest_and_src&dest_and_temp);
		cpu.EFLAGS.SF=s_temp;

        if(temp==0){
               cpu.EFLAGS.ZF=1;
        }
        else{
               cpu.EFLAGS.ZF=0;
        }

		if(op_dest->val<op_src->val)cpu.EFLAGS.CF=1;
		else
			cpu.EFLAGS.CF=0;
        print_asm_template2();
}

make_instr_helper(rm2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(i2a)
#if DATA_BYTE==2||DATA_BYTE==4
make_instr_helper(si2rm)
#endif


#include "cpu/exec/template-end.h"
