//
//  FunShare.cpp
//  BLELib2
//
//  Created by Lorenzo Ferrante on 13/10/23.
//

#include "FunShare.h"
#include "time.h"

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import <BLELib2/AESFunctions.h>

@implementation FunShare
#else

#include "AESFunctions.h"
#endif

#if defined(__OBJC__) && defined(CMP_CRIPTAZIONE)
#define DATA_ENCRYPT(a, b, c, d) [AESFunctions data_encrypt:a and:b and:c and:d];
#elif !defined(__OBJC__) && defined(CMP_CRIPTAZIONE)
#define DATA_ENCRYPT(a, b, c, d) data_encrypt(a, b, c, d)
#else
#define DATA_ENCRYPT(a, b, c, d) b
#endif

uint16_t const crc16_table[256] =
        {
                0x0000,0xc0c1,0xc181,0x0140,0xc301,0x03c0,0x0280,0xc241,
                0xc601,0x06c0,0x0780,0xc741,0x0500,0xc5c1,0xc481,0x0440,
                0xcc01,0x0cc0,0x0d80,0xcd41,0x0f00,0xcfc1,0xce81,0x0e40,
                0x0a00,0xcac1,0xcb81,0x0b40,0xc901,0x09c0,0x0880,0xc841,
                0xd801,0x18c0,0x1980,0xd941,0x1b00,0xdbc1,0xda81,0x1a40,
                0x1e00,0xdec1,0xdf81,0x1f40,0xdd01,0x1dc0,0x1c80,0xdc41,
                0x1400,0xd4c1,0xd581,0x1540,0xd701,0x17c0,0x1680,0xd641,
                0xd201,0x12c0,0x1380,0xd341,0x1100,0xd1c1,0xd081,0x1040,
                0xf001,0x30c0,0x3180,0xf141,0x3300,0xf3c1,0xf281,0x3240,
                0x3600,0xf6c1,0xf781,0x3740,0xf501,0x35c0,0x3480,0xf441,
                0x3c00,0xfcc1,0xfd81,0x3d40,0xff01,0x3fc0,0x3e80,0xfe41,
                0xfa01,0x3ac0,0x3b80,0xfb41,0x3900,0xf9c1,0xf881,0x3840,
                0x2800,0xe8c1,0xe981,0x2940,0xEB01,0x2bc0,0x2a80,0xea41,
                0xee01,0x2ec0,0x2f80,0xef41,0x2d00,0xedc1,0xec81,0x2c40,
                0xe401,0x24c0,0x2580,0xe541,0x2700,0xe7c1,0xe681,0x2640,
                0x2200,0xe2c1,0xe381,0x2340,0xe101,0x21c0,0x2080,0xe041,
                0xa001,0x60c0,0x6180,0xa141,0x6300,0xa3c1,0xa281,0x6240,
                0x6600,0xa6c1,0xa781,0x6740,0xa501,0x65c0,0x6480,0xa441,
                0x6c00,0xacc1,0xad81,0x6d40,0xaf01,0x6fc0,0x6e80,0xae41,
                0xaa01,0x6ac0,0x6b80,0xab41,0x6900,0xa9c1,0xa881,0x6840,
                0x7800,0xb8c1,0xb981,0x7940,0xbb01,0x7bc0,0x7a80,0xba41,
                0xbe01,0x7ec0,0x7f80,0xbf41,0x7d00,0xbdc1,0xbc81,0x7c40,
                0xb401,0x74c0,0x7580,0xb541,0x7700,0xb7c1,0xb681,0x7640,
                0x7200,0xb2c1,0xb381,0x7340,0xb101,0x71c0,0x7080,0xb041,
                0x5000,0x90c1,0x9181,0x5140,0x9301,0x53c0,0x5280,0x9241,
                0x9601,0x56c0,0x5780,0x9741,0x5500,0x95c1,0x9481,0x5440,
                0x9c01,0x5cc0,0x5d80,0x9d41,0x5f00,0x9fc1,0x9e81,0x5e40,
                0x5a00,0x9ac1,0x9b81,0x5b40,0x9901,0x59c0,0x5880,0x9841,
                0x8801,0x48c0,0x4980,0x8941,0x4b00,0x8bc1,0x8a81,0x4a40,
                0x4e00,0x8ec1,0x8f81,0x4f40,0x8d01,0x4dc0,0x4c80,0x8c41,
                0x4400,0x84c1,0x8581,0x4540,0x8701,0x47c0,0x4680,0x8641,
                0x8201,0x42c0,0x4380,0x8341,0x4100,0x81c1,0x8081,0x4040
        };

