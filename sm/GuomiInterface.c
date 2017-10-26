//
//  GuomiInterface.c
//  GuomiDemo
//
//  Created by 王军建 on 2017/10/26.
//  Copyright © 2017年 狗吃草. All rights reserved.
//

#include "GuomiInterface.h"
#include "sm3.h"
#include "util.h"

void gm_md5(const char* str, const size_t str_length, char* md5)
{
    DWORD hash[8] = {0};

    char* hexstr = OPENSSL_buf2hexstr((const unsigned char *)str, str_length);
    if (!hexstr)
        return;

    unsigned char* bin_str = OPENSSL_hexstr2buf(hexstr, 0);
    
    free(hexstr);
    
    if (!bin_str)
        return;
    
    SM3_Init();
    SM3_Update((BYTE *)bin_str, (DWORD)str_length);
    SM3_Final(hash);

    free(bin_str);
    
    for (DWORD i = 0; i < sizeof(hash)/sizeof(DWORD); i++)
    {
        char buf[10] = {0};
        sprintf(buf, "%02x", hash[i]);
        strcat(md5, buf);
    }
}
