#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/cdefs.h>
#include "data_repackage.h"
#include "linked_list.h"

char *load_string = "memory -load %%readmemh %n";
char *hieraych_string = "u_sigi_top.u_digital_top.u_ddr_blob.u_ddr_subsys_top_pwr_wrap%d.u_ddr_subsys_top_wrap.u_ddr_sys_top.u_DWC_ddr.ddrphy.u_dwc_ddrphy_top.u_lpddr5_16GB_rank%d_chan%s.memcore%d";
char *file_string = " -file %s/DDRsys%d_rank%d_chan%s_memcore%d_%d -start %#010x\n";
char *file_name = "DDRsys%d_rank%d_chan%s_memcore%d_%d";

/* file info create */
void free_data(void *useless_data)
{
    struct FILE_INFO *tmp = useless_data;
    free(tmp);
}

int equals(const void *x, const void *y) {
    struct FILE_INFO *a = (struct FILE_INFO *)x;
    struct FILE_INFO *b = (struct FILE_INFO *)y;
    return  a->sys_num == b->sys_num &&
            a->rank_num == b->rank_num &&
            a->mem_core_num == b->mem_core_num &&
            a->ch_name == b->ch_name &&
            a->start_offset == b->start_offset &&
            a->index == b->index &&
            a->increasing == b->increasing;
}

int consistents(const void *x, const void *y)
{
    struct FILE_INFO *a = (struct FILE_INFO *)x;
    struct FILE_INFO *b = (struct FILE_INFO *)y;
    int ret = 0;
    if (a->sys_num == b->sys_num &&
            a->rank_num == b->rank_num &&
            a->mem_core_num == b->mem_core_num &&
            (!strcmp(a->ch_name, b->ch_name)))
    ret = (a->start_offset == (b->end_offset + 1)) || (b->start_offset == (a->end_offset + 1));
    return ret;
}
/* DDR address in SOC map */
__uint64_t ddr_sys_base_addr[2][DDR_SYS_NUM] = {
    {0x3000000000, 0x3400000000, 0x3800000000, 0x3c00000000, 0x4000000000, 0x4400000000}, // none interleave space
    {0x1000000000, 0x1000000000, 0x1800000000, 0x1800000000, 0x2000000000, 0x2000000000}, // interleave space
};
#define GET_ADDRMAP_CS_B0(x) (0x3F & (x))
#define GET_ADDRMAP_BANK_B2(x) (0x3F & ((x) >> 16))
#define GET_ADDRMAP_BANK_B1(x) (0x3F & ((x) >> 8))
#define GET_ADDRMAP_BANK_B0(x) (0x3F & (x))
#define GET_ADDRMAP_BG_B1(x)   (0x3F & ((x) >> 8))
#define GET_ADDRMAP_BG_B0(x)   (0x3F & (x))
#define GET_ADDRMAP_COL_B10(x) (0x1F & ((x) >> 24))
#define GET_ADDRMAP_COL_B9(x)  (0x1F & ((x) >> 16))
#define GET_ADDRMAP_COL_B8(x)  (0x1F & ((x) >> 8))
#define GET_ADDRMAP_COL_B7(x)  (0x1F & (x))
#define GET_ADDRMAP_COL_B6(x)  (0xF & ((x) >> 24))
#define GET_ADDRMAP_COL_B5(x)  (0xF & ((x) >> 16))
#define GET_ADDRMAP_COL_B4(x)  (0xF & ((x) >> 8))
#define GET_ADDRMAP_COL_B3(x)  (0xF & (x))
#define GET_ADDRMAP_ROW_B17(x)  (0x1F & ((x) >> 24))
#define GET_ADDRMAP_ROW_B16(x)  (0x1F & ((x) >> 16))
#define GET_ADDRMAP_ROW_B15(x)  (0x1F & ((x) >> 8))
#define GET_ADDRMAP_ROW_B14(x)  (0x1F & (x))
#define GET_ADDRMAP_ROW_B13(x)  (0x1F & ((x) >> 24))
#define GET_ADDRMAP_ROW_B12(x)  (0x1F & ((x) >> 16))
#define GET_ADDRMAP_ROW_B11(x)  (0x1F & ((x) >> 8))
#define GET_ADDRMAP_ROW_B10(x)  (0x1F & (x))
#define GET_ADDRMAP_ROW_B9(x)  (0x1F & ((x) >> 24))
#define GET_ADDRMAP_ROW_B8(x)  (0x1F & ((x) >> 16))
#define GET_ADDRMAP_ROW_B7(x)  (0x1F & ((x) >> 8))
#define GET_ADDRMAP_ROW_B6(x)  (0x1F & (x))
#define GET_ADDRMAP_ROW_B5(x)  (0x1F & ((x) >> 24))
#define GET_ADDRMAP_ROW_B4(x)  (0x1F & ((x) >> 16))
#define GET_ADDRMAP_ROW_B3(x)  (0x1F & ((x) >> 8))
#define GET_ADDRMAP_ROW_B2(x)  (0x1F & (x))
#define GET_ADDRMAP_ROW_B1(x)  (0x1F & ((x) >> 8))
#define GET_ADDRMAP_ROW_B0(x)  (0x1F & (x))

