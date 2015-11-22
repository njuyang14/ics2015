#include "common.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
uint32_t check_cache1(hwaddr_t addr,size_t len);
void  read_cache1_miss(hwaddr_t addr,size_t len);
uint32_t read_cache1_hit(hwaddr_t addr,size_t len);

void write_hit_cache1(hwaddr_t addr, size_t len, uint32_t data);
/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	
	/*if(check_cache1(addr,len)==1){
		//printf("read hit cache1\n");
		return read_cache1_hit(addr,len);
	}
	//else if(check_cache2(addr,len)==1){}
	else{
		//printf("read miss cache1\n");
		read_cache1_miss(addr,len);
		return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	}*/
	
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	dram_write(addr, len, data);
	/*if(check_cache1(addr,len)==1){
		write_hit_cache1(addr, len, data);
	//	printf("write hit cache1\n");
	}
	//else if(write_hit_cache1()==1)
	else{
		dram_write(addr, len, data);
	//	printf("write miss cache1\n"); 
	}*/
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_write(addr, len, data);
}

