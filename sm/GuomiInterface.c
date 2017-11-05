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

void gm_generate_random(char *random_num)
{
    strcpy(random_num, "4C62EEFD6ECFC2B95B92FD6C3D9575148AFA17425546D49018E5388D49DD7B4F");
}

void gm_sm2_generate_keys(const char *random_num, char *public_key, char *private_key)
{
    char **sm2_param = sm2_param_recommand;
    int type = TYPE_GFp;
    int point_bit_length = 256;

    ec_param *ecp;
    sm2_ec_key *key_B;
    message_st message_data;
    
    strcpy(private_key, "00000000008f8b37dc19d95550fd06c1cacd43fe165f80e3b80242f0c66a733");
    
    ecp = ec_param_new();
    ec_param_init(ecp, sm2_param, type, point_bit_length);
    key_B = sm2_ec_key_new(ecp);
    
    sm2_ec_key_init(key_B, private_key, ecp);
    
    sprintf(public_key, "%s\n%s", key_B->P->x, key_B->P->y);
}

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
    //sm2_param_d_B[ecp->type] 测试自定义的私钥，key_B->P->x和key_B->P->y公钥

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
//    char **sm2_param = sm2_param_recommand;
//    int type = TYPE_GFp;
//    int point_bit_length = 256;
//
//    ec_param *ecp;
//    sm2_ec_key *key_B;
//    message_st message_data;
//
//    ecp = ec_param_new();
//    ec_param_init(ecp, sm2_param, type, point_bit_length);
//    key_B = sm2_ec_key_new(ecp);
//
//    //用私钥和随机数导出一个公钥，实际应用时没有私钥，也就是没有这行代码，直接设置下面的公钥
//    sm2_ec_key_init(key_B, sm2_param_d_B[ecp->type], ecp);//把中间的值给key_b的b
//
//    memset(&message_data, 0, sizeof(message_data));
//    //设置明文 这里输入一个字符串 如果输入char[]需要稍微改动
//    message_data.message = (BYTE*)text;
//
//    message_data.message_byte_length = (int)strlen((char *)message_data.message);
//    message_data.klen_bit = message_data.message_byte_length * 8;
//
//    //随机数 拷贝到message_data.k,实际使用时应该随机生成这个数
//    sm2_hex2bin((BYTE *)sm2_param_k[ecp->type], message_data.k, ecp->point_byte_length);
//
//    //设置公钥
//    sm2_bn2bin(key_B->P->x, message_data.public_key.x, ecp->point_byte_length);
//    sm2_bn2bin(key_B->P->y, message_data.public_key.y, ecp->point_byte_length);
//    DEFINE_SHOW_BIGNUM(key_B->P->x);//公钥PB =(xB ,yB ): 坐标xB :
//    DEFINE_SHOW_BIGNUM(key_B->P->y);//坐标yB :
//
//    //加密
//    sm2_encrypt(ecp, &message_data);
//    memcpy(encryptedText, message_data.C, sizeof(message_data.C));
//
//    printf(">>%s$$\n", encryptedText);
//
//    sm2_ec_key_free(key_B);
//    ec_param_free(ecp);
}

void gm_md5(const unsigned char* buffer, const size_t buffer_length, char* md5)
{
    DWORD hash[8] = {0};

    SM3_Init();
    SM3_Update((BYTE *)buffer, (DWORD)buffer_length);
    SM3_Final(hash);
    
    for (DWORD i = 0; i < sizeof(hash)/sizeof(DWORD); i++)
    {
        char buf[10] = {0};
        sprintf(buf, "%02x", hash[i]);
        strcat(md5, buf);
    }
}

long gm_sm4_calc_encrypted_data_memory_size(long data_len)
{
    if (data_len > 0)
    {
        const long m = 16;
        const long r = data_len % m;
        const long n = data_len / m;
        
        if (r == 0)
            return n * m;
        else
            return (n+1) * m;
    }
    
    return data_len;
}

void gm_sm4_encrypt(const char *key, const unsigned char *input, long len, unsigned char *output)
{
    sm4_context ctx;

    sm4_setkey_enc(&ctx, key);
    sm4_crypt_ecb(&ctx, 1, len, input, output);
}

void gm_sm4_decrypt(const char *key, const unsigned char *input, long len, unsigned char *output)
{
    sm4_context ctx;
    sm4_setkey_dec(&ctx, key);
    sm4_crypt_ecb(&ctx, 0, len, input, output);
}