/*****************************************************/
/* Just for RBC module address configuration         */
/* Do not change the sequence, or result will wrong. */
/*****************************************************/
#ifndef DDR_MODULE_ADDR_RBC
struct MODULE_BIT_INFO ddr_module_32gb[DDR_MODULE_32GB_BIT_NUM] = {
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
    {"R0",  10, 10, 12,  0},
    {"R1",  11, 11, 13,  0},
    {"R2",  12, 12, 14,  0},
    {"R3",  13, 13, 15,  0},
    {"R4",  14, 14, 16,  0},
    {"R5",  15, 15, 17,  0},
    {"R6",  16, 16, 18,  0},
    {"R7",  17, 17, 19,  0},
    {"R8",  18, 18, 20,  0},
    {"R9",  19, 19, 21,  0},
    {"R10", 20, 20, 22,  0},
    {"R11", 21, 21, 23,  0},
    {"R12", 22, 22, 24,  0},
    {"R13", 23, 23, 25,  0},
    {"R14", 24, 24, 26,  0},
    {"R15", 25, 25, 27,  0},
    {"R16", 26, 26, 28,  0},
    {"BA0", 27, 27, 29,  0},
    {"BA1", 28, 28, 30,  0},
    {"BG0", 29, 29, 31,  0},
    {"MemCore", 30, 30, 32,  0},
    {"Rank", 31, 31, 33,  0},
    // {"BA0", 23, 23, 25,  0},
    // {"BA1", 24, 24, 26,  0},
    // {"BG0", 25, 25, 27,  0},
    // {"BG1", 26, 26, 28,  0},
    // {"MemCore", 27, 27, 29,  0},
    // {"Rank", 28, 28, 30,  0},
};
_Static_assert(sizeof(ddr_module_32gb)/sizeof(ddr_module_32gb[0]) <= DDR_MODULE_32GB_BIT_NUM,
                "Please make sure array size not over defined.");
enum data_repackage_index
{
    B0 = 0, B1, B2, B3, C0, C1, C2, C3, C4, C5, R0, R1, R2, R3, R4, R5,
    R6, R7, R8, R9, R10, R11, R12, R13, R14, R15, R16, BA0, BA1, BG0, MemCore,
    Rank
    // B0 = 0, B1, B2, B3, C0, C1, C2, C3, C4, C5, R0, R1, R2, R3, R4, R5,
    // R6, R7, R8, R9, R10, R11, R12, BA0, BA1, BG0, BG1,
    // MemCore, Rank
};
// long hif_addr_update (union address_bits module_addr, __uint32_t *map)
int hif_addr_update (__uint32_t *addrmap)
{
    __uint8_t i = 0, j = 0, k = 0;
    ddr_module_32gb[Rank].hif_position = GET_ADDRMAP_CS_B0(addrmap[0]) + 6;
    ddr_module_32gb[R16].hif_position = (0x3f == GET_ADDRMAP_ROW_B16(addrmap[5])) ? 0x3f : GET_ADDRMAP_ROW_B16(addrmap[5]) + 22;
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
    ddr_module_32gb[MemCore].hif_position = (0x3f == GET_ADDRMAP_BG_B1(addrmap[2])) ? 0x3f : GET_ADDRMAP_BG_B1(addrmap[2]) + 4;
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
    for (i = 0; i < DDR_MODULE_32GB_BIT_NUM; i++) {
        printf("%s : module: %#08x hif: %#08x\n", ddr_module_32gb[i].bit_name, ddr_module_32gb[i].module_position, ddr_module_32gb[i].hif_position);
    }
    return 0;
}

