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

typedef struct _TRSXWR
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
       //PTRFX_retVOID_arg1_PCHAR UART_print;
    }dbg;
    #endif

}TRSXWR;

typedef struct _HTTPTRSX
{
    TRSXWR trsxw;
    TRSXWR trxr;
    //
    #ifdef HTTPTRSX_DEBUG
        #if defined(__AVR__) && defined(__GNUC__)  
            PTRFX_retVOID_arg1_PCHAR_arg2_INT8_T UART_print;
        #elif
            PTRFX_retVOID_arg1_PCHAR UART_print;
        #endif
    #endif        
}HTTPTRSX;

extern  HTTPTRSX httpTrsx;

typedef struct _JSON
{
    char *name;
    char *strval;
}JSON;

int8_t NIC_begin(uint8_t *MAC, uint8_t *IPstatic);
void NIC_getMyIP(char *str, uint16_t sizebuff);

#if defined(__AVR__) && defined(__GNUC__)
    void httpTrsx_setClient(TRSXWR *trsxw, Client* client);
    
    #define httpTrsxWrite_setClient(client) do{httpTrsx_setClient(&httpTrsx.trsxw, client);}while(0)
    #define httpTrsxRead_setClient(client) do{httpTrsx_setClient(&httpTrsx.trxr, client);}while(0)
#endif

#ifdef HTTPTRSX_DEBUG
    void httpTrsx_UARTdebug_enabled(TRSXWR *trsxw, BOOLEAN_T _bool);
    void httpTrsx_UARTdebug_print(TRSXWR *trsxw, char *str, int8_t mode);
    
    #define httpTrsxWrite_UARTdebug_enabled(_bool)   do{httpTrsx_UARTdebug_enabled(&httpTrsx.trsxw, _bool);}while(0)
    //#define httpTrsxWrite_UARTdebug_print(str)       do{httpTrsx_UARTdebug_print(&httpTrsx.trsxw, str);}while(0)

    #define httpTrsxRead_UARTdebug_enabled(_bool)   do{httpTrsx_UARTdebug_enabled(&httpTrsx.trxr, _bool);}while(0)
    //#define httpTrsxRead_UARTdebug_print(str)       do{httpTrsx_UARTdebug_print(&httpTrsx.trxr, str);}while(0)

    #if defined(__AVR__) && defined(__GNUC__)
    void httpTrsx_UARTdebug_setPrintFx(PTRFX_retVOID_arg1_PCHAR_arg2_INT8_T UART_print);
    #elif
    void httpTrsx_UARTdebug_setPrintFx(PTRFX_retVOID_arg1_PCHAR UART_print);
    #endif
#endif

void httpTrsx_setHost(TRSXWR *trsxw, char *host);
void httpTrsx_setURI(TRSXWR *trsxw, char *URI);
void httpTrsx_setApiKey(TRSXWR *trsxw, char *ApiKey);
void httpTrsx_setHdrLine(TRSXWR *trsxw, char *HdrLine);

#define httpTrsxWrite_setHost(host)          do{httpTrsx_setHost(&httpTrsx.trsxw , host);}while(0)
#define httpTrsxWrite_setURI(URI)            do{httpTrsx_setURI(&httpTrsx.trsxw , URI);}while(0)
#define httpTrsxWrite_setApiKey(ApiKey)      do{httpTrsx_setApiKey(&httpTrsx.trsxw , ApiKey);}while(0)
#define httpTrsxWrite_setHdrLine(HdrLine)    do{httpTrsx_setHdrLine(&httpTrsx.trsxw , HdrLine);}while(0)

#define httpTrsxRead_setHost(host)          do{httpTrsx_setHost(&httpTrsx.trxr , host);}while(0)
#define httpTrsxRead_setURI(URI)            do{httpTrsx_setURI(&httpTrsx.trxr , URI);}while(0)
#define httpTrsxRead_setApiKey(ApiKey)      do{httpTrsx_setApiKey(&httpTrsx.trxr , ApiKey);}while(0)
#define httpTrsxRead_setHdrLine(HdrLine)    do{httpTrsx_setHdrLine(&httpTrsx.trxr , HdrLine);}while(0)


void httpTrsx_setupServerByDomain(TRSXWR *trsxw, char *domain, uint16_t port);
void httpTrsx_setupServerByIP(TRSXWR *trsxw, uint8_t *IP, uint16_t port);
#define httpTrsxWrite_setupServerByDomain(domain,port) do{httpTrsx_setupServerByDomain(&httpTrsx.trsxw, domain, port);}while(0)
#define httpTrsxWrite_setupServerByIP(IP,port) do{httpTrsx_setupServerByIP(&httpTrsx.trsxw, IP, port);}while(0)

#define httpTrsxRead_setupServerByDomain(domain,port) do{httpTrsx_setupServerByDomain(&httpTrsx.trxr, domain, port);}while(0)
#define httpTrsxRead_setupServerByIP(IP,port) do{httpTrsx_setupServerByIP(&httpTrsx.trxr, IP, port);}while(0)

/******************************************************************************************/
typedef enum _http_trx_set_exec_mode_e
{
//	EM_WAIT_NEW_EXEC_MODE_E = -1,
//	EM_STOP_E = 0,
//	EM_RUN_ONCE_E,
//	EM_RUN_INTERVAL_E,
                
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

#define HTTP_TRSX_RX_BUFFER_MAX_SIZE 200

int8_t httpTrsx_getStatus(void);
void httpTrsx_setExecInterval_ms(unsigned long interval_ms);
void httpTrsx_setExecMode(HTTP_TRSX_SET_EXEC_MODE execMode);

int8_t httpTrsx_job(TRSXWR *trsxwr, JSON *json, uint8_t npairs, char *outmsg);

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

#endif /* HTTPTRSX2_H */


