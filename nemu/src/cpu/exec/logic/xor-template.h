#include "cpu/exec/template-start.h"

#define instr xor

static void do_execute () {
	DATA_TYPE src=op_src->val;
    DATA_TYPE dest=op_dest->val;
	DATA_TYPE result=dest^src;

	DATA_TYPE_S result_s=result;

	OPERAND_W(op_dest,result);

    cpu.EFLAGS.SF=(result_s>>(DATA_BYTE*8-1))&1;

	if(result_s==0)
		cpu.EFLAGS.ZF=1;
	else
		cpu.EFLAGS.ZF=0;

	/* TODO: Update EFLAGS. */
	cpu.EFLAGS.CF=0;
	cpu.EFLAGS.OF=0;
	//panic("please implement me");

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
