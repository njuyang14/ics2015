#include "cpu/exec/template-start.h"

#define instr inc

static void do_execute () {
	DATA_TYPE result = op_src->val + 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
	if(result==0)cpu.EFLAGS.ZF=1;
	else
	    cpu.EFLAGS.ZF=0;
	cpu.EFLAGS.SF=(result>>31)&1;
	DATA_TYPE ssrc=(op_src->val>>31)&1;
	
	if(ssrc==0&&ssrc!=cpu.EFLAGS.SF)cpu.EFLAGS.OF=1;

	//panic("please implement me");

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
