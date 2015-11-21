#include<common.h>
#include<stdlib.h>      
#include<time.h> 

typedef struct{
	uint8_t valid;
	uint16_t tag;//len=14
	uint8_t offset[64];
}cache1;
cache1 L1[128][8];

typedef struct{
	uint8_t valid;
	uint8_t dirty;
	uint16_t tag;//len=9
	uint8_t offset[64];
}cache2;
cache2 L2[4096][16];

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

void init_cache(){
	int i,j;
	for(i=0;i<128;i++)
		for(j=0;j<8;j++)
			L1[i][j].valid=0;
	for(i=0;i<4096;i++)
		for(j=0;j<16;j++)
		    L2[i][j].valid=0;
}

uint32_t check_cache1(hwaddr_t addr,size_t len){
	uint8_t tag_in_dram = addr >> 15;
	uint8_t cache_no = ( addr >> 8 ) & 0x7f;
	//uint8_t offset = addr & 0xff;
	int i;
	for( i=0; i<8; i++){
		if(tag_in_dram==L1[cache_no][i].tag&&L1[cache_no][i].valid==1){
			return 1;
		}
	}
	return 0;
}
void  read_cache1_miss(hwaddr_t addr,size_t len){
	uint8_t tag_in_dram = addr >> 15;
	uint8_t cache_no = ( addr >> 8 ) & 0x7f;
	uint8_t offset = addr & 0xff;

	srand((unsigned)time(0));
	int i=(rand()%(8-0+1))+ 0;

	L1[cache_no][i].valid = 1;
	L1[cache_no][i].tag=tag_in_dram;
	int j;
	addr = addr - offset;
	for(j=0;j<64;j++){
		L1[cache_no][i].offset[j]=dram_read(addr+j,1);
	}
}

uint32_t read_cache1_hit(hwaddr_t addr,size_t len){
	/*uint8_t tag_in_dram = addr >> 15;
	uint8_t cache_no = ( addr >> 8 ) & 0x7f;
	uint8_t offset = addr & 0xff;*/

	uint32_t data = dram_read(addr+len-1,1);
	while(len-1>0){
		data = ( data << 8 ) + dram_read(addr+len-2,1);
		len--;
	}
	return data;
}













