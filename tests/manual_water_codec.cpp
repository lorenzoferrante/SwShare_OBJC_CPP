#include <cassert>
#include <cstdio>
#include <initializer_list>
#include "FunShare.h"
#include "AESFunctions.h"

static api_answer_t response(uint32_t value, bool busy = false) {
    api_answer_t answer{};
    answer.number = sizeof(get_set_param_answer_t);
    answer.param.negate_command = (uint8_t)~COMMAND_GET_SET_PARAM;
    answer.param.flags.busy = busy;
    answer.param.flags.data_valid = !busy;
    answer.data.get_set_param.value = value;
    answer.crc = crc16(answer.number + sizeof(answer.number) + sizeof(answer.param), (uint8_t *)&answer.number);
    unsigned size = sizeof(api_answer_t) - sizeof(generic_answers_t) + answer.number;
    assert(data_encrypt((uint8_t *)&answer, size, (uint8_t *)TEMPORARY_MAIN_PWD, AES_ENC) == 48);
    return answer;
}
int main() {
    static_assert(N_PARAMETERS == 39);
    static_assert(PAR_RUB_DURATA_APERTURA_MANUALE == 22 && PAR_RUB_SUPPORTO_APERTURA_MANUALE == 25);
    api_query_t query{}; api_answer_t answer{};
    for (uint16_t index = 22; index <= 25; index++) {
        for (uint8_t write = 0; write <= 1; write++) {
            int size = prepare_data_for_get_set_param(&query, &answer, index, 180, write, 1);
            assert(size == 48); // Existing encrypted query format remains unchanged.
            data_encrypt((uint8_t *)&query, size, (uint8_t *)TEMPORARY_MAIN_PWD, AES_DEC);
            assert(query.param.command == COMMAND_GET_SET_PARAM);
            assert(query.data.get_set_param.param_num == index && query.data.get_set_param.write == write);
            assert(query.data.get_set_param.value == 180);
            assert(query.crc == crc16(query.number + sizeof(query.number) + sizeof(query.param), (uint8_t *)&query.number));
        }
    }
    assert(prepare_data_for_get_set_param(&query, &answer, 22, 0, 0, 2) == 48);
    for (uint32_t value : {0U, 1U, 2U, 3U, 180U, 3600U, (uint32_t)RUB_MANUAL_SUPPORT_SIGNATURE}) {
        answer = response(value);
        assert(prepare_data_for_get_set_param(&query, &answer, 0, 0, 0, 0) == (int32_t)value);
    }
    answer = response(1, true);
    assert(prepare_data_for_get_set_param(&query, &answer, 0, 0, 0, 0) == -1);
    answer = response(180);
    ((uint8_t *)&answer)[0] ^= 1;
    assert(prepare_data_for_get_set_param(&query, &answer, 0, 0, 0, 0) < 0);
    // ISL command still uses its independent existing contract.
    int size = prepare_data_for_isl(&query, &answer, ISL_OP_CALIB_STATO, 4, 1000, nullptr, nullptr, nullptr, 1);
    assert(size > 0);
    data_encrypt((uint8_t *)&query, size, (uint8_t *)TEMPORARY_MAIN_PWD, AES_DEC);
    assert(query.param.command == COMMAND_ISL29501 && query.data.isl.operazione == ISL_OP_CALIB_STATO);
    puts("BLE codec: encrypted get/set round trips, rejection, corruption and ISL compatibility passed");
}
