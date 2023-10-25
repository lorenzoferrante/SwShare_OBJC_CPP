//
//  FunShare.hpp
//  BLELib2
//
//  Created by Lorenzo Ferrante on 13/10/23.
//

#ifndef FunShare_h
#define FunShare_h

#include <stdio.h>
#include <stdint.h>

#define MAX_NUM_READ_WRITE (128)
#define MAX_MTU (128)

#ifdef __OBJC__
#include <Foundation/Foundation.h>
#import <BLELib2/sw_share.h>

@interface FunShare : NSObject
#else

#include "sw_share.h"
#endif

void printFunc(void);
uint16_t crc16(uint32_t len, uint8_t *buf);
int32_t prepare_data_for_raw_read (api_query_t *query, api_answer_t *answer, uint32_t addr_device, uint16_t quanti, bool q);
int32_t prepare_data_for_raw_write (api_query_t *query, api_answer_t *answer, uint8_t * data, uint32_t addr_device, uint16_t quanti, bool q);
int32_t prepare_data_for_get_set_param (api_query_t *query, api_answer_t *answer, uint16_t answ_size, uint16_t param_num, uint32_t value, uint8_t write, uint8_t q);
int32_t prepare_data_for_gen_command (api_query_t *query, api_answer_t *answer, uint16_t size_query, uint16_t size_answer, uint8_t command, uint8_t q);
uint8_t isleap(uint8_t year);
void gettime(uint32_t t, t_calendar *r);
uint32_t _mktime(t_calendar * t);
uint32_t maketime(t_calendar *t);
uint32_t get_my_date(void);
char* test_func(void);
    
#ifdef __OBJC__
@end
#endif

#endif /* FunShare_h */
