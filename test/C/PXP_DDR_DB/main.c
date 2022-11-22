#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args_analysis.h"
#include "linked_list.h"
#include "data_repackage.h"
#include "cJSON.h"

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

/* Parse a addressmp to array. */
void parse_objects(char *filename, __uint32_t *addrmap)
{
	FILE *f;long len;char *data, *p_end;
	char *out;cJSON *json, *address_map_cfg;
	__uint32_t lpddr5_addrmap_2cs_16GB[] = {
        0x19, 0x3f0903, 0x101, 0x1f020005, 0x4020500, 0x1f080808, 0x8080808, 0x8080808, 0x8080808, 0x808
    };
    __uint32_t lpddr4_addrmap_2cs_8GB[] = {
        0x18, 0x70707, 0x3f3f, 0x1f000000, 0x0, 0x1f070707, 0x7070707, 0x7070707, 0x7070707, 0x707
    };
	
	f=fopen(filename,"rb");fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
	data=(char*)malloc(len+1);fread(data,1,len,f);fclose(f);
	
	json=cJSON_Parse(data);
	if (!json) {
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());
    } else {
		// out=cJSON_Print(json); cJSON_Delete(json); printf("%s\n",out); free(out);
        address_map_cfg = cJSON_GetObjectItem(json, "address_map_cfg");
        cJSON *addrmap1 = cJSON_GetObjectItem(address_map_cfg, "ADDRMAP1");
        cJSON *addrmap3 = cJSON_GetObjectItem(address_map_cfg, "ADDRMAP3");
        cJSON *addrmap4 = cJSON_GetObjectItem(address_map_cfg, "ADDRMAP4");
        cJSON *addrmap5 = cJSON_GetObjectItem(address_map_cfg, "ADDRMAP5");
        cJSON *addrmap6 = cJSON_GetObjectItem(address_map_cfg, "ADDRMAP6");
        cJSON *addrmap7 = cJSON_GetObjectItem(address_map_cfg, "ADDRMAP7");
        cJSON *addrmap8 = cJSON_GetObjectItem(address_map_cfg, "ADDRMAP8");
        cJSON *addrmap9 = cJSON_GetObjectItem(address_map_cfg, "ADDRMAP9");
        cJSON *addrmap10 = cJSON_GetObjectItem(address_map_cfg, "ADDRMAP10");
        cJSON *addrmap11 = cJSON_GetObjectItem(address_map_cfg, "ADDRMAP11");
        #if 0
        printf("%#lx\n", strtoul(addrmap1->valuestring, &p_end, 16));
        printf("%#lx\n", strtoul(addrmap3->valuestring, &p_end, 16));
        printf("%#lx\n", strtoul(addrmap4->valuestring, &p_end, 16));
        printf("%#lx\n", strtoul(addrmap5->valuestring, &p_end, 16));
        printf("%#lx\n", strtoul(addrmap6->valuestring, &p_end, 16));
        printf("%#lx\n", strtoul(addrmap7->valuestring, &p_end, 16));
        printf("%#lx\n", strtoul(addrmap8->valuestring, &p_end, 16));
        printf("%#lx\n", strtoul(addrmap9->valuestring, &p_end, 16));
        printf("%#lx\n", strtoul(addrmap10->valuestring, &p_end, 16));
        printf("%#lx\n", strtoul(addrmap11->valuestring, &p_end, 16));
        #endif

        addrmap[0] = strtoul(addrmap1->valuestring, &p_end, 16);
        addrmap[1] = strtoul(addrmap3->valuestring, &p_end, 16);
        addrmap[2] = strtoul(addrmap4->valuestring, &p_end, 16);
        addrmap[3] = strtoul(addrmap5->valuestring, &p_end, 16);
        addrmap[4] = strtoul(addrmap6->valuestring, &p_end, 16);
        addrmap[5] = strtoul(addrmap7->valuestring, &p_end, 16);
        addrmap[6] = strtoul(addrmap8->valuestring, &p_end, 16);
        addrmap[7] = strtoul(addrmap9->valuestring, &p_end, 16);
        addrmap[8] = strtoul(addrmap10->valuestring, &p_end, 16);
        addrmap[9] = strtoul(addrmap11->valuestring, &p_end, 16);
 	}
   
}

/*************************************************/
/* DDR0  0x30_0000_0000  0 :{bit38,bit35:bit34} */
/* DDR1  0x34_0000_0000  1 :{bit38,bit35:bit34} */
/* DDR2  0x38_0000_0000  2 :{bit38,bit35:bit34} */
/* DDR3  0x3C_0000_0000  3 :{bit38,bit35:bit34} */
/* DDR4  0x40_0000_0000  4 :{bit38,bit35:bit34} */
/* DDR5  0x44_0000_0000  5 :{bit38,bit35:bit34} */
/*                                              */
/* DDR01 0x10_0000_0000  0 :{bit37,bit35}       */
/* DDR23 0x18_0000_0000  1 :{bit37,bit35}       */
/* DDR45 0x20_0000_0000  2 :{bit37,bit35}       */
/************************************************/
#define BIT38_35_34_JOINT(x) (((x >> 34) & (0x3)) | ((x >> 36) & (0x4)))
#define BIT37_34_JOINT(x) (((x >> 34) & (0x1)) | ((x >> 36) & (0x2)))
int used_sys_detect(struct FILE_INFO *p_file_info, __uint32_t *p)
{
    __uint32_t *ddrsys_set = p;
    __uint32_t ddrsys_start = 0, ddrsys_end = 0;
    if (p_file_info->interleave_size) {
        ddrsys_start = BIT37_34_JOINT(p_file_info->img_start_address);
        ddrsys_end = BIT37_34_JOINT(p_file_info->img_end_address);
        for (int i = ddrsys_start; i <= ddrsys_end; i++) {
            *(ddrsys_set + i) = *(ddrsys_set + i + 1) = 1;
        }
    } else {
        ddrsys_start = BIT38_35_34_JOINT(p_file_info->img_start_address);
        ddrsys_end = BIT38_35_34_JOINT(p_file_info->img_end_address);
        for (int i = ddrsys_start; i <= ddrsys_end; i++) {
            *(ddrsys_set + i) = 1;
        }
    }
}

