#include "cpu/exec/template-start.h"

#define instr jcc


make_helper(concat(jcc_jbe_,SUFFIX)){
	        if(cpu.EFLAGS.CF==1||cpu.EFLAGS.ZF==1){
				int temp_addr=instr_fetch(eip+1,DATA_BYTE);
				int shift=32-(DATA_BYTE<<3);
				temp_addr=(temp_addr<<shift)>>shift;
				cpu.eip=cpu.eip+temp_addr;
            }
			print_asm("jbe %x",cpu.eip+DATA_BYTE+1);
			return DATA_BYTE+1;
}

make_helper(concat(jcc_jne_,SUFFIX)){
	            if(cpu.EFLAGS.ZF==0){
				int temp_addr=instr_fetch(eip+1,DATA_BYTE);
				int shift=32-(DATA_BYTE<<3);
				temp_addr=(temp_addr<<shift)>>shift;
				cpu.eip=cpu.eip+temp_addr;
		        }
				print_asm("jne %x",cpu.eip+DATA_BYTE+1);
				return DATA_BYTE+1;
}

make_helper(concat(jcc_jle_,SUFFIX)){
	            if(cpu.EFLAGS.ZF==1||(cpu.EFLAGS.SF!=cpu.EFLAGS.OF)){
				int temp_addr=instr_fetch(eip+1,DATA_BYTE);
				int shift=32-(DATA_BYTE<<3);
				temp_addr=(temp_addr<<shift)>>shift;
		        cpu.eip=cpu.eip+temp_addr;
				}
				print_asm("jle %x",cpu.eip+DATA_BYTE+1);
				return DATA_BYTE+1;
}

make_helper(concat(jcc_jg_,SUFFIX)){
	            if(cpu.EFLAGS.ZF==0&&(cpu.EFLAGS.SF==cpu.EFLAGS.OF)){
				int temp_addr=instr_fetch(eip+1,DATA_BYTE);
				int shift=32-(DATA_BYTE<<3);
				temp_addr=(temp_addr<<shift)>>shift;
				cpu.eip=cpu.eip+temp_addr;
			    }
				print_asm("jg %x",cpu.eip+DATA_BYTE+1);
				return DATA_BYTE+1;
}

make_helper(concat(jcc_jge_,SUFFIX)){
	            if(cpu.EFLAGS.SF==cpu.EFLAGS.OF){
					int temp_addr=instr_fetch(eip+1,DATA_BYTE);
					int shift=32-(DATA_BYTE<<3);
					temp_addr=(temp_addr<<shift)>>shift;
				    cpu.eip=cpu.eip+temp_addr;
				}
				print_asm("jge %x",cpu.eip+DATA_BYTE+1);
				return DATA_BYTE+1;
}

make_helper(concat(jcc_jl_,SUFFIX)){
	            if(cpu.EFLAGS.SF!=cpu.EFLAGS.OF){
					int temp_addr=instr_fetch(eip+1,DATA_BYTE);
				    int shift=32-(DATA_BYTE<<3);
					temp_addr=(temp_addr<<shift)>>shift;
				    cpu.eip=cpu.eip+temp_addr;
				}
				print_asm("jl %x",cpu.eip+DATA_BYTE+1);
				return DATA_BYTE+1;
}

make_helper(concat(jcc_ja_,SUFFIX)){
	            if(cpu.EFLAGS.CF==0&&cpu.EFLAGS.ZF==0){
				int temp_addr=instr_fetch(eip+1,DATA_BYTE);
			    int shift=32-(DATA_BYTE<<3);
				temp_addr=(temp_addr<<shift)>>shift;
				cpu.eip=cpu.eip+temp_addr;
				}
				print_asm("ja %x",cpu.eip+DATA_BYTE+1);
				return DATA_BYTE+1;
}

make_helper(concat(jcc_js_,SUFFIX)){
	            if(cpu.EFLAGS.SF==1){
						int temp_addr=instr_fetch(eip+1,DATA_BYTE);
						int shift=32-(DATA_BYTE<<3);
						temp_addr=(temp_addr<<shift)>>shift;
						cpu.eip=cpu.eip+temp_addr;
				}
				print_asm("js %x",cpu.eip+DATA_BYTE+1);
				return DATA_BYTE+1;
}

