/**
 *  Horizon Robotics
 *
 *  Copyright (C) 2022 Horizon Robotics Inc.
 *  All rights reserved.
 *
 * @file        splite_bin_file.c
 * @brief       C code source file template
 * @details     header file to reference
 * @author      qishuai.wang@horizon.ai
 * @date        2022-11-08
 * @version     v0.0.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LE 1
// #define DBG
// #define DBG_1
#define SWAP(a, b) do { typeof(a) temp = a; a = b; b = temp;} while(0)
/* func : data swap */
int data_swap (__uint8_t *data, __uint32_t num, __uint32_t sub_num)
{
    __uint8_t i = 0, j = 0, k = 0;
    __uint32_t swap_num = num / sub_num / 2;
    __uint8_t *p_data = data;
    if (num % sub_num) {
        printf ("%s %d size Error!", __func__, __LINE__);
        return -1;
    }
    for (i = 0; i < swap_num; i++) {
        for (j = 0; j < sub_num; j++) {
            SWAP(*(p_data + j), *(p_data + num - sub_num + j));
        }
    }
    return 0;
}

int main()
{
    #define TEST_DATA_NUM 25 * 1024
    #define DATA_BYTES 4
    #define BYTE_TO_ASCII_COL 2
    #define BYTE_ALIGN(x) ((x >> 3))
    __uint8_t sentence[TEST_DATA_NUM * DATA_BYTES] = {0}; //buffer for bin data
    char str_buf[TEST_DATA_NUM * DATA_BYTES * BYTE_TO_ASCII_COL] = {0}; //buffer for string
    char file_buf[TEST_DATA_NUM * DATA_BYTES * BYTE_TO_ASCII_COL] = {0}; //buffer for string
    FILE *fptr;
    __uint8_t *str = sentence;
    __uint32_t width = 16;
    __uint32_t banks = 2;
    __uint32_t data_bytes = DATA_BYTES;
    __uint32_t i = 0, j = 0, k = 0, l = 0;
 
//  create bin data
    __uint8_t *ptr_uint8 = sentence;
    __uint16_t *ptr_uint16 = (__uint16_t *)sentence;
    __uint32_t *ptr_uint32 = (__uint32_t *)sentence;
    __uint64_t *ptr_uint64 = (__uint64_t *)sentence;
    for (i = 0; i < TEST_DATA_NUM; i++) {
        switch (data_bytes) {
        case 1/* sizeof(__uint8_t)constant-expression */:
            /* code */
            *ptr_uint8++ = i;
            break;
        case 2/* sizeof(__uint16_t)constant-expression */:
            /* code */
            *ptr_uint16++ = i;
            break;
        case 4/* sizeof(__uint32_t)constant-expression */:
            /* code */
            *ptr_uint32++ = i;
            break;
        case 8/* sizeof(__uint64_t)constant-expression */:
            /* code */
            *ptr_uint64++ = i;
            break;
        default:
            printf("Error!\r\n");
            break;
        }
    }
    #ifdef DBG    
    for (i = 0; i < TEST_DATA_NUM * data_bytes; i++) {
        printf ("%02x%s", sentence[i], (i + 1) % 4 ? "" :"\r\n");
    }
    #endif
//  convert bin data to txt
    char *p_str_buf = str_buf;
    for (i = 0; i < TEST_DATA_NUM * data_bytes; i++) {
        sprintf(p_str_buf, "%02x", sentence[i]);
        p_str_buf += BYTE_TO_ASCII_COL;
    }
    #ifdef DBG_1
    for (i = 0; i < TEST_DATA_NUM * data_bytes * BYTE_TO_ASCII_COL; i++) {
        printf ("%c%s", str_buf[i], (i + 1) % (4 * BYTE_TO_ASCII_COL) ? "" :"\r\n");
    }
    #endif
    fptr = fopen("test.vhx", "w+t");
    if(fptr == NULL)
    {
       printf("Error!");
       exit(1);
    }
    fprintf(fptr,"%s", str_buf);
    fclose(fptr);

//  split if need
    for(k = 0; k < banks; k++) {
        char *p_file_buf = file_buf;
        p_str_buf = str_buf;
        l = 0;
        char file_name[30] = {0};
        sprintf(file_name, "test_%d.vhx", k);
        for (i = 0; i < TEST_DATA_NUM * data_bytes ; i += BYTE_ALIGN(width) * BYTE_TO_ASCII_COL * (banks)) {
            __uint32_t position = i + BYTE_ALIGN(width) * BYTE_TO_ASCII_COL * k;
            if (LE) {
                // __uint32_t position = i + BYTE_ALIGN(width) * BYTE_TO_ASCII_COL * (k + 1) - 1;
                // for (j = BYTE_ALIGN(width) * BYTE_TO_ASCII_COL; j > 0; j--) {
                    // file_buf[l++] = str_buf[position--];
                // }
                memcpy(p_file_buf + l, p_str_buf + position, BYTE_ALIGN(width) * BYTE_TO_ASCII_COL);
                data_swap (p_file_buf + l, BYTE_ALIGN(width) * BYTE_TO_ASCII_COL, BYTE_TO_ASCII_COL);
                l += BYTE_ALIGN(width) * BYTE_TO_ASCII_COL;
                // format to vhx
                *(p_file_buf + l++) = '\n';
            } else {
                __uint32_t position = i + BYTE_ALIGN(width) * BYTE_TO_ASCII_COL * k;
                for (j = 0; j < BYTE_ALIGN(width) * BYTE_TO_ASCII_COL; j++) {
                    file_buf[l++] = str_buf[position++];
                }
            }
        }
        

        //write to file
        fptr = fopen(file_name, "w+t");
        if(fptr == NULL)
        {
            printf("Error!");
            exit(1);
        }
        fprintf(fptr,"%s", file_buf);
        fclose(fptr);
    }


   return 0;
}
