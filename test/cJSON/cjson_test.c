#include <stdio.h>
#include <stdlib.h>
#include <cJSON.h>

/* Create a objects as address_map_cfg. */
void create_objects(FILE *p_file)
{
	cJSON *root,*address_map;char *out;int i;	/* declare a few. */
    static const __uint32_t lpddr5_addrmap_2cs_16GB[] = {
        0x19, 0x3f0903, 0x101, 0x1f020005, 0x4020500, 0x1f080808, 0x8080808, 0x8080808, 0x8080808, 0x808
    };
    static const __uint32_t lpddr4_addrmap_2cs_8GB[] = {
        0x18, 0x70707, 0x3f3f, 0x1f000000, 0x0, 0x1f070707, 0x7070707, 0x7070707, 0x7070707, 0x707
    };
	
	/* addressmap array: */
	root=cJSON_CreateObject();	
    address_map=cJSON_CreateIntArray(lpddr5_addrmap_2cs_16GB, 10);
	cJSON_AddItemToObject(root, "address_map_cfg",address_map);

	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);	free(out);	/* Print to text, Delete the cJSON, print it, release the string. */
	/* addressmap item: */
	root=cJSON_CreateObject();	
    address_map=cJSON_CreateObject();
	cJSON_AddItemToObject(root, "address_map_cfg",address_map);
	// cJSON_AddNumberToObject(address_map, "ADDRMAP1", 0x19);
	// cJSON_AddNumberToObject(address_map, "ADDRMAP3", 0x3f0903);
	// cJSON_AddNumberToObject(address_map, "ADDRMAP4", 0x101);
	// cJSON_AddNumberToObject(address_map, "ADDRMAP5", 0x1f020005);
	// cJSON_AddNumberToObject(address_map, "ADDRMAP6", 0x4020500);
	// cJSON_AddNumberToObject(address_map, "ADDRMAP7", 0x1f080808);
	// cJSON_AddNumberToObject(address_map, "ADDRMAP8", 0x08080808);
	// cJSON_AddNumberToObject(address_map, "ADDRMAP9", 0x08080808);
	// cJSON_AddNumberToObject(address_map, "ADDRMAP10", 0x08080808);
	// cJSON_AddNumberToObject(address_map, "ADDRMAP11", 0x0808);
	cJSON_AddStringToObject(address_map, "ADDRMAP1", "0x00000019");
	cJSON_AddStringToObject(address_map, "ADDRMAP3", "0x003f0903");
	cJSON_AddStringToObject(address_map, "ADDRMAP4", "0x00000101");
	cJSON_AddStringToObject(address_map, "ADDRMAP5", "0x1f020005");
	cJSON_AddStringToObject(address_map, "ADDRMAP6", "0x04020500");
	cJSON_AddStringToObject(address_map, "ADDRMAP7", "0x1f080808");
	cJSON_AddStringToObject(address_map, "ADDRMAP8", "0x08080808");
	cJSON_AddStringToObject(address_map, "ADDRMAP9", "0x08080808");
	cJSON_AddStringToObject(address_map, "ADDRMAP10", "0x08080808");
	cJSON_AddStringToObject(address_map, "ADDRMAP11", "0x00000808");

	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out); fprintf(p_file,"%s\n",out); free(out);	/* Print to text, Delete the cJSON, print it, release the string. */
}
/* Parse text to JSON, then render back to text, and print! */
void doit(char *text)
{
	char *out;cJSON *json;
	
	json=cJSON_Parse(text);
	if (!json) {printf("Error before: [%s]\n",cJSON_GetErrorPtr());}
	else
	{
		out=cJSON_Print(json);
		cJSON_Delete(json);
		printf("%s\n",out);
		free(out);
	}
}

/* Read a file, parse, render back, etc. */
void dofile(char *filename)
{
	FILE *f;long len;char *data;
	
	f=fopen(filename,"rb");fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
	data=(char*)malloc(len+1);fread(data,1,len,f);fclose(f);
	doit(data);
	free(data);
}


/* Parse a addressmp to array. */
void parse_objects(char *filename)
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
 	}
   
}



int main (int argc, const char * argv[])
{
    char *json_file_name = "ddr_address_map.json";
    #if 1
    /* create json file */
     FILE *p_json_file = fopen(json_file_name, "w");
     if (p_json_file!=NULL)
     {
         // create json file
         create_objects(p_json_file);
         fclose (p_json_file);
     } else {
         printf("ddr_address_map.json open failed.\n");
     }


    /* parse json file */
    dofile(json_file_name);
    #endif

    parse_objects(json_file_name);
}