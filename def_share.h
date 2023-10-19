//
// Created by pnenna on 09/04/2023.
//

#ifndef DMP_DEF_SHARE_H
#define DMP_DEF_SHARE_H

#include "sw_share.h"

//-----------------------------------------------------------------------------
//funzioni comuni
extern uint16_t crc16(uint32_t len, uint8_t *buf);
extern int32_t prepare_data_for_raw_read (api_query_t *query, api_answer_t *answer, uint32_t addr_device, uint16_t quanti, bool q);
extern int32_t prepare_data_for_raw_write (api_query_t *query, api_answer_t *answer, uint8_t * data, uint32_t addr_device, uint16_t quanti, bool q);
extern int32_t prepare_data_for_get_set_param (api_query_t *query, api_answer_t *answer, uint16_t answ_size, uint16_t param_num, uint32_t value, uint8_t write, uint8_t q);
extern int32_t prepare_data_for_gen_command (api_query_t *query, api_answer_t *answer, uint16_t size_query, uint16_t size_answer, uint8_t command, uint8_t q);
extern uint8_t isleap(uint8_t year);
extern void gettime(uint32_t t, t_calendar *r);
extern uint32_t maketime(t_calendar * t);
extern uint32_t get_my_date (void);

#endif //DMP_DEF_SHARE_H