int main(int argc, char** argv) {
    char *input_file_name = "ddr_img.bin";
    char *json_file_name = "ddr_address_map.json";
    __uint64_t load_address = 0x3000000000;
    __uint8_t iecc = 0x0;
    __uint32_t interleave_size = 256;
    struct FILE_INFO head_file_info = {0};
    // Instantiate a new ArgParser instance.
    ArgParser* parser = ap_new();
    if (!parser) {
        exit(1);
    }

    // Register the program's helptext and version number.
    ap_set_helptext(parser, "Usage: example...");
    ap_set_version(parser, "1.0");

    // Register a flag and a string-valued and a integer-valued option.
    ap_flag(parser, "iecc e");
    ap_int_opt(parser, "interleave l", 256);
    ap_str_opt(parser, "input_file i", "ddrc.bin");
    ap_str_opt(parser, "json_file j", "ddr_address_map.json");
    ap_long_opt(parser, "address a", 0x3000000000);

    // Parse the command line arguments.
    if (!ap_parse(parser, argc, argv)) {
        exit(1);
    }

    // This debugging function prints the parser's state.
    // ap_print(parser);

    // printf("Inline ecc is %d\n", ap_found(parser, "e"));
    // printf("Input_file is %s\n", ap_str_value(parser, "input_file"));
    // printf("Json file is %s\n", ap_str_value(parser, "json_file"));
    // printf("Load address is %#lx\n", ap_long_value(parser, "a"));

    load_address = ap_long_value(parser, "a");
    input_file_name = ap_str_value(parser, "input_file");
    json_file_name = ap_str_value(parser, "json_file");
    head_file_info.iecc = ap_found(parser, "e");
    if (load_address > 0x2c00000000) {
        head_file_info.interleave_size = 0;
    } else {
        head_file_info.interleave_size = ap_int_value(parser, "interleave");
    }
    FILE * file_tmp = fopen(input_file_name, "rb");
    __uint64_t file_byte_size;
    if (file_tmp == NULL) {
        perror("Input file open failed\n");
    } else {
        head_file_info.img_file = file_tmp;
        fseek(file_tmp, 0, SEEK_SET);
        file_byte_size = ftell(file_tmp);
        if (0 == file_byte_size) {
            perror("Input file is NULL!\n");
        }
    }
    head_file_info.img_start_address = load_address;
    head_file_info.img_end_address = load_address + file_byte_size;
    // Free the parser's memory.
    ap_free(parser);

    // Input file open
    #if 0
    union address_bits test_address;
    test_address.addr = 0x403403;
    printf("module address %#lx to hif address is %#lx \n", test_address.addr, addr_moduled_to_hif(test_address));
    #endif

	__uint32_t addrmap[] = {
        0x19, 0x3f0903, 0x101, 0x1f020005, 0x4020500, 0x1f080808, 0x8080808, 0x8080808, 0x8080808, 0x808
    };
    parse_objects(json_file_name, addrmap);
    #if 0
    for (int i = 0; i < sizeof(addrmap)/sizeof(__uint32_t); i++) {
        printf("addrmap[%d]:%#x\n", i, addrmap[i]);
    }
    #endif

    hif_addr_update(addrmap);
    __uint32_t ddr_sys_set[DDR_SYS_NUM] = {0};
    used_sys_detect(&head_file_info, ddr_sys_set);
    linkedlist out_file_list = {0};
    linkedlist *p_out_file_list = &out_file_list;
    list_init(&p_out_file_list);
    for(int i = 0; i < DDR_SYS_NUM; i++) {
        if(ddr_sys_set[i]) {
            head_file_info.sys_num = i;
        }
        for (int j = 0; j < DDR_RANK_NUM; j++) {
            head_file_info.rank_num = j;
            for (int l = 0; l < 2; l++) {
                strcpy(head_file_info.ch_name, l ? "B" : "A");
                for (int k = 0; k < DDR_MEMCORE_NUM; k++) {
                    head_file_info.mem_core_num = k;
                    __uint32_t memcore_index = 0;
                    do{
                        memcore_file_create(p_out_file_list, &head_file_info, memcore_index);
                    } while(memcore_index++ < DDR_MEMCORE_INDEX_MAX);
                }
            }
        }
    }
    list_destroy(p_out_file_list, free_data);
}