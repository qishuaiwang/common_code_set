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
enum data_repackage_index
{
    B0 = 0, B1, B2, B3, C0, C1, C2, C3, C4, C5, BA0, BA1, BG0, BG1, R0,  
    R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15,
    MemCore, Rank
};

#define GET_ADDRMAP_CS_B0(x) (0x3F & (x))
#define GET_ADDRMAP_BANK_B2(x) (0x3F & ((x) >> 16))
#define GET_ADDRMAP_BANK_B1(x) (0x3F & ((x) >> 8))
#define GET_ADDRMAP_BANK_B0(x) (0x3F & (x))
#define GET_ADDRMAP_BG_B1(x)   (0x3F & ((x) >> 8))
#define GET_ADDRMAP_BG_B0(x)   (0x3F & (x))
#define GET_ADDRMAP_COL_B10(x) (0x3F & ((x) >> 24))
#define GET_ADDRMAP_COL_B9(x)  (0x3F & ((x) >> 16))
#define GET_ADDRMAP_COL_B8(x)  (0x3F & ((x) >> 8))
#define GET_ADDRMAP_COL_B7(x)  (0x3F & (x))
#define GET_ADDRMAP_COL_B6(x)  (0x3F & ((x) >> 24))
#define GET_ADDRMAP_COL_B5(x)  (0x3F & ((x) >> 16))
#define GET_ADDRMAP_COL_B4(x)  (0x3F & ((x) >> 8))
#define GET_ADDRMAP_COL_B3(x)  (0x3F & (x))
#define GET_ADDRMAP_ROW_B17(x)  (0x3F & ((x) >> 24))
#define GET_ADDRMAP_ROW_B16(x)  (0x3F & ((x) >> 16))
#define GET_ADDRMAP_ROW_B15(x)  (0x3F & ((x) >> 8))
#define GET_ADDRMAP_ROW_B14(x)  (0x3F & (x))
#define GET_ADDRMAP_ROW_B13(x)  (0x3F & ((x) >> 24))
#define GET_ADDRMAP_ROW_B12(x)  (0x3F & ((x) >> 16))
#define GET_ADDRMAP_ROW_B11(x)  (0x3F & ((x) >> 8))
#define GET_ADDRMAP_ROW_B10(x)  (0x3F & (x))
#define GET_ADDRMAP_ROW_B9(x)  (0x3F & ((x) >> 24))
#define GET_ADDRMAP_ROW_B8(x)  (0x3F & ((x) >> 16))
#define GET_ADDRMAP_ROW_B7(x)  (0x3F & ((x) >> 8))
#define GET_ADDRMAP_ROW_B6(x)  (0x3F & (x))
#define GET_ADDRMAP_ROW_B5(x)  (0x3F & ((x) >> 24))
#define GET_ADDRMAP_ROW_B4(x)  (0x3F & ((x) >> 16))
#define GET_ADDRMAP_ROW_B3(x)  (0x3F & ((x) >> 8))
#define GET_ADDRMAP_ROW_B2(x)  (0x3F & (x))
#define GET_ADDRMAP_ROW_B1(x)  (0x3F & ((x) >> 8))
#define GET_ADDRMAP_ROW_B0(x)  (0x3F & (x))

