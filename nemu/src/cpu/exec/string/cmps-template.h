#include "cpu/exec/template-start.h"

#define instr cmps

make_helper(concat(cmps_m_, SUFFIX)) {
    DATA_TYPE src=swaddr_read(cpu.esi,4);
	DATA_TYPE dest=swaddr_read(cpu.edi,4);
	DATA_TYPE_S temp=dest+~(src)+1;

	DATA_TYPE_S s_dest=(dest>>31)&1;
	DATA_TYPE_S s_src=(src>>31)&1;
	DATA_TYPE_S s_temp=(temp>>31)&1;

	DATA_TYPE_S dest_and_src=!!(s_dest^s_src);
	DATA_TYPE_S dest_and_temp=!!(s_dest^s_temp);
	cpu.EFLAGS.OF=!!(dest_and_src&dest_and_temp);

	cpu.EFLAGS.SF=s_temp;

	if(temp==0){
		cpu.EFLAGS.ZF=1;
	}
    else{
	    cpu.EFLAGS.ZF=0;
		if(temp<0)cpu.EFLAGS.CF=1;
		else if(temp>0)cpu.EFLAGS.CF=0;
	}
	print_asm("cmps");
	return 1;
}
#include "cpu/exec/template-end.h"
