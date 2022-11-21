#include <stdio.h>
#include <stdlib.h>
#include "args_analysis.h"
#include "linked_list.h"
#include "data_repackage.h"
#include "cJSON.h"

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

int main(int argc, char** argv) {
    char *json_file_name = "ddr_address_map.json";
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
    ap_str_opt(parser, "input_file i", "ddrc.bin");
    ap_str_opt(parser, "json_file j", "ddr_address_map.json");
    ap_long_opt(parser, "address a", 0x3000000000);

    // Parse the command line arguments.
    if (!ap_parse(parser, argc, argv)) {
        exit(1);
    }

    // This debugging function prints the parser's state.
    ap_print(parser);

    printf("Inline ecc is %d\n", ap_found(parser, "e"));
    printf("Input_file is %s\n", ap_str_value(parser, "input_file"));
    printf("Json file is %s\n", ap_str_value(parser, "json_file"));
    printf("Load address is %#lx\n", ap_long_value(parser, "a"));

    json_file_name = ap_str_value(parser, "json_file");
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

    hif_addr_update (addrmap);
}