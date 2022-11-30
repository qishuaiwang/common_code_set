#ifndef DATA_REPACKAGE_H
#define DATA_REPACKAGE_H

#include <stdio.h>
#include "linked_list.h"

#define DDR_SYS_NUM 6
#define DDR_RANK_NUM 2

// #define DDR_MODULE_ADDR_RBC
#define DDR_MEMCORE_NUM 2
#define DDR_MEMCORE_SIZE 0x100000000 // 0x4_0000_0000 / 4 = 0x1_0000_0000 =2^32
// #define DDR_MEMCORE_INDEX_MAX 0x8000
#define DDR_MEMCORE_INDEX_MAX 0x40000000 // DDR_MEMCORE_SIZE = DDR_MEMCORE_INDEX_MAX * 2(chn) * 2(bytes)
#define DDR_MODULE_32GBIT_BIT_NUM 31

#define BIT_GET(x,y) (((x) >> (y)) & 0x1)

#define BACKDOOR_SCRIPT_FILE "ddr_load_memory.qel"
struct MODULE_BIT_INFO {
    char *bit_name;
    char module_position;
    char hif_position;
    char soc_position;
    char value;
};

union address_bits {
	struct {
        __uint64_t bit0 : 1;
        __uint64_t bit1 : 1;
        __uint64_t bit2 : 1;
        __uint64_t bit3 : 1;
        __uint64_t bit4 : 1;
        __uint64_t bit5 : 1;
        __uint64_t bit6 : 1;
        __uint64_t bit7 : 1;
        __uint64_t bit8 : 1;
        __uint64_t bit9 : 1;
        __uint64_t bit10 : 1;
        __uint64_t bit11 : 1;
        __uint64_t bit12 : 1;
        __uint64_t bit13 : 1;
        __uint64_t bit14 : 1;
        __uint64_t bit15 : 1;
        __uint64_t bit16 : 1;
        __uint64_t bit17 : 1;
        __uint64_t bit18 : 1;
        __uint64_t bit19 : 1;
        __uint64_t bit20 : 1;
        __uint64_t bit21 : 1;
        __uint64_t bit22 : 1;
        __uint64_t bit23 : 1;
        __uint64_t bit24 : 1;
        __uint64_t bit25 : 1;
        __uint64_t bit26 : 1;
        __uint64_t bit27 : 1;
        __uint64_t bit28 : 1;
        __uint64_t bit29 : 1;
        __uint64_t bit30 : 1;
        __uint64_t bit31 : 1;
        __uint64_t bit32 : 1;
        __uint64_t bit33 : 1;
        __uint64_t bit34 : 1;
        __uint64_t bit35 : 1;
        __uint64_t bit36 : 1;
        __uint64_t bit37 : 1;
        __uint64_t bit38 : 1;
        __uint64_t bit39 : 1;
	} bits;
	__uint64_t addr;
};

struct FILE_INFO {
    __uint8_t sys_num;
    __uint8_t rank_num;
    __uint8_t mem_core_num;
    char ch_name[2];
    __uint64_t start_offset;
    __uint32_t interleave_size;
    __uint32_t index;
    bool increasing;
    bool iecc;
    char file_name[32];
    FILE *op_file;
    __uint64_t byte_writed_num;
    FILE *img_file;
    __uint64_t img_start_address;
    __uint64_t img_end_address;
};


union data_read
{
    __uint16_t data;
    char data_char[2];
};


int hif_addr_update (__uint32_t *addrmap);
int memcore_file_create (linkedlist *file_list, struct FILE_INFO *file_tmp, __uint64_t index, __uint64_t data_save_num);
__uint64_t mem_save_data_num_get(struct FILE_INFO *p_file_info);

#endif