// long hif_addr_update (union address_bits module_addr, __uint32_t *map)
long hif_addr_update (__uint32_t *addrmap)
{
    __uint8_t i = 0, j = 0, k = 0;
    ddr_module_32gb[Rank].hif_position = GET_ADDRMAP_CS_B0(addrmap[0]) + 6;
    ddr_module_32gb[R15].hif_position = (0x3f == GET_ADDRMAP_ROW_B15(addrmap[5])) ? 0x3f : GET_ADDRMAP_ROW_B15(addrmap[5]) + 21;
    ddr_module_32gb[R14].hif_position = (0x3f == GET_ADDRMAP_ROW_B14(addrmap[5])) ? 0x3f : GET_ADDRMAP_ROW_B14(addrmap[5]) + 20;
    ddr_module_32gb[R13].hif_position = (0x3f == GET_ADDRMAP_ROW_B13(addrmap[6])) ? 0x3f : GET_ADDRMAP_ROW_B13(addrmap[6]) + 19;
    ddr_module_32gb[R12].hif_position = (0x3f == GET_ADDRMAP_ROW_B12(addrmap[6])) ? 0x3f : GET_ADDRMAP_ROW_B12(addrmap[6]) + 18;
    ddr_module_32gb[R11].hif_position = (0x3f == GET_ADDRMAP_ROW_B11(addrmap[6])) ? 0x3f : GET_ADDRMAP_ROW_B11(addrmap[6]) + 17;
    ddr_module_32gb[R10].hif_position = (0x3f == GET_ADDRMAP_ROW_B10(addrmap[6])) ? 0x3f : GET_ADDRMAP_ROW_B10(addrmap[6]) + 16;
    ddr_module_32gb[R9].hif_position = (0x3f == GET_ADDRMAP_ROW_B9(addrmap[7])) ? 0x3f : GET_ADDRMAP_ROW_B9(addrmap[7]) + 15;
    ddr_module_32gb[R8].hif_position = (0x3f == GET_ADDRMAP_ROW_B8(addrmap[7])) ? 0x3f : GET_ADDRMAP_ROW_B8(addrmap[7]) + 14;
    ddr_module_32gb[R7].hif_position = (0x3f == GET_ADDRMAP_ROW_B7(addrmap[7])) ? 0x3f : GET_ADDRMAP_ROW_B7(addrmap[7]) + 13;
    ddr_module_32gb[R6].hif_position = (0x3f == GET_ADDRMAP_ROW_B6(addrmap[7])) ? 0x3f : GET_ADDRMAP_ROW_B6(addrmap[7]) + 12;
    ddr_module_32gb[R5].hif_position = (0x3f == GET_ADDRMAP_ROW_B5(addrmap[8])) ? 0x3f : GET_ADDRMAP_ROW_B5(addrmap[8]) + 11;
    ddr_module_32gb[R4].hif_position = (0x3f == GET_ADDRMAP_ROW_B4(addrmap[8])) ? 0x3f : GET_ADDRMAP_ROW_B4(addrmap[8]) + 10;
    ddr_module_32gb[R3].hif_position = (0x3f == GET_ADDRMAP_ROW_B3(addrmap[8])) ? 0x3f : GET_ADDRMAP_ROW_B3(addrmap[8]) + 9;
    ddr_module_32gb[R2].hif_position = (0x3f == GET_ADDRMAP_ROW_B2(addrmap[8])) ? 0x3f : GET_ADDRMAP_ROW_B2(addrmap[8]) + 8;
    ddr_module_32gb[R1].hif_position = (0x3f == GET_ADDRMAP_ROW_B1(addrmap[9])) ? 0x3f : GET_ADDRMAP_ROW_B1(addrmap[9]) + 7;
    ddr_module_32gb[R0].hif_position = (0x3f == GET_ADDRMAP_ROW_B0(addrmap[9])) ? 0x3f : GET_ADDRMAP_ROW_B0(addrmap[9]) + 6;
    ddr_module_32gb[BG1].hif_position = (0x3f == GET_ADDRMAP_BG_B1(addrmap[2])) ? 0x3f : GET_ADDRMAP_BG_B1(addrmap[2]) + 4;
    ddr_module_32gb[BG0].hif_position = (0x3f == GET_ADDRMAP_BG_B0(addrmap[2])) ? 0x3f : GET_ADDRMAP_BG_B0(addrmap[2]) + 3;
    ddr_module_32gb[BA1].hif_position = (0x3f == GET_ADDRMAP_BANK_B1(addrmap[1])) ? 0x3f : GET_ADDRMAP_BANK_B1(addrmap[1]) + 4;
    ddr_module_32gb[BA0].hif_position = (0x3f == GET_ADDRMAP_BANK_B0(addrmap[1])) ? 0x3f : GET_ADDRMAP_BANK_B0(addrmap[1]) + 3;
    ddr_module_32gb[C5].hif_position = (0x3f == GET_ADDRMAP_COL_B9(addrmap[3])) ? 0x3f : GET_ADDRMAP_COL_B9(addrmap[3]) + 9;
    ddr_module_32gb[C4].hif_position = (0x3f == GET_ADDRMAP_COL_B8(addrmap[3])) ? 0x3f : GET_ADDRMAP_COL_B8(addrmap[3]) + 8;
    ddr_module_32gb[C3].hif_position = (0x3f == GET_ADDRMAP_COL_B7(addrmap[3])) ? 0x3f : GET_ADDRMAP_COL_B7(addrmap[3]) + 7;
    ddr_module_32gb[C2].hif_position = (0x3f == GET_ADDRMAP_COL_B6(addrmap[4])) ? 0x3f : GET_ADDRMAP_COL_B6(addrmap[4]) + 6;
    ddr_module_32gb[C1].hif_position = (0x3f == GET_ADDRMAP_COL_B5(addrmap[4])) ? 0x3f : GET_ADDRMAP_COL_B5(addrmap[4]) + 5;
    ddr_module_32gb[C0].hif_position = (0x3f == GET_ADDRMAP_COL_B4(addrmap[4])) ? 0x3f : GET_ADDRMAP_COL_B4(addrmap[4]) + 4;
    ddr_module_32gb[B3].hif_position = (0x3f == GET_ADDRMAP_COL_B3(addrmap[4])) ? 0x3f : GET_ADDRMAP_COL_B3(addrmap[4]) + 3;
    for (i = 0; i < DDR_MODULE_32GBIT_BIT_NUM; i++) {
        printf("%s : %#x\n", ddr_module_32gb[i].bit_name, ddr_module_32gb[i].hif_position);
    }
    return 0;
}
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


