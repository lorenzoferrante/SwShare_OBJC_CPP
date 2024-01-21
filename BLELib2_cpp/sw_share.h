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

typedef uint16_t tempo_tw;

//-----------------------------------------------------------------------------
//attributi timers
#define SCADUTO (0)
#define FERMO (0xFF)
#define FERMO_W (0xFFFF)
#define FERMO_32 (0xffffffff UL) //timer a 32 bit fermo

//-----------------------------------------------------------------------------
//definizioni generiche
#define MAX_NUM_READ_WRITE 								(128)
#define MAX_MTU 												(128)
#define N_QUEUE_HD_LOGGER									(5)

//-----------------------------------------------------------------------------
// struttura calendario
#define SPD														(86400)	// Secondi in un giorno.
#define MAGIC_WDAY											(6)		// il 1 gennaio 2000 era un sabato.
#define MAGIC_YEAR											(2000)
#define MAGIC_PREF											(2000)
#define MAGIC_SUFF											(0)

//-----------------------------------------------------------------------------
typedef uint8_t return_type_aes;
typedef uint8_t length_type_aes;

#define ENC_KEY_LENGTH										(16)
#define N_ROW_AES												(4)
#define N_COL_AES												(4)
#define N_BLOCK_AES											(ENC_KEY_LENGTH)
#define N_MAX_ROUNDS_AES									(14)
#define N_PARAM_HIDDEN_LOGGER								(4)
#define N_HIDDEN_LOGGER										(200)
#define BASE_ADDR_READ_PARAMETERS						(0xfff00000)
#define BASE_ADDR_READ_LOGGER								(0xfff10000)

//-----------------------------------------------------------------------------
typedef enum
{
	HL_EV_AVVIO_DISPOSITIVO,
	HL_EV_IMPOSTATO_PARAMETRO,
	HL_EV_IMPOSTATA_DATA_ORA,
	HL_EV_ANTI_ALLAGAMENTO,
	N_HL_EVT_TYPE,
	HL_EV_CANCELLA_LOGGER = 0xff,
} PACKED T_HL_EVENTS;

//-----------------------------------------------------------------------------
typedef enum
{
	AES_ENC,
	AES_DEC
}
aes_mode_t;

//-----------------------------------------------------------------------------
typedef struct
{
	uint8_t ksch[(N_MAX_ROUNDS_AES + 1) * N_BLOCK_AES];
	uint8_t rnd;
}
aes_context;

//-----------------------------------------------------------------------------
typedef struct t_param
{
	uint32_t minimo;
	uint32_t fabbrica;
	uint32_t massimo;
} t_param;

//-----------------------------------------------------------------------------
typedef struct {
	uint8_t sec;				// Secondi [0-59]
	uint8_t min;				// Minuti  [0-59]
	uint8_t hour;				// Ore     [0-23]
	uint8_t day;				// Giorni  [1-31]
	uint8_t mon;				// Mese    [0-11]
	uint8_t year;				// Anno    [0-135]
	uint8_t wday;				// giorno  [0-6] 0=Dom 1=Lun]
	uint8_t mode;				// MODE_ITA, MODE_USA, MODE_ISO
} t_calendar;

//-----------------------------------------------------------------------------
typedef struct
{
	uint16_t		evento;
	uint32_t		date_time;
	uint32_t		parametri[N_PARAM_HIDDEN_LOGGER];
} PACKED t_hd_log_element;

//-----------------------------------------------------------------------------
typedef struct
{
	t_hd_log_element	logger[N_HIDDEN_LOGGER];
	uint16_t				index;
	uint8_t				ricircolo;
	uint8_t				free;
} PACKED t_hidden_logger;

//-----------------------------------------------------------------------------
typedef enum
{
	COM_CHANNEL_BLE,
	COM_CHANNEL_SERIAL,
	N_COM_CHANNEL,
} PACKED COM_CHANNELS_T;

