#include "cpu/exec/template-start.h"

#define instr adc

static void do_execute() {
	DATA_TYPE result=op_dest->val+op_src->val+cpu.EFLAGS.CF;
	OPERAND_W(op_dest,result);
	if(result==0)
		cpu.EFLAGS.ZF=1;
	else
	    cpu.EFLAGS.ZF=0;

	cpu.EFLAGS.SF=(result>>31)&1;
	DATA_TYPE ssrc=(op_src->val>>31)&1;
	DATA_TYPE sdest=(op_dest->val>>31)&1;
	if(ssrc==sdest&&ssrc!=cpu.EFLAGS.SF)
		cpu.EFLAGS.OF=1;
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
