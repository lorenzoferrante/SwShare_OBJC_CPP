#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstddef>
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
    // Wi-Fi baseline: fixed numeric commands, headers, payloads and encryption sizes.
    static_assert(COMMAND_GET_SET_PARAM == 3 && COMMAND_GET_SET_TIME == 5 && COMMAND_DEVICES_ONBOARD == 6);
    static_assert(COMMAND_CLOUD_CONFIG_SET == 7 && COMMAND_CLOUD_WIFI_CONFIG_SET == 8 && COMMAND_CLOUD_STATUS == 9);
    static_assert(PAR_COM_CALIBRAZIONE == 19 && PAR_AVAILABLE_20 == 38);
    static_assert(sizeof(get_set_param_query_t) == 32 && sizeof(get_set_param_answer_t) == 32);
    static_assert(offsetof(api_query_t, data) == 12 && offsetof(api_answer_t, data) == 12);
    static_assert(sizeof(api_query_t) == 1036 && sizeof(api_answer_t) == 1036);
    static_assert(sizeof(cloud_config_query_t) == 484 && sizeof(cloud_wifi_config_query_t) == 100);
    static_assert(sizeof(cloud_status_answer_t) == 300 && sizeof(cloud_status_query_t) == 16);
    static_assert(offsetof(cloud_config_query_t, wifi_ssid) == 4 && offsetof(cloud_status_answer_t, wifi_ssid) == 12);
    int size = prepare_data_for_cloud_config_set(&query, &answer, "wifi", "wifi-pass", "broker", 1883, "user", "mqtt-pass", "B07", 1);
    assert(size == 512);
    data_encrypt((uint8_t *)&query, size, (uint8_t *)TEMPORARY_MAIN_PWD, AES_DEC);
    assert(query.param.command == 7 && query.number == 484);
    assert(query.data.cloud_config.mqtt_port == 1883 && query.data.cloud_config.enabled == 1);
    assert(!strcmp(query.data.cloud_config.wifi_ssid, "wifi") && !strcmp(query.data.cloud_config.wifi_password, "wifi-pass"));
    assert(!strcmp(query.data.cloud_config.mqtt_host, "broker") && !strcmp(query.data.cloud_config.mqtt_username, "user"));
    assert(!strcmp(query.data.cloud_config.mqtt_password, "mqtt-pass") && !strcmp(query.data.cloud_config.device_id, "B07"));
    assert(query.crc == crc16(query.number + sizeof(query.number) + sizeof(query.param), (uint8_t *)&query.number));
    size = prepare_data_for_cloud_wifi_config_set(&query, &answer, "wifi", "password", 1);
    assert(size == 128);
    data_encrypt((uint8_t *)&query, size, (uint8_t *)TEMPORARY_MAIN_PWD, AES_DEC);
    assert(query.param.command == 8 && query.number == 100 && query.data.cloud_wifi_config.enabled == 1);
    assert(!strcmp(query.data.cloud_wifi_config.wifi_password, "password"));
    size = prepare_data_for_cloud_status(&query, &answer, nullptr, 1);
    assert(size == 32);
    data_encrypt((uint8_t *)&query, size, (uint8_t *)TEMPORARY_MAIN_PWD, AES_DEC);
    assert(query.param.command == 9 && query.number == 16);
    for (uint8_t command : {7, 8, 9}) {
        for (bool busy : {false, true}) {
            answer = {};
            answer.number = sizeof(cloud_status_answer_t);
            answer.param.negate_command = (uint8_t)~command;
            answer.param.flags.busy = busy;
            answer.param.flags.data_valid = !busy;
            answer.data.cloud_status.configured = 1;
            answer.data.cloud_status.wifi_state = CLOUD_STATE_CONNECTED;
            answer.data.cloud_status.mqtt_state = CLOUD_STATE_CONNECTING;
            answer.data.cloud_status.last_error = -23;
            answer.data.cloud_status.mqtt_port = 1883;
            strcpy(answer.data.cloud_status.device_id, "B07");
            answer.crc = crc16(answer.number + sizeof(answer.number) + sizeof(answer.param), (uint8_t *)&answer.number);
            assert(data_encrypt((uint8_t *)&answer, 12 + answer.number, (uint8_t *)TEMPORARY_MAIN_PWD, AES_ENC) == 320);
            auto encoded = answer;
            cloud_status_answer_t status{};
            int result;
            if (command == 7) {
                assert(prepare_data_for_cloud_config_set(&query, &answer, nullptr, nullptr, nullptr, 0, nullptr, nullptr, nullptr, 2) == 320);
                result = prepare_data_for_cloud_config_set(&query, &answer, nullptr, nullptr, nullptr, 0, nullptr, nullptr, nullptr, 0);
            } else if (command == 8) {
                assert(prepare_data_for_cloud_wifi_config_set(&query, &answer, nullptr, nullptr, 2) == 320);
                result = prepare_data_for_cloud_wifi_config_set(&query, &answer, nullptr, nullptr, 0);
            } else {
                assert(prepare_data_for_cloud_status(&query, &answer, nullptr, 2) == 320);
                result = prepare_data_for_cloud_status(&query, &answer, &status, 0);
                if (!busy) assert(status.last_error == -23 && !strcmp(status.device_id, "B07"));
            }
            assert(result == (busy ? -1 : 0));
            if (command == 9) {
                answer = encoded;
                ((uint8_t *)&answer)[0] ^= 1;
                assert(prepare_data_for_cloud_status(&query, &answer, &status, 0) < 0);
            }
        }
    }
    puts("BLE codec: timed parameters, stable legacy/cloud layouts, encrypted round trips and errors passed");
}
