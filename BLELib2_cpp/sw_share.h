//
//  sw_share.h
//  BLELib2
//
//  Created by Lorenzo Ferrante on 17/10/23.
//

#ifndef sw_share_h
#define sw_share_h

#include <stdint.h>

//-----------------------------------------------------------------------------
#define TEMPORARY_MAIN_PWD ("#!(6701-k*/32.0=")
#define CMP_CRIPTAZIONE (1)

//-----------------------------------------------------------------------------
#define PACKED __attribute__((packed, aligned(1)))
#define MAX_PACKET_SIZE (1024)
#define TIMING_BUDGET_uS (20000)
#define INTER_MEASUREMENT_mS (80)
#define CMP_MAX_DISTANZA (1100)
#define TEMPO_SISTEMA (100)
#define SYS_MS (x) (x / TEMPO_SISTEMA) //macro per i timers SYS
#define CALIBRAZIONE (40000)
#define TEMPO_ANTI_RIMBALZO (1000) //filtro anti rimbalzo alla chiusura in ms

//i2c
#define I2C_MASTER_NUM (0) /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_SCL_IO PIN_I2C_MASTER_SCL /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO PIN_I2C_MASTER_SDA /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_FREQ_HZ (100000) /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE (0) /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE (0) /*!< I2C master doesn't need buffer */
#define VL53L1_SENSOR_ADDRESS (0x52 >> 1) //shift right di un bit in quanto la funzione di invio effettua nuovamente lo shift left

//pin
#define PIN_I2C_MASTER_SCL (6)
#define PIN_I2C_MASTER_SDA (5)
#define PIN_LED (2)
#define PIN_XSHUT (7)

#define PIN_K1 (8)
#define PIN_K2 (10)
#define PIN_K3 (9)
#define PIN_K4 (4)

typedef uint16_t tempo_tw;

//-----------------------------------------------------------------------------
//attributi timers
#define SCADUTO (0)
#define FERMO (0xFF)
#define FERMO_W (0xFFFF)
#define FERMO_32 (0xffffffff UL) //timer a 32 bit fermo

//-----------------------------------------------------------------------------
//definizioni generiche
#define MAX_NUM_READ_WRITE (128)
#define MAX_MTU (128)

//-----------------------------------------------------------------------------
// struttura calendario
#define SPD (86400) // Secondi in un giorno.
#define MAGIC_WDAY 6 // il 1 gennaio 2000 era un sabato.
#define MAGIC_YEAR 2000
#define MAGIC_PREF 2000
#define MAGIC_SUFF 0

//-----------------------------------------------------------------------------
typedef uint8_t return_type_aes;
typedef uint8_t length_type_aes;

#define ENC_KEY_LENGTH (16)
#define N_ROW_AES (4)
#define N_COL_AES (4)
#define N_BLOCK_AES (ENC_KEY_LENGTH)
#define N_MAX_ROUNDS_AES (14)

//-----------------------------------------------------------------------------
typedef enum {
    AES_ENC,
    AES_DEC
}
        aes_mode_t;

//-----------------------------------------------------------------------------
typedef struct {
    uint8_t ksch[(N_MAX_ROUNDS_AES + 1) * N_BLOCK_AES];
    uint8_t rnd;
}
        aes_context;

//-----------------------------------------------------------------------------
typedef struct t_param {
    uint32_t minimo;
    uint32_t fabbrica;
    uint32_t massimo;
}
        t_param;

//-----------------------------------------------------------------------------
typedef struct {
    uint8_t sec; // Secondi [0-59]
    uint8_t min; // Minuti  [0-59]
    uint8_t hour; // Ore     [0-23]
    uint8_t day; // Giorni  [1-31]
    uint8_t mon; // Mese    [0-11]
    uint8_t year; // Anno    [0-135]
    uint8_t wday; // giorno  [0-6] 0=Dom 1=Lun]
    uint8_t mode; // MODE_ITA, MODE_USA, MODE_ISO
}
        t_calendar;

//-----------------------------------------------------------------------------
typedef enum {
    COM_CHANNEL_BLE,
    COM_CHANNEL_SERIAL,
    N_COM_CHANNEL,
}
    PACKED COM_CHANNELS_T;

//-----------------------------------------------------------------------------
typedef enum
{
	COMMAND_NULL,
	COMMAND_RAW_READ,
	COMMAND_RAW_WRITE,
	COMMAND_GET_SET_PARAM,
	COMMAND_GREETING_MESSAGE,
	COMMAND_GET_SET_TIME,
} PACKED API_COMMAND_T;

//-----------------------------------------------------------------------------
typedef enum {
    PARAMETER_SENSITIVITY,
    PARAMETER_FLUSH_TIME,
    N_PARAMETERS,
}
    PACKED PARAMETERS_T;

//-----------------------------------------------------------------------------
//timers a word a 30ms
typedef struct {
    tempo_tw ANTI_ALLAGAMENTO;
    tempo_tw ANTI_RIMBALZO;
}
    PACKED t_sysw_timers;

