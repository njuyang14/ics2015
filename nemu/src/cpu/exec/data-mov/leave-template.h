#include "cpu/exec/template-start.h"

#define instr leave

static void do_execute() {
	        cpu.esp=cpu.ebp;//mov %ebp %esp
			//OPERAND_W(cpu.ebp,MEM_R(cpu.esp));
			MEM_W(cpu.ebp,MEM_R(cpu.esp));//pop ebp

}

make_helper(concat(leave_, SUFFIX)) {
	        //swaddr_t addr = instr_fetch(eip + 1, 4);
			//MEM_W(cpu.esp, cpu.ebp);
			do_execute();
			cpu.esp=cpu.esp+4;
			print_asm("leave\n");
			return 1;
}

#include "cpu/exec/template-end.h"


