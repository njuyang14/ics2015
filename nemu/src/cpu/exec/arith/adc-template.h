#include "cpu/exec/template-start.h"

#define instr adc

static void do_execute() {
	DATA_TYPE result=op_dest->val+op_src->val+cpu.EFLAGS.CF;
	OPERAND_W(op_dest,result);
	if(result==0)
		cpu.EFLAGS.ZF=1;
	else
	    cpu.EFLAGS.ZF=0;
    DATA_TYPE_S result_s=result;
	cpu.EFLAGS.SF=(result_s>>(DATA_BYTE*8-1))&1;

	DATA_TYPE_S ssrc=(op_src->val>>(DATA_BYTE*8-1))&1;
	DATA_TYPE_S sdest=(op_dest->val>>(DATA_BYTE*8-1))&1;
	if(ssrc==sdest&&ssrc!=cpu.EFLAGS.SF)
		cpu.EFLAGS.OF=1;
	else
		cpu.EFLAGS.OF=0;
    if(cpu.EFLAGS.CF==0){
	    if(result<op_src->val&&result<op_dest->val)
	        cpu.EFLAGS.CF=1;
	    else
		    cpu.EFLAGS.CF=0;
	}
	else{
		if(result<=op_src->val&&result<=op_dest->val)
		    cpu.EFLAGS.CF=1;
		else
		    cpu.EFLAGS.CF=0;
	}
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