//-----------------------------------------------------------------------------
typedef enum
{
	COMMAND_NULL,
	COMMAND_RAW_READ,
	COMMAND_RAW_WRITE,
	COMMAND_GET_SET_PARAM,
	COMMAND_GREETING_MESSAGE,
	COMMAND_GET_SET_TIME,
	COMMAND_DEVICES_ONBOARD,
} PACKED API_COMMAND_T;

//-----------------------------------------------------------------------------
typedef enum
{
	PAR_RUB_SENSITIVITY,
	PAR_SCA_SENSITIVITY,
	PAR_RUB_RITARDO_CHIUSURA,
	PAR_RUB_INTERVALLO_CICLO,
	PAR_RUB_TEMPO_CICLO,
	PAR_SCA_RAGGIO_PULSANTE,
	PAR_SCA_ATTESA_SCARICO,
	PAR_SCA_MIN_ATTESA_SCARICO,
	PAR_SCA_FLUSH_TIME_SHORT,
	PAR_SCA_FLUSH_TIME_LONG,
	PAR_SCA_INTERVALLO_CICLO,
	PAR_SCA_TEMPO_CICLO,
	PAR_SCA_TEMPO_STAZION_MIN,
	PAR_SCA_TEMPO_FILTRO_STAZION,
	PAR_SCA_TEMPO_SOGLIA_STAZION,
	PAR_SCA_TEMPO_SCARICO_VELO,
	PAR_SCA_TEMPO_INTERVALLO_VELO,
	PAR_RUB_TEMPO_ANTIALLAGAMENTO,

	PAR_AVAILABLE_1,
	PAR_AVAILABLE_2,
	PAR_AVAILABLE_3,
	PAR_AVAILABLE_4,
	PAR_AVAILABLE_5,
	PAR_AVAILABLE_6,
	PAR_AVAILABLE_7,
	PAR_AVAILABLE_8,
	PAR_AVAILABLE_9,
	PAR_AVAILABLE_10,
	PAR_AVAILABLE_11,
	PAR_AVAILABLE_12,
	PAR_AVAILABLE_13,
	PAR_AVAILABLE_14,
	PAR_AVAILABLE_15,
	PAR_AVAILABLE_16,
	PAR_AVAILABLE_17,
	PAR_AVAILABLE_18,
	PAR_AVAILABLE_19,
	PAR_AVAILABLE_20,
	N_PARAMETERS,
} PACKED PARAMETERS_T;

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
typedef struct
{
	uint8_t									device;			//0=led, 1=elettrovalvola, 0xff=cancellazione logger
	uint8_t									attivazione;	//0=lampeggiante, 1=fissa, 0xfe=fine attivazione device 0xff fine di tutte le attivazioni
	uint8_t									spare[62];
} PACKED devices_onboard_query_t;

//-----------------------------------------------------------------------------
typedef union
{
	uint8_t									max_packet[MAX_PACKET_SIZE];
	raw_read_query_t						raw_read;
	raw_write_query_t						raw_write;
	get_set_param_query_t				get_set_param;
	greeting_message_query_t			greeting_message;
	get_set_time_query_t					get_set_time;
	devices_onboard_query_t				devices_onboard;
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
	uint32_t									time;						//in caso di get riporta i secondi trascorsi dal primo gennaio 2000
	uint32_t									cnt_attivazioni;		//in caso di get riporta il numero di attivazioni
	uint8_t									spare[56];
} PACKED get_set_time_answer_t;

//-----------------------------------------------------------------------------
typedef struct
{
	uint8_t									spare[64];
} PACKED devices_onboard_answer_t;

//-----------------------------------------------------------------------------
typedef union
{
	uint8_t									max_packet[MAX_PACKET_SIZE];
	raw_read_answer_t						raw_read;
	raw_write_answer_t					raw_write;
	get_set_param_answer_t				get_set_param;
	greeting_message_answer_t			greeting_message;
	get_set_time_answer_t				get_set_time;
	devices_onboard_answer_t			devices_onboard;
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
