//
//  GuomiInterface.h
//  GuomiDemo
//
//  Created by 王军建 on 2017/10/26.
//  Copyright © 2017年 狗吃草. All rights reserved.
//

#ifndef GuomiInterface_h
#define GuomiInterface_h

#include <stdio.h>

void gm_buffer2hexstr(const unsigned char* buffer, long len, char* hexstr);
void gm_hexstr2buffer(const char* hexstr, unsigned char* buffer, long* buffer_len);

void gm_sm2_encrypt(const char* text, char* encryptedText);

void gm_md5(const char* str, const size_t str_length, char* md5);

void gm_sms4(void);

#endif /* GuomiInterface_h */