//-----------------------------------------------------------------------------
uint16_t spm[12] =
        {
                0,
                (31),
                (31+28),
                (31+28+31),
                (31+28+31+30),
                (31+28+31+30+31),
                (31+28+31+30+31+30),
                (31+28+31+30+31+30+31),
                (31+28+31+30+31+30+31+31),
                (31+28+31+30+31+30+31+31+30),
                (31+28+31+30+31+30+31+31+30+31),
                (31+28+31+30+31+30+31+31+30+31+30)
        };

//-----------------------------------------------------------------------------
//Parametri:
//query:			puntatore al vettore di query da riempire (valido solo se q=1 altrimenti puo' essere messo a NULL)
//answer:		puntatore al vettore di answer da processare (valido solo se q=0 altrimenti puo' essere messo a NULL)
//addr_device:	address del dispositivo al quale fare la lettura grezza
//quanti:		se q=1 numero di byte da leggere, se q=0 dimensione del vettore di answer (serve per la cancellazione)
//q: 				1 = preparazione del vettore di query, 0 = analisi del vettore di answer e restituzione dei byte letti sullo stesso vettore di answer
//return:		se q vale 1 la funzione ritorna il numero di byte da inviare
//					se q vale 0 la funzione ritorna il numero di byte letti in caso di risposta andata a buon fine, altrimenti un valore negativo
int32_t prepare_data_for_raw_read (api_query_t *query, api_answer_t *answer, uint32_t addr_device, uint16_t quanti, bool q)
{
	int32_t l;
	if (q)
	{
		memset(query, 0, sizeof(api_query_t));
		query->param.command = COMMAND_RAW_READ;
		query->data.raw_read.address = addr_device;
		query->data.raw_read.byte_number = quanti;
		query->number = sizeof(raw_read_query_t);
		query->param.timestamp = get_my_date();
		query->crc = crc16(query->number + sizeof(query->number) + sizeof(query->param), (uint8_t *)&query->number);
		l = sizeof(api_query_t) - sizeof(generic_queries_t) + sizeof(raw_read_query_t);
		l = DATA_ENCRYPT((uint8_t *)query, l, (uint8_t *)TEMPORARY_MAIN_PWD, AES_ENC);
		return l;
	}
	else
	{
		l = DATA_ENCRYPT((uint8_t *)answer, quanti, (uint8_t *)TEMPORARY_MAIN_PWD, AES_DEC);
		if (answer->param.flags.busy)
			return -1;
		uint16_t loc_crc;
		loc_crc = crc16(answer->number + sizeof(answer->number) + sizeof(answer->param), (uint8_t *)&answer->number);
		if (loc_crc != answer->crc)
			return -2;
		uint16_t num = answer->data.raw_read.number;
		if ((num > MAX_NUM_READ_WRITE) || (num > MAX_MTU))
			return -3;
		uint8_t loc_result[num];
		memcpy(loc_result, answer->data.raw_read.data, num);
		memset(answer, 0, l);
		memcpy(answer, loc_result, num);
		return num;
	}
}

