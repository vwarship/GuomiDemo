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

void gm_buffer2hexstr(const unsigned char *buffer, long len, char *hexstr);
void gm_hexstr2buffer(const char *hexstr, unsigned char *buffer, long *buffer_len);

void gm_generate_random(char *random_num);

void gm_sm2_generate_keys(const char *random_num, char *public_key, char *private_key);
void gm_sm2_encrypt(const char *public_key, const unsigned char *text, char unsigned *encryptedText);
void gm_sm2_decrypt(const char *private_key, const unsigned char *encryptedText, char unsigned *text);

void gm_md5(const unsigned char *buffer, const size_t buffer_length, char *md5);

long gm_sm4_calc_encrypted_data_memory_size(long data_len);
void gm_sm4_encrypt(const char *key, const unsigned char *input, long len, unsigned char *output);
void gm_sm4_decrypt(const char *key, const unsigned char *input, long len, unsigned char *output);

#endif /* GuomiInterface_h */
