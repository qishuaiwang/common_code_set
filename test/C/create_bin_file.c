/**
 *  Horizon Robotics
 *
 *  Copyright (C) 2022 Horizon Robotics Inc.
 *  All rights reserved.
 *
 * @file        create_bin_file.c
 * @brief       C code source file template
 * @details     header file to reference
 * @author      qishuai.wang@horizon.ai
 * @date        2022-11-08
 * @version     v0.0.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int uint16_vhx_create()
{
    #define TEST_BYTE_NUM 256
    #define BYTE_ALIGN(x) ((x >> 3))
   __uint16_t sentence[TEST_BYTE_NUM ] = {'0','1'};
   FILE *fptr;
   __uint16_t *str = sentence;
   __uint32_t width = 16;
   __uint32_t banks = 2;
   __uint32_t i = 0, j = 0, k = 0;
 
   fptr = fopen("test-0.vhx", "w+t");
   if(fptr == NULL)
   {
      printf("Error!");
      exit(1);
   }
   k = 0;
    for (i = 0; i < TEST_BYTE_NUM / banks; i += (BYTE_ALIGN(width) * banks)) {
        for (j = 0; j < banks; j++) {
            // sentence[k++] = (i + j);
            sprintf(str++, "%2x", (i + j));
            // itoa((i + j), &sentence[k++], 16);
        }
   }
   fprintf(fptr,"%s", sentence);
   fclose(fptr);

   fptr = fopen("test-1.vhx", "w+t");
   if(fptr == NULL)
   {
      printf("Error!");
      exit(1);
   }
   k = 0;
    for (i = 0; i < TEST_BYTE_NUM / banks; i += (BYTE_ALIGN(width) * banks)) {
        for (j = 0; j < banks; j++) {
            sprintf(str++, "%2x", (2 + i + j));
            // sentence[k++] = itoa(i + j + 2);
        }
   }
//    printf("输入字符串:\n");
//    fgets(sentence, (sizeof sentence / sizeof sentence[0]), stdin);
 
   fprintf(fptr,"%s", sentence);
   fclose(fptr);
 
   return 0;
}

union uint32_char
{
    /* data */
    __uint32_t data;
    char bytes[4];
};

int main(int argc, char** argv)
{
    union uint32_char temp;
    __uint64_t i = 0;
    __uint64_t file_size = 4*1024ul*1024ul*1024ul;
    char *bin_file_name = "sequence.bin";

    char filename[100] = {0};
    if (argc > 1) {
        strcpy(filename, argv[2]);
        bin_file_name = filename;    
        file_size = strtoul(argv[1], NULL, 0);
    }
    printf("bin file name: %s size:%#lx\n", bin_file_name, file_size);
    FILE *p_file = fopen(bin_file_name, "wb");
    for (i = 0; i < file_size/4; i++) {
        temp.data = i * sizeof(__uint32_t);
        fwrite(temp.bytes, sizeof(char), sizeof(__uint32_t), p_file);
    }
    fclose(p_file);
    return 0;
}