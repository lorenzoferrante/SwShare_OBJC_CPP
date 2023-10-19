//
//  AESFunctions.h
//  BLELib2
//
//  Created by Lorenzo Ferrante on 17/10/23.
//

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

#ifdef __OBJC__
+ (uint32_t) data_encrypt:(unsigned char *)ptr and:(uint32_t)len and:(unsigned char *)key and:(aes_mode_t)mode;
#else
uint32_t data_encrypt(unsigned char *ptr, uint32_t len, unsigned char *key, aes_mode_t mode);
#endif

#ifdef __OBJC__
@end
#endif