//-----------------------------------------------------------------------------
//QUERIES
//-----------------------------------------------------------------------------
typedef struct
{
	uint32_t									address;
	uint16_t									byte_number;
} PACKED raw_read_query_t;

//-----------------------------------------------------------------------------
typedef struct
{
	uint32_t									address;
	uint16_t									byte_number;
	uint8_t									data[];
} PACKED raw_write_query_t;

//-----------------------------------------------------------------------------
typedef struct
{
	uint16_t									param_num;		//numero di parametro
	uint8_t									write;			//0=leggi, 1=scrivi
	uint8_t									spare;			//mettere a 0
	uint32_t									value;			//se write = 1 valore da scrivere nel parametro altrimenti metttere a 0
	uint8_t									spare_1[24];	//serve per portare a 32 byte la query, mettere a 0
} PACKED get_set_param_query_t;

//-----------------------------------------------------------------------------
typedef struct
{
	uint8_t									data[32];
	uint8_t									spare[32];
} PACKED greeting_message_query_t;

//-----------------------------------------------------------------------------
typedef struct
{
	uint32_t									time;				//in caso di set riportare il tempo in secondi a partire dal primo gennaio 2000
	uint8_t									get_set;			//0 = get, 1 = set
	uint8_t									spare[59];
} PACKED get_set_time_query_t;

//-----------------------------------------------------------------------------
typedef union
{
	uint8_t									max_packet[MAX_PACKET_SIZE];
	raw_read_query_t						raw_read;
	raw_write_query_t						raw_write;
	get_set_param_query_t				get_set_param;
	greeting_message_query_t			greeting_message;
	get_set_time_query_t					get_set_time;
} PACKED generic_queries_t;

//-----------------------------------------------------------------------------
typedef struct
{
	uint8_t									command;			//comando (vedere)
	uint8_t									brand;			//brand del dispositivo (0 = DMP)
	uint16_t									spare;			//mettere a 0
	uint32_t									timestamp;
} PACKED query_parameters_t;

//-----------------------------------------------------------------------------
typedef struct
{
	uint16_t									crc;				//crc dei campi successivi
	uint16_t									number;			//numero di byte del solo campo "data" (usato per calcolare il crc)
	query_parameters_t					param;
	generic_queries_t						data;
} PACKED api_query_t;


//-----------------------------------------------------------------------------
//ANSWERS
//-----------------------------------------------------------------------------
typedef struct
{
	uint16_t									number;			//numero di byte del solo campo "data"
	uint8_t									data[];
} PACKED raw_read_answer_t;

//-----------------------------------------------------------------------------
typedef struct
{
	uint32_t									dummy;
} PACKED raw_write_answer_t;

//-----------------------------------------------------------------------------
typedef struct
{
	uint32_t									value;			//se read = 1 sulla query valore del parametro altrimenti 0
	uint8_t									spare_1[28];	//serve per portare a 32 byte l'answer
} PACKED get_set_param_answer_t;

//-----------------------------------------------------------------------------
typedef struct
{
	uint8_t									data[32];
	uint8_t									spare[32];
} PACKED greeting_message_answer_t;

//-----------------------------------------------------------------------------
typedef struct
{
	uint32_t									time;				//in caso di get time riporta i secondi trascorsi dal primo gennaio 2000
	uint8_t									spare[60];
} PACKED get_set_time_answer_t;

//-----------------------------------------------------------------------------
typedef union
{
	uint8_t									max_packet[MAX_PACKET_SIZE];
	raw_read_answer_t						raw_read;
	raw_write_answer_t					raw_write;
	get_set_param_answer_t				get_set_param;
	greeting_message_answer_t			greeting_message;
	get_set_time_answer_t				get_set_time;
} PACKED generic_answers_t;

//-----------------------------------------------------------------------------
typedef struct
{
	uint32_t									busy							:1;
	uint32_t									crc_error					:1;
	uint32_t									size_error					:1;
	uint32_t									data_valid					:1;
	uint32_t									date_time_error			:1;	//1=errore sulla data-ora (sviluppi futuri)
	uint32_t									encryption_error			:1;	//1=errore sulla decriptazione dei dati in arrivo
	uint32_t									spare							:26;
} PACKED answer_flags_t;

//-----------------------------------------------------------------------------
typedef struct
{
	uint8_t									negate_command;
	uint8_t									spare_1;			//mettere a 0
	uint16_t									spare_2;			//mettere a 0
	answer_flags_t							flags;
} PACKED answer_parameters_t;

//-----------------------------------------------------------------------------
typedef struct
{
	uint16_t									crc;				//crc dei campi successivi
	uint16_t									number;			//numero di byte del solo campo "data"
	answer_parameters_t					param;
	generic_answers_t						data;
} PACKED api_answer_t;

#endif /* sw_share_h */