//-----------------------------------------------------------------------------
//Parametri:
//query:			puntatore al vettore di query da riempire (valido solo se q=1 altrimenti puo' essere messo a NULL)
//answer:		puntatore al vettore di answer da processare (valido solo se q=0 altrimenti puo' essere messo a NULL)
//data:			buffer contenente i dati da scrivere, se q vale zero puo' essere posto a NULL
//addr_device:	address del dispositivo al quale fare la scrittura grezza
//quanti:		se q=1 numero di byte da scrivere, se q=0 non significativo
//q: 				1 = preparazione del vettore di query con i dati da scrivere, 0 = analisi del vettore di answer
//return:		se q vale 1 la funzione ritorna il numero di byte da inviare
//					se q vale 0 la funzione ritorna 0 in caso di risposta andata a buon fine, altrimenti un valore negativo
int32_t prepare_data_for_raw_write (api_query_t *query, api_answer_t *answer, uint8_t * data, uint32_t addr_device, uint16_t quanti, bool q)
{
	int32_t l;
	if (q)
	{
		memset(query, 0, sizeof(api_query_t));
		query->param.command = COMMAND_RAW_WRITE;
		query->data.raw_write.address = addr_device;
		query->data.raw_write.byte_number = quanti;
		query->number = sizeof(raw_write_query_t) + quanti;
		query->param.timestamp = get_my_date();
		memcpy (query->data.raw_write.data, data, quanti);
		query->crc = crc16(query->number + sizeof(query->number) + sizeof(query->param), (uint8_t *)&query->number);
		l = sizeof(api_query_t) - sizeof(generic_queries_t) + sizeof(raw_write_query_t) + quanti;
		l = DATA_ENCRYPT((uint8_t *)query, l, (uint8_t *)TEMPORARY_MAIN_PWD, AES_ENC);
		return l;
	}
	else
	{
		(void) DATA_ENCRYPT((uint8_t *)answer, quanti, (uint8_t *)TEMPORARY_MAIN_PWD, AES_DEC);
		if (answer->param.flags.busy)
			return -1;
		uint16_t loc_crc;
		loc_crc = crc16(answer->number + sizeof(answer->number) + sizeof(answer->param), (uint8_t *)&answer->number);
		if (loc_crc != answer->crc)
			return -2;
		return 0;
	}
}

//-----------------------------------------------------------------------------
//Per le chiamae API da c++ bisogna usare quella generica ovvero "prepare_data_for_gen_command"
//Parametri generici:
//query:			puntatore al vettore di query da riempire (valido solo se q=1 altrimenti puo' essere messo a NULL)
//answer:		puntatore al vettore di answer da processare (valido solo se q=0 altrimenti puo' essere messo a NULL)
//q: 				0 = analisi del vettore di answer, 1 = preparazione del vettore di query con i dati da scrivere, 2 = calcolo dati da ricevere
//Parametri specifici:
//param_num:	numero di parametro da leggere/scrivere
//value:			se write=1 e q=1 valore da assegnare al parametro, altrimenti non significativo
//write:			1=scrittura 0=lettura
//return:		se q=0 e write=0 la funzione ritorna il valore del parametro letto in caso di risposta andata a buon fine, altrimenti un valore negativo
//					se q=0 e write=1 la funzione ritorna 0 in caso di risposta andata a buon fine, altrimenti un valore negativo
//					se q=1 la funzione ritorna il numero di byte da inviare
//					se q=2 la funzione ritorna il numero di byte di risposta che il dispositivo deve rispondere dopo la chiamata
int32_t prepare_data_for_get_set_param (api_query_t *query, api_answer_t *answer, uint16_t answ_size, uint16_t param_num, uint32_t value, uint8_t write, uint8_t q)
{
	int32_t l;
	int32_t attesi = sizeof(api_answer_t) - sizeof(generic_answers_t) + sizeof(get_set_param_answer_t);
	(void) answ_size;
#if CMP_CRIPTAZIONE
	attesi += ENC_KEY_LENGTH - (attesi % ENC_KEY_LENGTH);
#endif
	if (q == 1)
	{
		memset(query, 0, sizeof(api_query_t));
		query->param.command = COMMAND_GET_SET_PARAM;
		query->data.get_set_param.param_num = param_num;
		query->data.get_set_param.value = value;
		query->data.get_set_param.write = write;
		l = prepare_data_for_gen_command(query, answer, sizeof(get_set_param_query_t), sizeof(get_set_param_answer_t), COMMAND_GET_SET_PARAM, q);
		return l;
	}
	else if (q == 0)
	{
		l = prepare_data_for_gen_command(query, answer, sizeof(get_set_param_query_t), attesi, COMMAND_GET_SET_PARAM, q);
		if (l < 0)
			return l;
		else
			return answer->data.get_set_param.value;
	}
	else
		return attesi;
}

