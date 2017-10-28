//
//  GuomiInterface.c
//  GuomiDemo
//
//  Created by 王军建 on 2017/10/26.
//  Copyright © 2017年 狗吃草. All rights reserved.
//

#include "GuomiInterface.h"
#include <openssl/rand.h>
#include <openssl/evp.h>
#include "sm2.h"
#include "sm3.h"
#include "sm4.h"
#include "util.h"

void gm_buffer2hexstr(const unsigned char* buffer, long len, char* hexstr)
{
    char* str = OPENSSL_buf2hexstr(buffer, len);
    if (str)
    {
        strcpy(hexstr, str);
        free(str);
    }
}

void gm_hexstr2buffer(const char* hexstr, unsigned char* buffer, long* buffer_len)
{
    long len = 0;
    unsigned char* buf = OPENSSL_hexstr2buf(hexstr, &len);
    if (buf)
    {
        memcpy(buffer, buf, len);
        *buffer_len = len;
        
        free(buf);
    }
}

char *OPENSSL_buf2hexstr(const unsigned char *buffer, long len);
unsigned char *OPENSSL_hexstr2buf(const char *str, long *len);

void gm_sm2_encrypt(const char* text, char* encryptedText)
{
    char **sm2_param = sm2_param_recommand;
    int type = TYPE_GFp;
    int point_bit_length = 256;
    
    ec_param *ecp;
    sm2_ec_key *key_B;
    message_st message_data;
    
    ecp = ec_param_new();
    ec_param_init(ecp, sm2_param, type, point_bit_length);
    key_B = sm2_ec_key_new(ecp);
    
    //用私钥和随机数导出一个公钥，实际应用时没有私钥，也就是没有这行代码，直接设置下面的公钥
    sm2_ec_key_init(key_B, sm2_param_d_B[ecp->type], ecp);//把中间的值给key_b的b
    
    memset(&message_data, 0, sizeof(message_data));
    //设置明文 这里输入一个字符串 如果输入char[]需要稍微改动
    message_data.message = (BYTE*)text;
    
    message_data.message_byte_length = (int)strlen((char *)message_data.message);
    message_data.klen_bit = message_data.message_byte_length * 8;
    
    //随机数 拷贝到message_data.k,实际使用时应该随机生成这个数
    sm2_hex2bin((BYTE *)sm2_param_k[ecp->type], message_data.k, ecp->point_byte_length);
    
    //设置公钥
    sm2_bn2bin(key_B->P->x, message_data.public_key.x, ecp->point_byte_length);
    sm2_bn2bin(key_B->P->y, message_data.public_key.y, ecp->point_byte_length);
    DEFINE_SHOW_BIGNUM(key_B->P->x);//公钥PB =(xB ,yB ): 坐标xB :
    DEFINE_SHOW_BIGNUM(key_B->P->y);//坐标yB :
    
    //加密
    sm2_encrypt(ecp, &message_data);
    memcpy(encryptedText, message_data.C, sizeof(message_data.C));
    
    printf(">>%s$$\n", encryptedText);
    
    sm2_ec_key_free(key_B);
    ec_param_free(ecp);
}

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

void gm_sms4(void)
{
    const int len = 80;
    unsigned char key[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
//    unsigned char input[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
    unsigned char output[len];
    sm4_context ctx;
    unsigned long i;
    
    char* str = "hello123456789012345612345678sdfff";
    char* input = OPENSSL_buf2hexstr((const unsigned char *)str, strlen(str));
    for(i=0;i<len;i++)
        printf("%02x ", input[i]);
    printf("\n");


    //encrypt standard testing vector
    sm4_setkey_enc(&ctx,key);
    sm4_crypt_ecb(&ctx,1,len,input,output);
    for(i=0;i<len;i++)
        printf("%02x ", output[i]);
    printf("\n");
    
    //decrypt testing
    sm4_setkey_dec(&ctx,key);
    sm4_crypt_ecb(&ctx,0,len,output,output);
    for(i=0;i<len;i++)
        printf("%02x ", output[i]);
    printf("\n");
    
//    char* sstr = OPENSSL_hexstr2buf(hexstr, 0);
//    printf("sstr>>%s\n", sstr);
}
