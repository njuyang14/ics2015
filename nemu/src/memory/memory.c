#include "common.h"
#include "nemu.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
uint32_t check_cache1(hwaddr_t addr,size_t len);
void  read_cache1_miss(hwaddr_t addr,size_t len);
uint32_t read_cache1_hit(hwaddr_t addr,size_t len);

uint32_t check_cache2(hwaddr_t addr, size_t len);
void read_cache2_miss(hwaddr_t addr,size_t len);
uint32_t read_cache2_hit(hwaddr_t addr,size_t len);
void write_hit_cache2(hwaddr_t addr, size_t len, uint32_t data);
void write_allocate(hwaddr_t addr, size_t len, uint32_t data);

void write_hit_cache1(hwaddr_t addr, size_t len, uint32_t data);
/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	//return dram_read(addr, len) & (~0u >> ((4 - len) << 3));	
	if(check_cache1(addr,len)==1){
		//printf("read hit cache1\n");
		return read_cache1_hit(addr,len);
	}
	else if(check_cache2(addr,len)==1){
		read_cache1_miss(addr,len);
		return read_cache2_hit(addr,len);
	}
	else{
		//printf("read miss cache1\n");
		read_cache2_miss(addr,len);
		read_cache1_miss(addr,len);
		return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	}
	
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	//dram_write(addr, len, data);
	if(check_cache1(addr,len)==1){
		write_hit_cache1(addr, len, data);
	//	printf("write hit cache1\n");
	}
	else if(check_cache2(addr,len)==1){
		write_hit_cache2(addr, len, data);//write back
	}
	else{
		write_allocate(addr, len, data);
		//dram_write(addr, len, data);
	//	printf("write miss cache1\n"); 
	}
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}

uint32_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg){
	if(cpu.cr0.protect_enable==0)return addr;
	uint16_t index;
	if(sreg==0)
		index=cpu.es.selector>>3;
	else if(sreg==1)
		index=cpu.cs.selector>>3;
	else if(sreg==2)
		index=cpu.ss.selector>>3;
	else //if(sreg==3)
		index=cpu.ds.selector>>3;
    uint32_t base0=lnaddr_read(cpu.gdtr.base+index*8+2,2)<<24;
	uint32_t base1=lnaddr_read(cpu.gdtr.base+index*8+4,1)<<16;
	uint32_t base2=lnaddr_read(cpu.gdtr.base+index*8+7,1);
	return base0+base1+base2+addr;
}

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data,uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	lnaddr_write(lnaddr, len, data);
}

