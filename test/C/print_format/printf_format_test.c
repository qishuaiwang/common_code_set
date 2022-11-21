#include <stdio.h>

#define MAX_SIZE 0x4000
void main()
{
    __uint32_t cnt = 0;
    size_t ddr_addr = 0;
    __uint32_t data = 0;
    size_t ddr_base_addr = 0x3000000000;
	   /* */
	#if 0
   	for (cnt = 0; cnt < MAX_SIZE / sizeof(long); cnt <<= 1) {
    	ddr_addr = ddr_base_addr + cnt;	/* pointer arith! */
    	data = ddr_addr >> 0x16;
    	printf("%#018lx data %#010x \n", ddr_addr, data);
	}
	#endif
    	ddr_addr = ddr_base_addr + cnt;	/* pointer arith! */
    	data = ddr_addr >> 0x16;
    	printf("%#018lx data %#010x, start%nend \n", ddr_addr, data, &ddr_base_addr);
    	printf("%#28lx data %#20x, start%nend \n", ddr_addr, data, &ddr_base_addr);
    	printf("%-#28lx data %-#20x, start%nend \n", ddr_addr, data, &ddr_base_addr);
    	printf("%-#28lx data %-#20x, start%pend \n", ddr_addr, data, &ddr_base_addr);
}