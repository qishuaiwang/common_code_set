#ifndef DATA_REPACKAGE_H
#define DATA_REPACKAGE_H

#include <stdio.h>

#define DDR_MODULE_32GBIT_BIT_NUM 31
#define BIT_GET(x,y) (((x) >> (y)) & 0x1)

struct MODULE_BIT_INFO {
    char *bit_name;
    char module_position;
    char hif_position;
    char soc_position;
    char value;
};

union address_bits {
	struct {
		int bit0 : 1;
		int bit1 : 1;
		int bit2 : 1;
		int bit3 : 1;
		int bit4 : 1;
		int bit5 : 1;
		int bit6 : 1;
		int bit7 : 1;
		int bit8 : 1;
		int bit9 : 1;
		int bit10 : 1;
		int bit11 : 1;
        int bit12 : 1;
        int bit13 : 1;
        int bit14 : 1;
        int bit15 : 1;
        int bit16 : 1;
        int bit17 : 1;
        int bit18 : 1;
        int bit19 : 1;
        int bit20 : 1;
        int bit21 : 1;
        int bit22 : 1;
        int bit23 : 1;
        int bit24 : 1;
        int bit25 : 1;
        int bit26 : 1;
        int bit27 : 1;
        int bit28 : 1;
        int bit29 : 1;
        int bit30 : 1;
        int bit31 : 1;
        int bit32 : 1;
        int bit33 : 1;
        int bit34 : 1;
        int bit35 : 1;
        int bit36 : 1;
        int bit37 : 1;
        int bit38 : 1;
        int bit39 : 1;
	} bits;
	long addr;
};

struct FILE_INFO {
    __uint8_t sys_num;
    __uint8_t rank_num;
    __uint8_t mem_core_num;
    char *ch_name;
    __uint64_t start_offset;
    __uint32_t index;
    bool incresing;
    char *file_name;
};




long addr_moduled_to_hif (union address_bits module_addr);

#endif