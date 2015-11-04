#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);
	DATA_TYPE_S result_s=result;

	/* TODO: Update EFLAGS. */
	//panic("please implement me");
	if(result==0)cpu.EFLAGS.ZF=1;
	else
		cpu.EFLAGS.ZF=0;
	cpu.EFLAGS.SF=(result_s>>31)&1;
	DATA_TYPE ssrc=(op_src->val>>31)&1;

	if(ssrc!=0&&0==cpu.EFLAGS.SF)cpu.EFLAGS.OF=1;
	else
		cpu.EFLAGS.OF=0;

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
