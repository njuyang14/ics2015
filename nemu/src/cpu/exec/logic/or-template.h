#include "cpu/exec/template-start.h"

#define instr or

static void do_execute () {
	DATA_TYPE result = op_dest->val | op_src->val;
	OPERAND_W(op_dest, result);
	DATA_TYPE_S result_s=result;

	/* TODO: Update EFLAGS. */
	cpu.EFLAGS.CF=0;
	cpu.EFLAGS.OF=0;
	if(result==0)cpu.EFLAGS.ZF=0;
	else
		cpu.EFLAGS.ZF=0;

	 cpu.EFLAGS.SF=(result_s>>(DATA_BYTE*8-1))&1;
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
