#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	int sreg=2;
        if(DATA_BYTE==4||DATA_BYTE==1)
            cpu.esp=cpu.esp-4;
		else if(DATA_BYTE==2)
			cpu.esp=cpu.esp-2;
        MEM_W(cpu.esp,op_src->val);
        print_asm_template1();
}

make_instr_helper(r)



#include "cpu/exec/template-end.h"