make_helper(concat(jcc_jae_,SUFFIX)){
	            if(cpu.EFLAGS.CF==0){
					int temp_addr=instr_fetch(eip+1,DATA_BYTE);
					int shift=32-(DATA_BYTE<<3);
					temp_addr=(temp_addr<<shift)>>shift;
					cpu.eip=cpu.eip+temp_addr;
				}
				print_asm("jae %x",cpu.eip+DATA_BYTE+1);
				return DATA_BYTE+1;
}

make_helper(concat(jcc_jns_,SUFFIX)){
	            if(cpu.EFLAGS.CF==0){
					int temp_addr=instr_fetch(eip+1,DATA_BYTE);
					int shift=32-(DATA_BYTE<<3);
					temp_addr=(temp_addr<<shift)>>shift;
					cpu.eip=cpu.eip+temp_addr;
				}
				print_asm("jns %x",cpu.eip+DATA_BYTE+1);
				return DATA_BYTE+1;
}

make_helper(concat(jcc_jo_,SUFFIX)){
	        if(cpu.EFLAGS.OF==1){
				int temp_addr=instr_fetch(eip+1,DATA_BYTE);
				int shift=32-(DATA_BYTE<<3);
				temp_addr=(temp_addr<<shift)>>shift;
				cpu.eip=cpu.eip+temp_addr;
			}
			print_asm("jo %x",cpu.eip+DATA_BYTE+1);
			return DATA_BYTE+1;
}


make_helper(concat(jcc_jno_,SUFFIX)){
	                if(cpu.EFLAGS.OF==0){
						int temp_addr=instr_fetch(eip+1,DATA_BYTE);
					    int shift=32-(DATA_BYTE<<3);
						temp_addr=(temp_addr<<shift)>>shift;
						cpu.eip=cpu.eip+temp_addr;
					}
					print_asm("jno %x",cpu.eip+DATA_BYTE+1);
					return DATA_BYTE+1;
}

make_helper(concat(jcc_jnae_,SUFFIX)){
	                if(cpu.EFLAGS.CF==1){
						int temp_addr=instr_fetch(eip+1,DATA_BYTE);
						int shift=32-(DATA_BYTE<<3);
						temp_addr=(temp_addr<<shift)>>shift;
						cpu.eip=cpu.eip+temp_addr;
					}
					print_asm("jnae %x",cpu.eip+DATA_BYTE+1);
					return DATA_BYTE+1;
}

make_helper(concat(jcc_jp_,SUFFIX)){
	                if(cpu.EFLAGS.PF==1){
						int temp_addr=instr_fetch(eip+1,DATA_BYTE);
						int shift=32-(DATA_BYTE<<3);
					    temp_addr=(temp_addr<<shift)>>shift;
						cpu.eip=cpu.eip+temp_addr;
					}
					print_asm("jp %x",cpu.eip+DATA_BYTE+1);
					return DATA_BYTE+1;
}

make_helper(concat(jcc_jpo_,SUFFIX)){
	                if(cpu.EFLAGS.PF==0){
					int temp_addr=instr_fetch(eip+1,DATA_BYTE);
					int shift=32-(DATA_BYTE<<3);
			        temp_addr=(temp_addr<<shift)>>shift;
					cpu.eip=cpu.eip+temp_addr;
					}
					print_asm("jpo %x",cpu.eip+DATA_BYTE+1);
					return DATA_BYTE+1;
}

/*make_helper(concat(jcc_jns_,SUFFIX)){
	if(cpu.EFLAGS.CF==0){
        int temp_addr=instr_fetch(eip+1,DATA_BYTE);
		int shift=32-(DATA_BYTE<<3);
		temp_addr=(temp_addr<<shift)>>shift;
		cpu.eip=cpu.eip+temp_addr;
    }
	print_asm("jns %x",cpu.eip+DATA_BYTE+1);
	return DATA_BYTE+1;
}*/





#include "cpu/exec/template-end.h"