//-----------------------------------------------------------------------------
//Parametri generici: vedere descrizione parametri generici della funzione "prepare_data_for_get_set_param"
//Parametri specifici:
//device:		0=led, 1=elettrovalvola, 0xff=cancellazione logger
//attivazione:	0=lampeggiante, 1=fissa, 0xfe=fine attivazione device 0xff fine di tutte le attivazioni
//return:		se q=0 la funzione ritorna 0 se il vettore di answer e' composto correttamente, altrimenti un valore negativo
//					se q=1 la funzione ritorna il numero di byte da inviare
//					se q=2 la funzione ritorna il numero di byte che il dispositivo deve rispondere dopo la chiamata
int32_t prepare_data_for_devices_onboard (api_query_t *query, api_answer_t *answer, uint8_t device, uint8_t attivazione, uint8_t q)
{
	int32_t l;
	int32_t attesi = sizeof(api_answer_t) - sizeof(generic_answers_t) + sizeof(devices_onboard_answer_t);
#if CMP_CRIPTAZIONE
	attesi += ENC_KEY_LENGTH - (attesi % ENC_KEY_LENGTH);
#endif
	if (q == 1)
	{
		memset(query, 0, sizeof(api_query_t));
		query->param.command = COMMAND_DEVICES_ONBOARD;
		query->data.devices_onboard.device = device;
		query->data.devices_onboard.attivazione = attivazione;
		l = prepare_data_for_gen_command(query, answer, sizeof(devices_onboard_query_t), sizeof(devices_onboard_answer_t), COMMAND_DEVICES_ONBOARD, q);
		return l;
	}
	else if (q == 0)
	{
		l = prepare_data_for_gen_command(query, answer, sizeof(devices_onboard_query_t), attesi, COMMAND_DEVICES_ONBOARD, q);
			return l;
	}
	else
		return attesi;
}

//-----------------------------------------------------------------------------
//Parametri generici: per i parametri generici vedere "prepare_data_for_get_set_param"
//Parametri specifici: da stabilire, per il momento esistono solo i campo "data_q" e "data_a"
int32_t prepare_data_for_greeting_message (api_query_t *query, api_answer_t *answer, char * data_q, char * data_a, uint8_t q)
{
	int32_t l;
	int32_t attesi = sizeof(api_answer_t) - sizeof(generic_answers_t) + sizeof(greeting_message_answer_t);
#if CMP_CRIPTAZIONE
	attesi += ENC_KEY_LENGTH - (attesi % ENC_KEY_LENGTH);
#endif
	if (q == 1)
	{
		memset(query, 0, sizeof(api_query_t));
		memcpy(query->data.greeting_message.data, data_q, sizeof(query->data.greeting_message.data));
		l = prepare_data_for_gen_command(query, answer, sizeof(greeting_message_query_t), sizeof(greeting_message_answer_t), COMMAND_GET_SET_PARAM, q);
		return l;
	}
	else if (q == 0)
	{
		l = prepare_data_for_gen_command(query, answer, sizeof(greeting_message_query_t), attesi, COMMAND_GET_SET_PARAM, q);
		memcpy(data_a, answer->data.greeting_message.data, sizeof(answer->data.greeting_message.data));
		return l;
	}
	else
		return attesi;
}

