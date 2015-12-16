#include "common.h"
#include "nemu.h"
#include "tlb.h"
#include <stdlib.h>
#include <time.h>

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

hwaddr_t page_translate(lnaddr_t addr){
	if(cpu.cr0.protect_enable==0||cpu.cr0.paging==0)
		return (hwaddr_t)addr;
	/* IA32_PAGE */
	int i,flag=0;
	for(i=0;i<64;i++){
		if(tlb[i].tag==(addr>>12)&&tlb[i].valid==1){
			flag=1;//hit
			break;
		}
	}
	if(flag==1){
		uint32_t offset = addr & 0xfff;
		return offset+((tlb[i].page_data>>12)<<12);
	}
	else
	{
	    uint32_t dir = addr >> 22;
	    uint32_t page = ( addr >> 12 )& 0x3ff;
	    uint32_t offset = addr & 0xfff;
	    uint32_t page_base=hwaddr_read((cpu.cr3.page_directory_base<<12)+dir*4,4)>>12;
	    //Assert((page_base&1)==1,"ad=%x\n",page_base);
		srand(time(0)+clock());
		int no=rand()%64;
		tlb[no].valid=1;
		tlb[no].tag=(addr>>12);
		tlb[no].page_data=hwaddr_read((page_base<<12)+page*4,4);//TLB read

        return offset+((hwaddr_read((page_base<<12)+page*4,4)>>12)<<12);
	}
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	hwaddr_t hwaddr = page_translate(addr);
	return hwaddr_read(hwaddr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_t hwaddr = page_translate(addr);
	hwaddr_write(hwaddr, len, data);
}

lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg){
	if(cpu.cr0.protect_enable==0)return (lnaddr_t)addr;
#ifdef DEBUG
	    assert(sreg == 0 || sreg == 1 || sreg == 2 || sreg == 3);
#endif
    uint32_t base0 = cpu.segdesc[sreg].base_15_0;
	uint32_t base1 = cpu.segdesc[sreg].base_23_16<<16;
	uint32_t base2 = cpu.segdesc[sreg].base_31_24<<24;
	return (lnaddr_t)( base0 + base1 + base2 + addr);
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

