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
#include "part4.h"

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
    
    char *x = BN_bn2hex(key_B->P->x);
    char *y = BN_bn2hex(key_B->P->y);
    
    sprintf(public_key, "%s\n%s", x, y);
    
    OPENSSL_free(x);
    OPENSSL_free(y);
}

static char **sm2_param = sm2_param_recommand;
static int sm2_type = TYPE_GFp;
static int sm2_point_bit_length = 256;

void gm_sm2_encrypt(const char *public_key, const unsigned char *text, long text_length, char unsigned *encrypted_text)
{
    if (!public_key)
        return;

    const int public_key_buf_len = 1024;
    if (public_key_buf_len <= strlen(public_key))
        return;

    ec_param *ecp;
    message_st message_data;

    char public_key_buf[public_key_buf_len] = {'\0'};
    strcpy(public_key_buf, public_key);

    char *x = public_key_buf;
    char *y = NULL;
    for (int i=0; i<strlen(public_key); ++i)
    {
        if (x[i] == '\n')
        {
            x[i] = '\0';
            y = x + i + 1;
        }
    }

    BIGNUM *bn_x = BN_new();
    BIGNUM *bn_y = BN_new();
    BN_hex2bn(&bn_x, x);
    BN_hex2bn(&bn_y, y);

    ecp = ec_param_new();
    ec_param_init(ecp, sm2_param, sm2_type, sm2_point_bit_length);

    memset(&message_data, 0, sizeof(message_data));

    message_data.message = (BYTE*)text;
    message_data.message_byte_length = text_length;
    message_data.klen_bit = message_data.message_byte_length * 8;

//    //随机数 拷贝到message_data.k,实际使用时应该随机生成这个数
//    sm2_hex2bin((BYTE *)sm2_param_k[ecp->type], message_data.k, ecp->point_byte_length);

    //设置公钥 公钥PB =(xB ,yB ): 坐标xB
    sm2_bn2bin(bn_x, message_data.public_key.x, ecp->point_byte_length);
    sm2_bn2bin(bn_y, message_data.public_key.y, ecp->point_byte_length);

    BN_clear_free(bn_x);
    BN_clear_free(bn_y);

    //加密
    sm2_encrypt(ecp, &message_data);
    ec_param_free(ecp);

    memcpy(encrypted_text, message_data.C, sizeof(message_data.C));
}

void gm_sm2_decrypt(const char *private_key, const unsigned char *encrypted_text, long encrypted_text_length, char unsigned *text)
{
    message_st message_data;

    ec_param *ecp;
    ecp = ec_param_new();
    ec_param_init(ecp, sm2_param, sm2_type, sm2_point_bit_length);

    sm2_ec_key *key_B;
    key_B = sm2_ec_key_new(ecp);
    sm2_ec_key_init(key_B, private_key, ecp);

    memset(&message_data, 0, sizeof(message_data));
    //明文的长度，这个长度应该根据密文计算，这里固定写6
    message_data.message_byte_length = 6;
    //k的比特长度是明文长度*8
    message_data.klen_bit = message_data.message_byte_length * 8;

    sm2_bn2bin(key_B->d, message_data.private_key, ecp->point_byte_length);
    message_data.decrypt = (BYTE *)OPENSSL_malloc(message_data.message_byte_length + 1);
    memset(message_data.decrypt, 0, message_data.message_byte_length + 1);

    //设置密文
    for (int i = 0; i < encrypted_text_length; i++)
        message_data.C[i] = encrypted_text[i];

    sm2_decrypt(ecp, &message_data);
    memcpy(text, message_data.decrypt, strlen(message_data.decrypt));
    OPENSSL_free(message_data.decrypt);

    sm2_ec_key_free(key_B);
    ec_param_free(ecp);
}

void gm_md5(const unsigned char* buffer, long buffer_length, char* md5)
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

