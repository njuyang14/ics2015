#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute() {
    DATA_TYPE src=op_src->val;
	DATA_TYPE dest=op_dest->val;
	DATA_TYPE result=dest-src;

	DATA_TYPE_S src_s=src;
	DATA_TYPE_S dest_s=dest;
	DATA_TYPE_S result_s=result;

    OPERAND_W(op_dest,result);

	cpu.EFLAGS.SF=(result_s>>(DATA_BYTE*8-1))&1;

	DATA_TYPE_S ssrc=(src_s>>(DATA_BYTE*8-1))&1;
	DATA_TYPE_S sdest=(dest_s>>(DATA_BYTE*8-1))&1;
	DATA_TYPE_S sresult=(result_s>>(DATA_BYTE*8-1))&1;

	if(sresult==ssrc&&ssrc!=sdest)
		cpu.EFLAGS.OF=1;
	 else
		cpu.EFLAGS.OF=0;

	if(result_s==0)
		cpu.EFLAGS.ZF=1;
	else
	    cpu.EFLAGS.ZF=0;

	if(cpu.EFLAGS.CF==0){
		if(dest_s<src_s)
			cpu.EFLAGS.CF=1;
		else
			cpu.EFLAGS.CF=0;
	}
	else{
		if(dest_s<=src_s)
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
