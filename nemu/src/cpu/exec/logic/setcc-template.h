#include "cpu/exec/template-start.h"

#define instr setcc


#if DATA_BYTE==1
static void do_execute() {
	DATA_TYPE opcode=instr_fetch(cpu.eip+1,1);
	if(opcode==0x90){
		if(cpu.EFLAGS.OF==1)op_src->val=1;
		else op_src->val=0;
	}
	else if(opcode==0x91){
		if(cpu.EFLAGS.OF==0)op_src->val=1;
		else op_src->val=0;
	}
	else if(opcode==0x92){
		if(cpu.EFLAGS.CF==1)op_src->val=1;
		        else op_src->val=0;
	}
	else if(opcode==0x93){
		if(cpu.EFLAGS.CF==0)op_src->val=1;
		        else op_src->val=0;
	}
	else if(opcode==0x94){
		if(cpu.EFLAGS.ZF==1)op_src->val=1;
		        else op_src->val=0;
	}
	else if(opcode==0x95){
		if(cpu.EFLAGS.ZF==0)op_src->val=1;
		        else op_src->val=0;
	}
	else if(opcode==0x96){
	if(cpu.EFLAGS.CF==1||cpu.EFLAGS.ZF==1)op_src->val=1;
	        else op_src->val=0;
	}
	else if(opcode==0x97){
	if(cpu.EFLAGS.CF==0&&cpu.EFLAGS.ZF==0)op_src->val=1;
	        else op_src->val=0;
	}
	else if(opcode==0x98){
	if(cpu.EFLAGS.SF==1)op_src->val=1;
	        else op_src->val=0;
	}
	else if(opcode==0x99){
	if(cpu.EFLAGS.SF==0)op_src->val=1;
	        else op_src->val=0;
	}
	else if(opcode==0x9a){
	if(cpu.EFLAGS.PF==1)op_src->val=1;
	        else op_src->val=0;
	}
	else if(opcode==0x9b){
	if(cpu.EFLAGS.PF==0)op_src->val=1;
	        else op_src->val=0;
	}
	else if(opcode==0x9c){
	if(cpu.EFLAGS.SF!=cpu.EFLAGS.OF)op_src->val=1;
	        else op_src->val=0;
	}
	else if(opcode==0x9d){
	if(cpu.EFLAGS.SF!=cpu.EFLAGS.OF)op_src->val=1;
	        else op_src->val=0;
	}
	else if(opcode==0x9e){
	if(cpu.EFLAGS.ZF==1||(cpu.EFLAGS.SF!=cpu.EFLAGS.OF))op_src->val=1;
	        else op_src->val=0;
	}
	else if(opcode==0x9f){
	if(cpu.EFLAGS.ZF==0&&(cpu.EFLAGS.SF==cpu.EFLAGS.OF))op_src->val=1;
	        else op_src->val=0;
	}
	print_asm_template1();

}

make_instr_helper(rm)
#endif

/*make_helper(concat(setcc_setne_,SUFFIX)){
	            if(cpu.EFLAGS.ZF==0){
				cpu.gpr[0]._16=1;									
				}
				else
					cpu.gpr[0]._16=0; 
				print_asm("setne %%al");
				return 2;
}*/

#include "cpu/exec/template-end.h"
