//
//  AESFunctions.h
//  BLELib2
//
//  Created by Lorenzo Ferrante on 17/10/23.
//
#ifndef AESFunctions_h
#define AESFunctions_h

#include <string.h>
#include <stdio.h>

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import <BLELib2/sw_share.h>

@interface AESFunctions : NSObject
#else

#include "sw_share.h"
#endif

#if !defined(__MENTOR_BUS__)
void print_hex(unsigned char *p, int len);
#endif

void xor_block( void *d, const void *s );
void copy_and_key( void *d, const void *s, const void *k );
void mix_sub_columns(uint8_t *dt);
void inv_mix_sub_columns(uint8_t *dt);
void inv_shift_sub_rows(uint8_t *st);
void add_round_key(uint8_t *d, const uint8_t *k);
void shift_sub_rows(uint8_t *st);

return_type_aes aes_set_key(const unsigned char *key, length_type_aes keylen, aes_context *ctx);
return_type_aes aes_encrypt(const unsigned char *in, unsigned char *out, const aes_context *ctx);
return_type_aes aes_cbc_encrypt(const unsigned char *in, unsigned char *out, uint16_t n_block, unsigned char *iv, const aes_context *ctx);
return_type_aes aes_decrypt(const unsigned char *in, unsigned char *out, const aes_context *ctx);
return_type_aes aes_cbc_decrypt(const unsigned char *in, unsigned char *out, uint16_t n_block, unsigned char *iv, const aes_context *ctx);

uint32_t data_pad(unsigned char *buffer, uint32_t size);

#ifdef __OBJC__
+ (uint32_t) data_encrypt:(unsigned char *)ptr and:(uint32_t)len and:(unsigned char *)key and:(aes_mode_t)mode;
#else
uint32_t data_encrypt(unsigned char *ptr, uint32_t len, unsigned char *key, aes_mode_t mode);
#endif


#ifdef __OBJC__
@end
#endif

#endif /* AESFunctions_h */
