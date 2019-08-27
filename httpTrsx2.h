/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   httpTrsx2.h
 * Author: jcaf
 *
 * Created on August 21, 2019, 12:09 PM
 */

#ifndef HTTPTRSX2_H
#define HTTPTRSX2_H

#include "system.h"
#include "types.h"

#define HTTPTRSX_DEBUG

typedef enum _http_trx_set_exec_mode_e
{
        EM_WAIT_NEW_EXEC_MODE_E = 0,
	EM_RUN_INTERVAL_E,
	EM_RUN_ONCE_E,
        EM_STOP_E,
}
HTTP_TRSX_SET_EXEC_MODE_E;

typedef struct HTTP_TRSX_SET_EXEC_MODE_T
{
    HTTP_TRSX_SET_EXEC_MODE_E k;
}HTTP_TRSX_SET_EXEC_MODE;

#define EM_WAIT_NEW_EXEC_MODE   ((HTTP_TRSX_SET_EXEC_MODE){EM_WAIT_NEW_EXEC_MODE_E})
#define EM_STOP                 ((HTTP_TRSX_SET_EXEC_MODE){EM_STOP_E})
#define EM_RUN_ONCE             ((HTTP_TRSX_SET_EXEC_MODE){EM_RUN_ONCE_E})
#define EM_RUN_INTERVAL         ((HTTP_TRSX_SET_EXEC_MODE){EM_RUN_INTERVAL_E})

enum _http_trx_set_status
{
	IDLE,
	RUNNING
};

typedef struct _TRSX
{
    /*Server*/
    char *domain;
    uint8_t *IP;
    uint16_t port;
    
    /*connection resource*/
    char *URI;
    char *Host;
    char *ApiKey;
    char *HdrLine;
    
//    struct _TRSXWRrqst//request message
//    {
//    }rsqtMsg;
    
    struct _TRSXWRresp//response message
    {
        int8_t sm0;
        int8_t sm1;
        struct _TRSXWRresp_tmr
        {
            unsigned long responseMsg_timeout;
            unsigned long afterServerDisconneted_flushbuffer;
        }timer;
        
        uint32_t idx;
    }respMsg;
    
    #if defined(__AVR__) && defined(__GNUC__)  
        Client *client;
    #elif
    #endif
    
    #ifdef HTTPTRSX_DEBUG
    struct _HTTPTRSX_Debug
    {
       struct _HTTPTRSX_Debug_bf
       {
           unsigned enabled:1;
           unsigned __a:7;
       }bf;
    }dbg;
    #endif

    struct _TRSX_exec
    {
        HTTP_TRSX_SET_EXEC_MODE_E execMode;
        int8_t status;
        unsigned long execInterval_ms;//exec with interval
        //
        
        HTTP_TRSX_SET_EXEC_MODE_E last_execMode;
        int8_t runInterval_sm0=0;
        unsigned long tmr_runInterval;
    }exec;

}TRSX;

typedef struct _HTTPTRSXDEBUG
{
   #ifdef HTTPTRSX_DEBUG
        #if defined(__AVR__) && defined(__GNUC__)  
            PTRFX_retVOID_arg1_PCHAR_arg2_INT8_T UART_print;
        #elif
            PTRFX_retVOID_arg1_PCHAR UART_print;
        #endif
    #endif    //HTTPTRSX_DEBUG;
}HTTPTRSXDEBUG;

extern HTTPTRSXDEBUG httpTrsxDebug;

typedef struct _JSON
{
    char *name;
    char *strval;
}JSON;

int8_t NIC_begin(uint8_t *MAC, uint8_t *IPstatic);
void NIC_getMyIP(char *str, uint16_t sizebuff);

#if defined(__AVR__) && defined(__GNUC__)
    void httpTrsx_setClient(TRSX *trsx, Client* client);
#elif    
#endif

#ifdef HTTPTRSX_DEBUG
    void httpTrsx_UARTdebug_enabled(TRSX *trsx, BOOLEAN_T _bool);
    void httpTrsx_UARTdebug_print(TRSX *trsx, char *str, int8_t mode);
    

    #if defined(__AVR__) && defined(__GNUC__)
    void httpTrsx_UARTdebug_setPrintFx(PTRFX_retVOID_arg1_PCHAR_arg2_INT8_T UART_print);
    #elif
    void httpTrsx_UARTdebug_setPrintFx(PTRFX_retVOID_arg1_PCHAR UART_print);
    #endif
#endif

void httpTrsx_setupServerByDomain(TRSX *trsx, char *domain, uint16_t port);
void httpTrsx_setupServerByIP(TRSX *trsx, uint8_t *IP, uint16_t port);
//
void httpTrsx_setHost(TRSX *trsx, char *host);
void httpTrsx_setURI(TRSX *trsx, char *URI);
void httpTrsx_setApiKey(TRSX *trsx, char *ApiKey);
void httpTrsx_setHdrLine(TRSX *trsx, char *HdrLine);

/******************************************************************************************/

#define HTTP_TRSX_RX_BUFFER_MAX_SIZE 200

int8_t httpTrsx_getStatus(TRSX *trsx);
void httpTrsx_setExecInterval_ms(TRSX *trsx, unsigned long interval_ms);
void httpTrsx_setExecMode(TRSX *trsx, HTTP_TRSX_SET_EXEC_MODE execMode);

int8_t httpTrsx_job(TRSX *trsx, JSON *json, uint8_t npairs, char *outmsg);

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

#endif /* HTTPTRSX2_H */