#else
struct MODULE_BIT_INFO ddr_module_32gb[DDR_MODULE_32GB_BIT_NUM] = {
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
_Static_assert(sizeof(ddr_module_32gb)/sizeof(ddr_module_32gb[0]) <= DDR_MODULE_32GB_BIT_NUM,
                "Please make sure array size not over defined.");
enum data_repackage_index
{
    B0 = 0, B1, B2, B3, C0, C1, C2, C3, C4, C5, BA0, BA1, BG0, BG1, R0,  
    R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15,
    MemCore, Rank
};
// long hif_addr_update (union address_bits module_addr, __uint32_t *map)
int hif_addr_update (__uint32_t *addrmap)
{
    __uint8_t i = 0, j = 0, k = 0;
    ddr_module_32gb[Rank].hif_position = GET_ADDRMAP_CS_B0(addrmap[0]) + 6;
    ddr_module_32gb[MemCore].hif_position = (0x3f == GET_ADDRMAP_ROW_B16(addrmap[5])) ? 0x3f : GET_ADDRMAP_ROW_B16(addrmap[5]) + 22;
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
    for (i = 0; i < DDR_MODULE_32GB_BIT_NUM; i++) {
        printf("%s : module: %#08x hif: %#08x\n", ddr_module_32gb[i].bit_name, ddr_module_32gb[i].module_position, ddr_module_32gb[i].hif_position);
    }
    return 0;
}

#endif
__uint64_t addr_module_to_hif (__uint64_t module_addr)
{
    union address_bits hif_addr;
    hif_addr.addr = 0;
    // module_addr.addr = 0;
    __uint32_t i = 0;
    __uint64_t bit_value = 0;

    for (i = 0; i < DDR_MODULE_32GB_BIT_NUM; i++) {
        bit_value = BIT_GET(module_addr, ddr_module_32gb[i].module_position);
        hif_addr.addr |= (bit_value << ddr_module_32gb[i].hif_position);
    }
    return hif_addr.addr;
}

/*************************************************/
/* soc   hif     CHA     CHB   mem_line_num      */
/* 0x0   0x0     0x0     0x2   0x0               */
/* 0x4   0x1     0x4     0x6   0x1               */
/* 0x8   0x2     0x8     0xa   0x2               */
/*************************************************/
__uint64_t addr_hif_to_soc(struct FILE_INFO *p_file_info, __uint64_t hif_addr)
{
    __uint64_t soc_addr = 0;
    __uint64_t addr = 0;
    char *ch = p_file_info->ch_name;
    if (!strcmp(ch, "A")) {
        addr = (hif_addr << 2);
    } else if (!strcmp(ch, "B")) {
        addr = (hif_addr << 2) + 2;
    } else {
        addr = (hif_addr << 2);
        printf("Error!!! Wrong chn detect.\n");
    }
    if (p_file_info->interleave_size) {
        soc_addr = ddr_sys_base_addr[1][p_file_info->sys_num] + (((addr / p_file_info->interleave_size) << 1) + p_file_info->sys_num % 2)
        * p_file_info->interleave_size + (addr % p_file_info->interleave_size);
        // soc_addr = ddr_sys_base_addr[1][p_file_info->sys_num] + ((addr & (~(p_file_info->interleave_size - 1))) << 1) + (p_file_info->sys_num & 0x1)
        // * p_file_info->interleave_size + (addr & (p_file_info->interleave_size - 1));
    } else {
        soc_addr = ddr_sys_base_addr[0][p_file_info->sys_num] + addr;
    }
    return soc_addr;
}

__uint64_t module_addr_get (struct FILE_INFO *p_file_info, __uint64_t index)
{
    __uint64_t module_addr = 0;
    module_addr = (p_file_info->mem_core_num << ddr_module_32gb[MemCore].module_position)
    | (p_file_info->rank_num << ddr_module_32gb[Rank].module_position) | index;
    return module_addr;
}

__uint64_t addr_hif_to_module (__uint64_t hif_addr)
{
    union address_bits module_addr;
    module_addr.addr = 0;
    // module_addr.addr = 0;
    __uint32_t i = 0;
    __uint64_t bit_value = 0;

    for (i = 0; i < DDR_MODULE_32GB_BIT_NUM; i++) {
        bit_value = BIT_GET(hif_addr, ddr_module_32gb[i].hif_position);
        module_addr.addr |= (bit_value << ddr_module_32gb[i].module_position);
    }
    return module_addr.addr;
}

/*************************************************/
/* soc   hif     CHA     CHB   mem_line_num      */
/* 0x0   0x0     0x0     0x2   0x0               */
/* 0x4   0x1     0x4     0x6   0x1               */
/* 0x8   0x2     0x8     0xa   0x2               */
/*************************************************/
__uint64_t addr_soc_to_modual(struct FILE_INFO *p_file_info, __uint64_t soc_addr)
{
    __uint64_t hif_addr = 0;
    __uint64_t module_addr = 0;
    __uint8_t ddr_sys = 0;
    /* DDR SYS */
    if (p_file_info->interleave_size) {
        if (soc_addr < ddr_sys_base_addr[1][0] || soc_addr > ddr_sys_base_addr[1][DDR_SYS_NUM - 1]) {
            perror("soc_address is not in interleave space!\n");
        } else {
            ddr_sys = DDR_SYS_NUM - (soc_addr & p_file_info->interleave_size) ? 1 : 2;
            for (int i = 1; i < DDR_SYS_NUM; i += 2) {
                if (soc_addr < ddr_sys_base_addr[1][i]) {
                    ddr_sys = i - (soc_addr & p_file_info->interleave_size) ? 0 : 1;
                    break;
                }
            }
        }
    } else {
        if (soc_addr < ddr_sys_base_addr[0][0] || soc_addr > ddr_sys_base_addr[0][DDR_SYS_NUM - 1]) {
            perror("soc_address is not correct!\n");
        } else {
            ddr_sys = DDR_SYS_NUM - 1;
            for (int i = 1; i < DDR_SYS_NUM; i++) {
                if (soc_addr < ddr_sys_base_addr[0][i]) {
                    ddr_sys = i - 1;
                    break;
                }
            }
        }
    }
    p_file_info->sys_num = ddr_sys;
    /* soc_addr & 0x3 */
    /* 0/1 channel A */
    /* 2/3 channel B */
    char *ch = p_file_info->ch_name;
    strcpy(ch, (soc_addr & 0x2) ? "B" : "A");
    hif_addr = soc_addr >> 2;
    module_addr = addr_hif_to_module(hif_addr);
    /* rank */
    p_file_info->rank_num = (module_addr >> Rank) & 0x1;
    /* mem_core_num */
    p_file_info->mem_core_num = (module_addr >> MemCore) & 0x1;
    /* file_index */
    p_file_info->index += 1;
    /* start_offset end_offset */
    p_file_info->start_offset = (module_addr) & (~(0x3 << MemCore));
    p_file_info->end_offset = p_file_info->start_offset; // size 1. end_offset == start_offset.
    return module_addr;
}

int filecpy(const char* dest, const char* src)
{
	FILE* fin = fopen(src, "r");
	FILE* fout = fopen(dest, "w");
    char c = 0;

	if (fin && fout)
	{
		// while (!feof(fin))//读到最后一个字符，feof（fin）仍未false
		// {
		// 	fputc(fgetc(fin), fout);
		// }
        c = fgetc(fin);
		while (c != EOF)//读到最后一个字符，feof（fin）仍未false
		{
			fputc(c, fout);
            c = fgetc(fin);
		}
		fclose(fin);
		fclose(fout);
		return 0;
	}
	return -1;
}

int filecat(const char* dest, const char* src)
{
	FILE* fin = fopen(src, "r");
	FILE* fout = fopen(dest, "a");

    char c = 0;
	if (fin && fout)
	{
		// while (!feof(fin))//读到最后一个字符，feof（fin）仍未false
        c = fgetc(fin);
		while (c != EOF)//读到最后一个字符，feof（fin）仍未false
		{
			fputc(c, fout);
            c = fgetc(fin);
		}
		fclose(fin);
		fclose(fout);
		return 0;
	}
	return -1;
}

__int8_t item_merge(linkedlist *p_f_list, struct FILE_INFO *data)
{
    void *v_item = NULL;
    struct list_node *item = NULL;
    __int8_t ret = -1;
    if (p_f_list->head != NULL) {

        /* if any item need merge? end_addr = start_addr - 1. */
        if (1 == list_consistent_item_get(&v_item, p_f_list, data, consistents)) {
            item = (struct list_node *)v_item;
            struct FILE_INFO *a = (struct FILE_INFO *)item->data;
            struct FILE_INFO *b = (struct FILE_INFO *)data;
            if (a->start_offset < b->start_offset) {
                /* file merge */
                filecat(a->file_name, b->file_name);
                remove(b->file_name);
                /* item update */
                a->end_offset = b->end_offset;
                /* data free */
                free_data(data);
            } else {
                /* file merge */
                filecat(b->file_name, a->file_name);
                filecpy(a->file_name, b->file_name);
                remove(b->file_name);
                /* item update */
                a->start_offset = b->start_offset;
                /* data free */
                free_data(data);
            }
            ret = 0;
        }
    }
    return ret;
}
// __uint64_t module_addr_get (struct FILE_INFO *p_file_info, __uint64_t index)
// {
//     __uint64_t module_addr = 0;
//     module_addr = (p_file_info->mem_core_num << ddr_module_32gb[MemCore].module_position)
//     | (p_file_info->rank_num << ddr_module_32gb[Rank].module_position) | ((index & (0x3c000000) << 1) | (index & (0x3ffffff)));
//     return module_addr;
// }

//__uint64_t get_file_position(long soc_address, long start_address)
//{
//    if (soc_address < start_address) {
//        offset++;
//    } else {
//        soc_address - start_address;
//    }
//}

int create_mem_load_cmd (FILE *p_file, struct FILE_INFO *p_file_info)
{
    __uint32_t tmp_data = 0;
	char cur_dir[1024] = {0};
	getcwd(cur_dir, 1024);
    fprintf(p_file, load_string, &tmp_data);
    fprintf(p_file, hieraych_string, p_file_info->sys_num, p_file_info->rank_num, p_file_info->ch_name, p_file_info->mem_core_num);
    fprintf(p_file, file_string, cur_dir, p_file_info->sys_num, p_file_info->rank_num, p_file_info->ch_name, p_file_info->mem_core_num, p_file_info->index, p_file_info->start_offset);
}

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)
__uint64_t mem_save_data_num_get(struct FILE_INFO *p_file_info)
{
    __uint64_t start_addr, end_addr, mem_index_min, mem_index_max;
    __uint64_t soc_addr_min, soc_addr_max, module_addr;
    __uint64_t data_saved_num;
    start_addr = p_file_info->img_start_address;
    end_addr = p_file_info->img_end_address;
    data_saved_num = MIN(DDR_MEMCORE_SIZE, end_addr - start_addr);
    return data_saved_num;
    // mem_index_min = 0;
    // module_addr = module_addr_get(p_file_info, mem_index_min);
    // soc_addr_min = addr_hif_to_soc(p_file_info, addr_module_to_hif(module_addr));
    // mem_index_max = DDR_MEMCORE_INDEX_MAX;
    // module_addr = module_addr_get(p_file_info, mem_index_max);
    // soc_addr_max = addr_hif_to_soc(p_file_info, addr_module_to_hif(module_addr));
    // data_saved_num = MIN(end_addr, soc_addr_max) - MAX(start_addr, soc_addr_min) + 1;
    // return data_saved_num;
}