//-----------------------------------------------------------------------------
//Funzione interna utilizzabile anche da chiamate C/C++
int32_t prepare_data_for_gen_command(api_query_t *query, api_answer_t *answer, uint16_t size_query, uint16_t size_answer, uint8_t command, uint8_t q)
{
    int32_t l;
	 if (q == 1)
    {
        query->param.command = command;
        query->number = size_query;
        query->param.timestamp = get_my_date();
        query->crc = crc16(query->number + sizeof(query->number) + sizeof(query->param), (uint8_t *)&query->number);
        l = sizeof(api_query_t) - sizeof(generic_queries_t) + size_query;
        l = DATA_ENCRYPT((uint8_t *)query, l, (uint8_t *)TEMPORARY_MAIN_PWD, AES_ENC);
        return l;
    }
	 else if (q == 0)
    {
        (void) DATA_ENCRYPT((uint8_t *)answer, size_answer, (uint8_t *)TEMPORARY_MAIN_PWD, AES_DEC);
        if (answer->param.flags.busy)
            return -1;
        uint16_t loc_crc;
        loc_crc = crc16(answer->number + sizeof(answer->number) + sizeof(answer->param), (uint8_t *)&answer->number);
        if (loc_crc != answer->crc)
            return -2;
        return 0;
    }
	 else
	 {
		 int k = sizeof(api_answer_t) - sizeof(generic_answers_t) + size_answer;
	 #if CMP_CRIPTAZIONE
		 k += ENC_KEY_LENGTH - (k % ENC_KEY_LENGTH);
	 #endif
		 return k;
	 }
}

//-----------------------------------------------------------------------------
//Parametri generici: per i parametri generici vedere "prepare_data_for_get_set_param"
//Parametri specifici:
//time:			in caso di get restituisce la data ed ora letti
//					in caso di set deve contenere la data ed ora da impostare
//cnt_attiv:	in caso di get restituisce il numero di attivazioni dell'elettrovalvola
//					in caso di set non e' significativo (viene restituito zero)
//get_set:		0 = get, 1 = set
//return:		se q=0 la funzione ritorna 0 se il vettore di answer e' composto correttamente, altrimenti un valore negativo
//					se q=1 la funzione ritorna il numero di byte da inviare
//					se q=2 la funzione ritorna il numero di byte che il dispositivo deve rispondere dopo la chiamata
int32_t prepare_data_for_get_set_time (api_query_t *query, api_answer_t *answer, uint32_t * time, uint32_t * cnt_attiv, uint8_t get_set, uint8_t q)
{
	int32_t l;
	int32_t attesi = sizeof(api_answer_t) - sizeof(generic_answers_t) + sizeof(get_set_time_answer_t);
#if CMP_CRIPTAZIONE
	attesi += ENC_KEY_LENGTH - (attesi % ENC_KEY_LENGTH);
#endif
	if (q == 1)
	{
		memset(query, 0, sizeof(api_query_t));
		query->data.get_set_time.time = *time;
		query->data.get_set_time.get_set = get_set;
		l = prepare_data_for_gen_command(query, answer, sizeof(get_set_time_query_t), sizeof(get_set_time_answer_t), COMMAND_GET_SET_TIME, q);
		return l;
	}
	else if (q == 0)
	{
		l = prepare_data_for_gen_command(query, answer, sizeof(get_set_time_query_t), attesi, COMMAND_GET_SET_TIME, q);
		*time = answer->data.get_set_time.time;
		*cnt_attiv = answer->data.get_set_time.cnt_attivazioni;
		return l;
	}
	else
		return attesi;
}

//-----------------------------------------------------------------------------
uint8_t isleap(uint8_t year) {
    return ((!(year%4)) && ((year%100) || !(year%400)));
}

