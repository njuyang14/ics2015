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
		for(j=0;j<16;j++){
		    L2[i][j].valid=0;
			L2[i][j].dirty=0;
		}
}

uint32_t check_cache1(hwaddr_t addr, size_t len){
	uint16_t tag_in_dram = addr >> 13;
	uint8_t cache_no = ( addr >> 6 ) & 0x7f;
	//uint8_t offset = addr & 0x3f;
	int i;
	for( i=0; i<8; i++){
		if(tag_in_dram==L1[cache_no][i].tag&&L1[cache_no][i].valid==1){
			return 1;
		}
	}
	return 0;
}

void read_cache1_miss(hwaddr_t addr,size_t len){
	uint16_t tag_in_dram = addr >> 13;
	uint8_t cache_no = ( addr >> 6 ) & 0x7f;
	uint8_t offset = addr & 0x3f;

	uint16_t tag_in_dram2 = addr >> 18;
	uint8_t cache_no2 = ( addr >> 6 ) & 0xfff;
	//uint8_t offset2 = addr & 0x3f;

	srand(time(0)+clock());
	int i=rand()%8;
    //printf("rand=%d\n",i);
	L1[cache_no][i].valid = 1;
	L1[cache_no][i].tag=tag_in_dram;
	int j,block_no=0;
	addr = addr - offset;
	for(j=0;j<16;j++){
		if(tag_in_dram2==L2[cache_no2][j].tag&&L2[cache_no2][j].valid==1){
			block_no=j;
			break;
		}
	}
	for(j=0;j<64;j++){
		L1[cache_no][i].offset[j]=L2[cache_no2][block_no].offset[j];
	}
}

uint32_t hwaddr_read(hwaddr_t addr, size_t len);
void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data);

uint32_t read_cache1_hit(hwaddr_t addr,size_t len){
    int block_no=0;
	
	uint16_t tag_in_dram = addr >> 13;
	uint8_t cache_no = ( addr >> 6 ) & 0x7f;
	uint8_t offset = addr & 0x3f;

    int i;
    for( i=0; i<8; i++){
		if(tag_in_dram==L1[cache_no][i].tag&&L1[cache_no][i].valid==1){
			block_no=i;
			break;
		}
	}
	uint32_t data;
	if(offset+len<=64){
		data = L1[cache_no][block_no].offset[offset+len-1];
	    while(len-1>0){
		    data=(data<<8)+L1[cache_no][block_no].offset[offset+(--len)-1];
	    }
	}
	else{
		int offset_tmp=63;
		data = L1[cache_no][block_no].offset[offset_tmp];
		int cnt=len-(offset+len-64);
		//printf("cnt=%d\n",cnt);
		int j=1;
		while(j<cnt){
			offset_tmp--;
			data=(data<<8)+L1[cache_no][block_no].offset[offset_tmp];
			j++;
		}
		//printf("nextaddr=%x\n",((addr+0x40)>>6)<<6);
		uint32_t temp=hwaddr_read(((addr+0x40)>>6)<<6,offset+len-64);
		data=(temp<<((len-(offset+len-64))*8))+data;
		//printf("data=%x\n",data);
	}
	return data;
}


void write_hit_cache1(hwaddr_t addr, size_t len, uint32_t data){
	/* write through */

	/* write cache1*/
	uint16_t tag_in_dram = addr >> 13;
	uint8_t cache_no = ( addr >> 6 ) & 0x7f;
	uint8_t offset = addr & 0x3f;
	size_t tmplen=len;
	uint32_t data2=data;
	int i,block_no=0;
	for( i=0; i<8; i++){
		if(tag_in_dram==L1[cache_no][i].tag&&L1[cache_no][i].valid==1){
			block_no=i;
			break;
		}
	}
	int j,cnt;
	if(offset+len<=64){
	    while(len>0){
		    L1[cache_no][block_no].offset[offset+len-1]=(data>>((len-1)*8))&0xff;
		    len--;
	    }
	}
	else{
		//int offset_tmp=63;
		cnt=len-(offset+len-64);
		j=0;
		while(j<cnt){
			L1[cache_no][block_no].offset[offset]=data&0xff;
			data=data>>8;
			j++;
			offset++;
		}
		hwaddr_write(((addr+0x40)>>6)<<6, len-cnt, data);
	}
	/* write cache2 */
	uint16_t tag_in_dram2 = addr >> 18;
	uint8_t cache_no2 = ( addr >> 6 ) & 0xfff;
	uint8_t offset2 = addr & 0x3f;
	for( i=0; i<16; i++){
	    if(tag_in_dram2==L2[cache_no2][i].tag&&L1[cache_no2][i].valid==1){
		    block_no=i;
		    break;
		}
	}
	if(offset2+tmplen<=64){
	    while(tmplen>0){
		    L2[cache_no2][block_no].offset[offset2+tmplen-1]=(data2>>((tmplen-1)*8))&0xff;
		    tmplen--;
	    }
	}
	else{
		cnt=tmplen-(offset2+tmplen-64);
		j=0;
		while(j<cnt){
			L2[cache_no2][block_no].offset[offset2]=data2&0xff;
			data2=data2>>8;
			j++;
			offset2++;
		}
		hwaddr_write(((addr+0x40)>>6)<<6, len-cnt, data2);
	}
	//printf("hit cache1\n");
}