#define IS_CONTAIN(x, s, e) ((s > x || e <= x) ? false : true)
int memcore_file_create (linkedlist *file_list, struct FILE_INFO *file_tmp, __uint64_t index, __uint64_t data_save_num)
{
    int ret_no = 0;
    __uint64_t start_addr, end_addr, mem_index;
    __uint64_t soc_addr, module_addr, hif_addr;
    struct FILE_INFO *p_file_info = (file_list->head) ? (struct FILE_INFO *)(file_list->head->data) : file_tmp;
    
    // printf("index: %lu\n", index);
    start_addr = p_file_info->img_start_address;
    end_addr = p_file_info->img_end_address;
    mem_index = index;
    //get module_addr
    module_addr = module_addr_get(p_file_info, mem_index);
    /* memcore size */
    if (module_addr > DDR_MEMCORE_SIZE){
        ret_no = -1;
        goto end;
    }
    //get soc_addr
    soc_addr = addr_hif_to_soc(p_file_info, addr_module_to_hif(module_addr));
    // if (soc_addr == 0x3008000040)
    // {
        // while (1) printf("now\n");
    // }
/* soc_addr < start_addr, soc_addr > start_addr + file_size, */
/* no file need operate, offset plus, file_creating = 0 */
    if (!(IS_CONTAIN(soc_addr, start_addr, end_addr))) {
        if (__glibc_unlikely(p_file_info->increasing)) {
f_save:     p_file_info->increasing = false;
            fclose(p_file_info->op_file);
            FILE *p_script_file = fopen(BACKDOOR_SCRIPT_FILE, "a+");
            create_mem_load_cmd(p_script_file, p_file_info);
            fclose(p_script_file);
            p_file_info->index++;
            goto end;
        }
    } else {
/* soc_addr >= start_addr, soc_addr <= start_addr + file_size,*/
/* continue add data to cur file if file_creating,*/
/* or new file if file_creating = 0, file_creating = 1*/
        if (!p_file_info->increasing) {
            // create one file_info item, add to list
            struct FILE_INFO *node = (struct FILE_INFO *) malloc(sizeof(struct FILE_INFO));
            memcpy(node, p_file_info, sizeof(*node));
            node->start_offset = mem_index;
            node->increasing = true;
            list_insert(file_list, node);
            p_file_info = node;
            // create new file
            sprintf(p_file_info->file_name, file_name, p_file_info->sys_num,
            p_file_info->rank_num, p_file_info->ch_name, p_file_info->mem_core_num,
            p_file_info->index);
            p_file_info->op_file = fopen(p_file_info->file_name, "w");
        }

        // get data from image file
        union data_read data;
        __uint64_t file_offset = 0;
        __uint32_t result = 0;
        file_offset = soc_addr - p_file_info->img_start_address;
        // FILE *p_src_file = fopen("src", "rb");
        FILE *p_src_file = p_file_info->img_file;
        fseek(p_src_file, file_offset, SEEK_SET);
        result = fread(data.data_char, 1, 2, p_src_file);
        if (result != 2) {fputs ("Reading error",stderr); exit (3);}
        // write data to vhx file
        // FILE *p_file = fopen(p_file_info->file_name, "a+");
        FILE *p_file = p_file_info->op_file;
        // char tmp[30] = {0};
        // int n = sprintf(tmp, "%x\n", data);
        fprintf(p_file, "%04x\n", data.data);
        p_file_info->byte_writed_num += 2;
        // fclose(p_file);
        if(p_file_info->byte_writed_num == (data_save_num)/2) {
            ret_no = -1;
            goto f_save;
        }
        if(p_file_info->byte_writed_num == (data_save_num)) {
            ret_no = -1;
            goto f_save;
        }
        if(mem_index == (DDR_MEMCORE_INDEX_MAX - 1)) {
            ret_no = -1;
            goto f_save;
        }
    }
    // mem_index++
    // mem_index++;
    // memcore_file_create (file_list, p_file_info, mem_index);
    return 0;
end:
    return ret_no;
}

