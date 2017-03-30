#include<common.h>

typedef struct{
    uint8_t valid;
	uint32_t tag;//len=20
	uint32_t page_data;
	//uint8_t asid;
}TLB;
TLB tlb[64];