uint32_t check_cache2(hwaddr_t addr, size_t len){
	uint32_t tag_in_dram = addr >> 18;
	uint16_t cache_no = ( addr >> 6 ) & 0xfff;
	int i;
	for(i=0;i<16;i++){
		if(tag_in_dram==L2[cache_no][i].tag&&L2[cache_no][i].valid==1){
			return 1;
		}
	}
	return 0;
}

uint32_t read_cache2_hit(hwaddr_t addr,size_t len){
    int block_no=0;
    uint16_t tag_in_dram = addr >> 18;
	uint16_t cache_no = ( addr >> 6 ) & 0xfff;
    uint8_t offset = addr & 0x3f;

	int i;
	for( i=0; i<16; i++){
        if(tag_in_dram==L2[cache_no][i].tag&&L2[cache_no][i].valid==1){
	        block_no=i;
			break;
		}
	}
	uint32_t data;
	if(offset+len<=64){
		data = L2[cache_no][block_no].offset[offset+len-1];
		while(len-1>0){
			data=(data<<8)+L2[cache_no][block_no].offset[offset+(--len)-1];
		}
	}
	else{
		int offset_tmp=63;
		data = L2[cache_no][block_no].offset[offset_tmp];
		int cnt=len-(offset+len-64);
		int j=1;
		while(j<cnt){
			offset_tmp--;
			data=(data<<8)+L2[cache_no][block_no].offset[offset_tmp];
			j++;
        }
		uint32_t temp=hwaddr_read(((addr+0x40)>>6)<<6,offset+len-64);
		data=(temp<<((len-(offset+len-64))*8))+data;
	}
	return data;
}

void read_cache2_miss(hwaddr_t addr,size_t len){
	uint16_t tag_in_dram = addr >> 18;
	uint16_t cache_no = ( addr >> 6 ) & 0xfff;
	uint8_t offset = addr & 0x3f;
	srand(time(0)+clock());
	int i=rand()%16;
	//printf("rand=%d\n",i);
	if(L2[cache_no][i].dirty==1&&L2[cache_no][i].valid == 1){
	    L2[cache_no][i].dirty=0;
		/* to do: write dram */
		uint32_t init_addr=(L2[cache_no][i].tag<<18)+(cache_no<<6);
		int idx=0;
		while(idx<64){
			dram_write(init_addr,1,L2[cache_no][i].offset[idx]);
			idx++;
			init_addr++;
		}
	}
	L2[cache_no][i].valid = 1;
	L2[cache_no][i].tag=tag_in_dram;
	int j;
	addr = addr - offset;
	for(j=0;j<64;j++){
		L2[cache_no][i].offset[j]=dram_read(addr+j,1);
	}
}

void write_hit_cache2(hwaddr_t addr, size_t len, uint32_t data){
	/* write back */
    uint16_t tag_in_dram = addr >> 18;
	uint16_t cache_no = ( addr >> 6 ) & 0xfff;
	uint8_t offset = addr & 0x3f;
	int i,block_no=0;
	for( i=0; i<16; i++){
		if(tag_in_dram==L2[cache_no][i].tag&&L2[cache_no][i].valid==1){
			block_no=i;
			break;
		}
	}
	L2[cache_no][i].dirty=1;
	int cnt=len-(offset+len-64);
	int j=0;
	if(offset+len<=64){
	    while(len>0){
		    L2[cache_no][block_no].offset[offset+len-1]=(data>>((len-1)*8))&0xff;
		    len--;
	    }
	}
	else{
		while(j<cnt){
		    L2[cache_no][block_no].offset[offset]=data&0xff;
			data=data>>8;
			j++;
			offset++;
	    }
		hwaddr_write(((addr+0x40)>>6)<<6, len-cnt, data);
	}
	
}


void write_allocate(hwaddr_t addr, size_t len, uint32_t data){
	uint16_t tag_in_dram = addr >> 18;
	uint16_t cache_no = ( addr >> 6 ) & 0xfff;
	uint8_t offset = addr & 0x3f;
    dram_write(addr,len,data);
	
	int i,idx=0;
	srand(time(0)+clock());
	i=rand()%16;
	uint32_t init_addr=(L2[cache_no][i].tag<<18)+(cache_no<<6);
	
	if(L2[cache_no][i].dirty==1&&L2[cache_no][i].valid==1){
		while(idx<64){
		    dram_write(init_addr,1,L2[cache_no][i].offset[idx]);
			idx++;
			init_addr++;
		}
	}
	L2[cache_no][i].tag=tag_in_dram;
	L2[cache_no][i].dirty=0;
	L2[cache_no][i].valid=1;
	addr=addr-offset;
	int j;
	for(j=0;j<64;j++){
		L2[cache_no][i].offset[j]=dram_read(addr+j,1);
	}	
}





