void gettime(uint32_t t, t_calendar *r) {
    uint32_t        i;
    uint32_t        k;
    uint8_t        i2;
    uint32_t        work;

    work = t%(SPD);

    r->sec = work%60;
    work /= 60;
    r->min = work%60;
    r->hour = work/60;
    work = t/(SPD);
    r->wday = (MAGIC_WDAY+work)%7;

    for(i=MAGIC_YEAR;;++i)
    {
        k=isleap((uint8_t)i)?366:365;
        if (work>=k)
            work-=k;
        else
            break;
    }

    r->year = (i-MAGIC_PREF);
    r->day=1;
    if (isleap((uint8_t)i) && (work>58))
    {
        if (work==59)
            r->day=2;
        work-=1;
    }

    for (i2=11; i2 && ((uint32_t)spm[i2]>work); --i2) ;

    r->mon=i2;
    r->day+=work-spm[i2];
}

//-----------------------------------------------------------------------------
uint32_t _mktime(t_calendar *t) {
    uint32_t    day;
    uint32_t    i;

    // Servirebbe ma sempre falsa perche' e' un unsigned char
    // if ( t->year < MAGIC_SUFF )
    //        return (dword) -1;*/
    // day = t->yday = spm [t->mon] + t->day-1 + ( isleap (t->year+MAGIC_PREF)  &  (t->mon > 1) );
    day = spm [(int)t->mon] + t->day-1 + ( isleap ((uint8_t)(t->year+MAGIC_PREF))  &  (t->mon > 1) );

    for ( i = MAGIC_SUFF; i < t->year; i++ )
        day += (365 + isleap ((uint8_t)(i+MAGIC_PREF)));

    // day is now the number of days since 'Jan 1 1970'
    i = 7;
    t->wday = (day + MAGIC_WDAY) % i;  // Sunday=0, Monday=1, ..., Saturday=6

    i = 24;
    day *= i;
    i = 60;
    return ((day + t->hour) * i + t->min) * i + t->sec;
}

//-----------------------------------------------------------------------------
uint16_t crc16(uint32_t len, uint8_t *buf) {
    uint16_t crc = 0;
    uint32_t i;
    for (i=0; i<len; i++)
        crc = ((crc >> 8) & 0xff) ^ crc16_table[(crc ^ *buf++) & 0xff];
    return (crc);
}

//-----------------------------------------------------------------------------
uint32_t get_my_date(void) {
    t_calendar loc_calendar;
    uint32_t loc_time;

    time_t t = time(0);   // get time now
    struct tm* now = localtime(&t);
    loc_calendar.day = (uint8_t)now->tm_mday;
    loc_calendar.mon = (uint8_t)now->tm_mon;
    loc_calendar.year = (uint8_t)(now->tm_year - 100);
    loc_calendar.hour = (uint8_t)now->tm_hour;
    loc_calendar.min = (uint8_t)now->tm_min;
    loc_calendar.sec = (uint8_t)now->tm_sec;
    loc_time = _mktime(&loc_calendar);
    return loc_time;
}

//-----------------------------------------------------------------------------
uint32_t maketime(t_calendar *t)
{
    uint32_t day;
    uint32_t i;

    // Servirebbe ma sempre falsa perche' e' un unsigned char
    // if ( t->year < MAGIC_SUFF )
    //		return (dword) -1;*/
    // day = t->yday = spm [t->mon] + t->day-1 + ( isleap (t->year+MAGIC_PREF)  &  (t->mon > 1) );
    day = spm[(int)t->mon] + t->day - 1 + (isleap((uint8_t)(t->year + MAGIC_PREF)) & (t->mon > 1));

    for (i = MAGIC_SUFF; i < t->year; i++)
        day += (365 + isleap((uint8_t)(i + MAGIC_PREF)));

    // day is now the number of days since 'Jan 1 1970'
    i = 7;
    t->wday = (day + MAGIC_WDAY) % i; // Sunday=0, Monday=1, ..., Saturday=6

    i = 24;
    day *= i;
    i = 60;
    return ((day + t->hour) * i + t->min) * i + t->sec;
}

#if !defined(__SOFTWARE__) && !defined(__IS_APPLICATION__)
char* test_func(void) {
    char str[] = "TEST FUNC";
    return *str;
}
#endif

#ifdef __OBJC__
@end
#endif
