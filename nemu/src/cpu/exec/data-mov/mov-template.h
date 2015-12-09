#include "cpu/exec/template-start.h"

#define instr mov

static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)
make_instr_helper(rm)

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	int sreg=3;
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX));

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	int sreg=3;
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

make_helper(concat(mov_r2sr_, SUFFIX)) {
	uint8_t reg_code=instr_fetch(eip+2,1);
	int len=decode_rm_l(eip+1);
	if(reg_code==0xd8){
		cpu.ds.selector=REG(op_src->reg);
	    uint8_t index=cpu.ds.selector;
		cpu.ds.limit=lnaddr_read(cpu.gdtr.base+index*8,2);
		cpu.ds.base=lnaddr_read(cpu.gdtr.base+index*8+2,2)+(lnaddr_read(cpu.gdtr.base+index*8+4,1)<<16)+(lnaddr_read(cpu.gdtr.base+index*8+7,1)<<24);
        print_asm("mov %%%s ds",REG_NAME(op_src->reg));
	}
	else if(reg_code==0xc0){
		cpu.es.selector=REG(op_src->reg);
		uint8_t index=cpu.es.selector;
		cpu.es.limit=lnaddr_read(cpu.gdtr.base+index*8,2);
		cpu.es.base=lnaddr_read(cpu.gdtr.base+index*8+2,2)+(lnaddr_read(cpu.gdtr.base+index*8+4,1)<<16)+(lnaddr_read(cpu.gdtr.base+index*8+7,1)<<24);
	    print_asm("mov %%%s es",REG_NAME(op_src->reg));
	}
	else if(reg_code==0xd0){
		cpu.ss.selector=REG(op_src->reg);
		uint8_t index=cpu.ss.selector;
		cpu.ss.limit=lnaddr_read(cpu.gdtr.base+index*8,2);
		cpu.ss.base=lnaddr_read(cpu.gdtr.base+index*8+2,2)+(lnaddr_read(cpu.gdtr.base+index*8+4,1)<<16)+(lnaddr_read(cpu.gdtr.base+index*8+7,1)<<24);
	    print_asm("mov %%%s ss",REG_NAME(op_src->reg));
	}
	return len+1;
}

make_helper(concat(mov_c2r_, SUFFIX)) {
    int len=decode_rm_l(eip+1);
	REG(op_src->reg)=cpu.cr0.val;
	//REG(op_src->reg)=cpu.cr3.val;
	print_asm("mov cr0 %%%s",REG_NAME(op_src->reg));
	return len+1;
}

make_helper(concat(mov_r2c_, SUFFIX)) {
	int len=decode_rm_l(eip+1);
	cpu.cr0.val=REG(op_src->reg);
	cpu.cr3.val=REG(op_src->reg);
	print_asm("mov %%%s cr0",REG_NAME(op_src->reg));
	return len+1;
}

#include "cpu/exec/template-end.h"
