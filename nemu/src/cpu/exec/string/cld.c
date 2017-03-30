#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h" 
make_helper(cld){
	cpu.EFLAGS.DF=0;
	print_asm("cld");
	return 1;
}