int memcore_file_create_direct(linkedlist *file_list, struct FILE_INFO *file_tmp,  __uint64_t sys_addr, __uint16_t data)
{
    __uint64_t module_addr = 0;
    void *list_item =  NULL;
    struct FILE_INFO *node =  NULL;
    struct FILE_INFO *p_file_info = (file_list->head) ? (struct FILE_INFO *)(file_list->head->data) : file_tmp;
    node = (struct FILE_INFO *) malloc(sizeof(struct FILE_INFO));
    memcpy(node, p_file_info, sizeof(*node));
    module_addr = addr_soc_to_modual(node, sys_addr);
    sprintf(node->file_name, file_name, node->sys_num,
    node->rank_num, node->ch_name, node->mem_core_num,
    node->index);
    node->op_file = fopen(node->file_name, "w");
    fprintf(node->op_file, "%04x\n", data);
    fclose(node->op_file);
    if(item_merge(file_list, node)) {
        list_insert(file_list, node);
    }else {
        node = NULL;
    }
}

int create_mem_load_script(linkedlist *p_list, char *file_name)
{
    struct list_node *list_item = NULL;
    struct list_node *list_tmp = p_list->head;
    int ret = 0;
    FILE *p_file = fopen(file_name, "w");
    while(list_tmp) {
        create_mem_load_cmd (p_file, (struct FILE_INFO *)list_tmp->data);
        list_tmp = list_tmp->next;
    }
    return ret;
}

