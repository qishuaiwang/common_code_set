#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "data_repackage.h"

char *load_string = "memory -load %%readmemh ";
char *hieraych_string = "u_sigi_top.u_digital_top.u_ddr_blob.u_ddr_subsys_top_pwr_wrap%d.u_ddr_subsys_top_wrap.u_ddr_sys_top.u_DWC_ddr.ddrphy.u_dwc_ddrphy_top.u_lpddr5_16GB_rank%d_chan%s.memcore%d";
char *file_string = "-file DDRsys%d_rank%d_chan%c_memcore%d_%d\n";
struct MODULE_BIT_INFO ddr_module_32gb[DDR_MODULE_32GBIT_BIT_NUM] = {
    // {"CH" ,  -1,  -1,  1,  0},
    {"B0" ,  0,  0,  2,  0},
    {"B1" ,  1,  1,  3,  0},
    {"B2" ,  2,  2,  4,  0},
    {"B3" ,  3,  3,  5,  0},
    {"C0" ,  4,  4,  6,  0},
    {"C1" ,  5,  5,  7,  0},
    {"C2" ,  6,  6,  8,  0},
    {"C3" ,  7,  7,  9,  0},
    {"C4" ,  8,  8, 10,  0},
    {"C5" ,  9,  9, 11,  0},
    {"BA0", 10, 10, 12,  0},
    {"BA1", 11, 11, 13,  0},
    {"BG0", 12, 12, 14,  0},
    {"BG1", 13, 13, 15,  0},
    {"R0",  14, 14, 16,  0},
    {"R1",  15, 15, 17,  0},
    {"R2",  16, 16, 18,  0},
    {"R3",  17, 17, 19,  0},
    {"R4",  18, 18, 20,  0},
    {"R5",  19, 19, 21,  0},
    {"R6",  20, 20, 22,  0},
    {"R7",  21, 21, 23,  0},
    {"R8",  22, 22, 24,  0},
    {"R9",  23, 23, 25,  0},
    {"R10", 24, 24, 26,  0},
    {"R11", 25, 25, 27,  0},
    {"R12", 26, 26, 28,  0},
    {"R13", 27, 27, 29,  0},
    {"R14", 28, 28, 30,  0},
    {"R15", 29, 29, 31,  0},
    {"MemCore", 30, 30, 32,  0},
    {"Rank", 31, 31, 33,  0},
};

long addr_moduled_to_hif (union address_bits module_addr)
{
    union address_bits hif_addr;
    hif_addr.addr = 0;
    // module_addr.addr = 0;
    __uint32_t i = 0;
    __uint32_t bit_value = 0;

    for (i = 0; i < DDR_MODULE_32GBIT_BIT_NUM; i++) {
        bit_value = BIT_GET(module_addr.addr, ddr_module_32gb[i].module_position);
        hif_addr.addr |= (bit_value << ddr_module_32gb[i].hif_position);
    }
    return hif_addr.addr;
}

//__uint64_t get_file_position(long soc_address, long start_address)
//{
//    if (soc_address < start_address) {
//        offset++;
//    } else {
//        soc_address - start_address;
//    }
//}

/* file info create */
void free_data(void *useless_data)
{
    struct FILE_INFO *tmp = useless_data;
    free(tmp);
}

int equals(const void *x, const void *y) {
    struct FILE_INFO *a = x;
    struct FILE_INFO *b = y;
    return  a->sys_num == b->sys_num &&
            a->rank_num == b->rank_num &&
            a->mem_core_num == b->mem_core_num &&
            a->ch_name == b->ch_name &&
            a->start_offset == b->start_offset &&
            a->index == b->index &&
            a->incresing == b->incresing;
}

int create_mem_load_cmd (FILE *p_file, struct FILE_INFO *p_file_info)
{
    fprintf(p_file, load_string);
    fprintf(p_file, hieraych_string, p_file_info->sys_num, p_file_info->rank_num, p_file_info->ch_name, p_file_info->mem_core_num);
    fprintf(p_file, file_string, p_file_info->sys_num, p_file_info->rank_num, p_file_info->ch_name, p_file_info->mem_core_num, p_file_info->index);
}


