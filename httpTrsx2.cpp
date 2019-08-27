
#if defined(__AVR__) && defined(__GNUC__)
    #include <Arduino.h>
    #include <Ethernet.h>

    #define __millis() millis()
#elif
#endif

#include "httpTrsx2.h"
#include <string.h>
#include <stdint.h>
extern EthernetClient client;
/******************************************************************************************/
static HTTPTRSX httpTrsx;    
/******************************************************************************************/
#if defined(__AVR__) && defined(__GNUC__)
    void httpTrsx_setClient(TRSXWR *trsxw, Client* client)
    {
        trsxw->client = client;
    }
    #define httpTrsxWrite_setClient(client) do{httpTrsx_setClient(&httpTrsx.trsxw, client);}while(0)
    #define httpTrsxRead_setClient(client) do{httpTrsx_setClient(&httpTrsx.trxr, client);}while(0)
#elif
#endif

#ifdef HTTPTRSX_DEBUG
void httpTrsx_UARTdebug_enabled(TRSXWR *trsxw, BOOLEAN_T _bool)
{
    trsxw->dbg.bf.enabled = _bool.k;
    //httpTrsx.dbg.UART_print = UART_print;
}
void httpTrsx_UARTdebug_print(TRSXWR *trsxw, char *str, int8_t mode)
{
    if (trsxw->dbg.bf.enabled)
        {httpTrsx.UART_print(str, mode);}
}
#if defined(__AVR__) && defined(__GNUC__)
void httpTrsx_UARTdebug_setPrintFx(PTRFX_retVOID_arg1_PCHAR_arg2_INT8_T UART_print)
{
    httpTrsx.UART_print = UART_print;
}
#elif
void httpTrsx_UARTdebug_setPrintFx(PTRFX_retVOID_arg1_PCHAR UART_print)
{
    httpTrsx.UART_print = UART_print;
}
#endif
#endif
/* 1 = configured
 * 0 = not configured */
//int8_t NIC_begin(uint8_t *MAC, ENABLED_T DHCPenabled , uint8_t *IPstatic)
int8_t NIC_begin(uint8_t *MAC, uint8_t *IPstatic)
{
    int8_t cod_ret=0;
    
    //NIC.MAC = MAC;
    //NIC_setDHCP(DHCPenabled, IPstatic);
    
    #if defined(__AVR__) && defined(__GNUC__)
    IPAddress IP(IPstatic);
    if (1)//(DHCPenabled)
    {
        //Only begin(mac) return a value, others overloaded fxs return void
        if (Ethernet.begin(MAC) == 0)
        {
            Ethernet.begin(MAC, IP);//"Static IP was assigned\n"
            //NIC.bf.DHCP = 0;
        }
        else//"IP configured by DHCP\n"
        {
            //httpTrsxWrite_UARTdebug_print("IP configured by DHCP\n");
        }
    }
    else
    {
        Ethernet.begin(MAC, IP);//others overloaded fxs return void
        //NIC.bf.DHCP = 0;
    }
    cod_ret = 1;
    
    #endif
    return cod_ret;
}
void NIC_getMyIP(char *str, uint16_t sizebuff)
{
    snprintf(str, sizebuff, "%d.%d.%d.%d", Ethernet.localIP()[0], Ethernet.localIP()[1], Ethernet.localIP()[2], Ethernet.localIP()[3]);
}
/******************************************************************************************/
#if defined(__AVR__) && defined(__GNUC__)
    #define FS_STRING
    #ifdef FS_STRING
      #define FS(s) PSTR(s)
    #else
      #define FS(s) s
    #endif
#else
  #define FS(s) s
  //#define Serial.print(s) do{printf("%s", s);}while(0)
#endif
/******************************************************************************************/
int16_t tcpClient_getBytesAvailable(TRSXWR *trsxw)
{
    #if defined(__AVR__) && defined(__GNUC__)
    
    return trsxw->client->available();
    #elif
    #endif
}
int8_t tcpClient_connected(TRSXWR *trsxw)
{
    #if defined(__AVR__) && defined(__GNUC__)
    return trsxw->client->connected();
    #elif
    #endif
}
void tcpClient_stop(TRSXWR *trsxw)
{
    #if defined(__AVR__) && defined(__GNUC__)
    trsxw->client->stop();
    #elif
    #endif
}
char httpClient_readChar(TRSXWR *trsxw) 
{
    #if defined(__AVR__) && defined(__GNUC__)
    return trsxw->client->read();
    #elif
    #endif
}
/******************************************************************************************/
/*Print standard (RAM)*/
void http_print(TRSXWR *trsxw, const char *s)
{
    #if defined(__AVR__) && defined(__GNUC__)
          trsxw->client->print(s);
          httpTrsx_UARTdebug_print(trsxw, s, 0);
    #elif
    #endif
}
/******************************************************************************************/
void http_printk(TRSXWR *trsxw, char *s)
{
    #ifdef FS_STRING
        trsxw->client->print(reinterpret_cast <const __FlashStringHelper *> (s) );
        httpTrsx_UARTdebug_print(trsxw, s, 1);
    #else
      http_print(trsxw, s);
    #endif
}
/******************************************************************************************/
uint32_t json_getContentLength(JSON *json, uint8_t npairs)
{
    uint32_t acc=0;
    int i;
    for (i=0; i<npairs; i++)
    {
        acc += strlen(json[i].name)+strlen(json[i].strval) + 3;//+2"" + 1:
        if (i< (npairs-1))
            {acc++;}  //comma
    }
    return acc+2;//+2 curl-braces
}
/******************************************************************************************/
void uint32toa(uint32_t ui32, char *buff, uint32_t sizebuff)
{
    snprintf(buff, sizebuff, "%lu", ui32);
}
/******************************************************************************************/
void http_send_msgbody(TRSXWR *trsxw, JSON *json, uint8_t npairs)
{
    int i;
    http_printk(trsxw, FS("{"));
    for (i=0; i<npairs; i++)
    {
        http_printk(trsxw, FS("\""));
        http_print(trsxw, json[i].name);
        http_printk(trsxw, FS("\":"));
        http_print(trsxw, json[i].strval);
        
        if (i< (npairs-1))
            {http_printk(trsxw, FS(","));}
    }
    http_printk(trsxw, FS("}"));
}
/******************************************************************************************/
void httpTrsx_setHost(TRSXWR *trsxw, char *host)
{
    trsxw->Host = host;
}
void httpTrsx_setURI(TRSXWR *trsxw, char *URI)
{
    trsxw->URI = URI;
}
void httpTrsx_setApiKey(TRSXWR *trsxw, char *ApiKey)    
{
    trsxw->ApiKey = ApiKey;
}
void httpTrsx_setHdrLine(TRSXWR *trsxw, char *HdrLine)
{
    trsxw->HdrLine = HdrLine;
}
/******************************************************************************************/
int8_t httpTrsx_requestMsg(TRSXWR *trsxw, JSON *json, uint8_t npairs)//send the request message to HTTP server
{
    int8_t cod_ret = 0;
    char buff[20];
    /*1) Request Line*/
    http_printk(trsxw, FS("POST "));http_print(trsxw, trsxw->URI);http_printk(trsxw, FS(" HTTP/1.1\r\n"));
    
    /*2) Header lines*/
    http_printk(trsxw, FS("Host: "));http_print(trsxw, trsxw->Host);http_printk(trsxw, FS("\r\n"));
    //http_printk(trsxw, FS("Connection: keep-alive\r\n"));//HTTP persistent connection
    http_printk(trsxw, FS("Connection: close\r\n"));   
    http_printk(trsxw, FS("Content-Type: application/json\r\n"));
    http_printk(trsxw, FS("api_key_write: "));http_print(trsxw, trsxw->ApiKey);http_printk(trsxw, FS("\r\n"));
    http_printk(trsxw, FS("User-Agent: Agent/1.00\r\n"));
    if (trsxw->HdrLine != NULL)
        {http_print(trsxw, trsxw->HdrLine);}
    http_printk(trsxw, FS("Content-Length: "));
    uint32toa(json_getContentLength(json, npairs) ,  buff, sizeof(buff)/sizeof(buff[0]));
    http_print(trsxw, buff);http_printk(trsxw, FS("\r\n"));
    
    /*3) Send New Line*/
    http_printk(trsxw, FS("\r\n"));
    /*4) Send msg body*/
    http_send_msgbody(trsxw, json, npairs);http_printk(trsxw, FS("\r\n"));
    
    cod_ret = 1;
    return cod_ret;
}
/******************************************************************************************
        HTTP transaction: response message

I’ve had a TCPClient disconnect but still have the connected property == true. 
So it seams the only way to tell is if a Read from the network stream returns 0 bytes. 
So right now I’m assuming it is disconnected if it returns 0 bytes. I was wondering if there is a better way?
 
Here is documented REMARKS section for tcpClient.Connected property:
"The Connected property gets the connection state of the Client socket as of the last I/O
operation. When it returns false, the Client socket was either never connected, or is no
longer connected."
"Because the Connected property only reflects the state of the connection as of the most
recent operation, you should attempt to send or receive a message to determine the current
state. After the message send fails, this property no longer returns true. Note that this
behavior is by design. You cannot reliably test the state of the connection because, in the
time between the test and a send/receive, the connection could have been lost. Your code
should assume the socket is connected, and gracefully handle failed transmissions."
So, I thing yours 'Read' way is the best one and you may forget Connected property
alltogether!

KTIMEOUT_READBUFFER: Is the k-time of CPU (and resources) assigned to read data from the buffer if available.
KTIMEOUT_AFTERSERVERDISCONNECTED_FLUSHBUFFER: If the Server disconnect the client, client has a k-time to read all possible available data from rx buffer. After this time,the connection must be end sending client.stop()
KTIMEOUT_RESPONSEMSG_TOTALTIMEOUT: Is the global k-timeout assigned for all recepcion in an HTTP Transfer response-mode.
    return:
    0: Busy
    1: End response msg
*******************************************************************************************/
//const unsigned long KTIMEOUT_READBUFFER = 10;//ms
//unsigned long KTIMEOUT_AFTERSERVERDISCONNECTED_FLUSHBUFFER= 700;//ms
//unsigned long KTIMEOUT_RESPONSEMSG_TOTALTIMEOUT = 2000;//ms
//struct _httpTrsx_responseMsg_ktimeOut
//{
//}ktimeOut;
#define KTIMEOUT_READBUFFER                             10UL//ms
#define KTIMEOUT_AFTERSERVERDISCONNECTED_FLUSHBUFFER    0UL//ms
#define KTIMEOUT_RESPONSEMSG_TOTALTIMEOUT               6000UL//ms

#if KTIMEOUT_AFTERSERVERDISCONNECTED_FLUSHBUFFER > KTIMEOUT_RESPONSEMSG_TOTALTIMEOUT
    #undef KTIMEOUT_AFTERSERVERDISCONNECTED_FLUSHBUFFER
    #define KTIMEOUT_AFTERSERVERDISCONNECTED_FLUSHBUFFER KTIMEOUT_RESPONSEMSG_TOTALTIMEOUT
#endif // KTIMEOUT_AFTERSERVERDISCONNECTED_FLUSHBUFFER

int8_t httpTrsx_responseMsg(TRSXWR *trsxw, char *outmsg)
{
    unsigned long tmr_readbuffer;
    char c;
    
    int8_t cod_ret = 0;
    
    if (trsxw->respMsg.sm0 == 0)
    {
        trsxw->respMsg.timer.responseMsg_timeout = __millis();
        trsxw->respMsg.idx = 0;
        trsxw->respMsg.sm1 = 0;
        trsxw->respMsg.sm0++;
    }
    if (trsxw->respMsg.sm0 == 1)
    {
        tmr_readbuffer = __millis();
        do
        {
            if (tcpClient_getBytesAvailable(trsxw) > 0)//buffer>0
            {
                c = httpClient_readChar(trsxw);
                //httpTrsx_UARTdebug_print(trsxw, c);
                Serial.write(c);
                    
                if (outmsg!= NULL)
                {
                    outmsg[trsxw->respMsg.idx] = c;
                    if (++trsxw->respMsg.idx >= HTTP_TRSX_RX_BUFFER_MAX_SIZE)
                    {
                        trsxw->respMsg.idx = 0;//as circular buffer
                    }
                }
                //if (c == 'LAST_CHAR_BREAKING')
                //	break;
            }
            else
            {
                break;
            }
        }
        while ((__millis() - tmr_readbuffer) <= KTIMEOUT_READBUFFER);  //CPU assigned for window

        if (trsxw->respMsg.sm1 == 0)
        {
            if (!tcpClient_connected(trsxw))
            {
                trsxw->respMsg.timer.afterServerDisconneted_flushbuffer = __millis();
                trsxw->respMsg.sm1++;
            }
        }
        else
        {
            if ( (__millis() - trsxw->respMsg.timer.afterServerDisconneted_flushbuffer) >= KTIMEOUT_AFTERSERVERDISCONNECTED_FLUSHBUFFER ) //let a time to read all rx buffer
            {
                tcpClient_stop(trsxw);
                trsxw->respMsg.sm0 = 0;
                cod_ret = 1;
            }
        }

        //connection time-out
        if ( (__millis() - trsxw->respMsg.timer.responseMsg_timeout) >= KTIMEOUT_RESPONSEMSG_TOTALTIMEOUT) //abort and stop conection
        {
            tcpClient_stop(trsxw);
            trsxw->respMsg.sm0 = 0;
            cod_ret = 1;
        }
    }
    return cod_ret;
}

/*
1 HTTP transaction

Like most network protocols, HTTP uses the client-server model: An HTTP
client opens a connection and sends a request message to an HTTP server;
the server then returns a response message, usually containing the resource
that was requested. After delivering the response, the server closes the
connection (making HTTP a stateless protocol, i.e. not maintaining any
connection information between transactions).

    return:
    0: Busy in HTTP transaction
    1: End one HTTP transaction, even if client could not connect to the server.
*/

//#define SOCKET_DEBUG
#ifdef SOCKET_DEBUG
#include "utility/w5100.h"
#include "utility/socket.h"
void ShowSocketStatus(void);
#endif

void httpTrsx_setupServerByDomain(TRSXWR *trsxw, char *domain, uint16_t port)
{
    trsxw->domain = domain;
    trsxw->port = port;
}
void httpTrsx_setupServerByIP(TRSXWR *trsxw, uint8_t *IP, uint16_t port)
{
    trsxw->IP = IP;
    trsxw->port = port;
}
int8_t tcpClient_connection(TRSXWR *trsxw)
{
    int8_t cod_ret;
    
    #if defined(__AVR__) && defined(__GNUC__)
    if (trsxw->domain != NULL)
        cod_ret = trsxw->client->connect(trsxw->domain, trsxw->port);
    else
        cod_ret = trsxw->client->connect(trsxw->IP, trsxw->port);
    #elif
    #endif
    return cod_ret;
}

int8_t httpTrsx_do1trsx(TRSXWR *trsxw, JSON *json, uint8_t npairs, char *outmsg)
{
    static int8_t sm0;
    int8_t code_ret = 0;

    if (sm0 == 0)// client opens a connection
    {
        //httpTrsx.UART_print(FS("\nNueva transaction\n"), 1);
        
        if (tcpClient_connection(trsxw))
        {
            sm0++;
        }
        else
        {
            #ifdef SOCKET_DEBUG
            Serial.println(F("Socket status in error before client.stop():"));
            ShowSocketStatus();
            Serial.println(F("Socket status in error after client.stop()"));
            #endif

            tcpClient_stop(trsxw);
            code_ret = 1;
        }
    }
    if (sm0 == 1)//client->server: send request message
    {
        if (httpTrsx_requestMsg(trsxw, json, npairs))
        {
            sm0++;
        }
    }
    if (sm0 == 2)//server->client: receive response message
    {
        if (httpTrsx_responseMsg(trsxw, outmsg))
        {
            sm0 = 0x00;
            code_ret = 1;
        }
    }
    
    #ifdef SOCKET_DEBUG
    ShowSocketStatus();
    #endif
    
    return code_ret;
}

/*******************************************************************************************
HTTP transaction manager:
Exec.mode:

WAIT_NEW_EXEC_MODE = -1,
STOP = 0,
RUN_ONCE,
RUN_INTERVAL,

Status:

IDLE,
RUNNING
********************************************************************************************/
static struct _httpTrsxExec
{
    HTTP_TRSX_SET_EXEC_MODE_E execMode;
    int8_t status;
    unsigned long exec_interval_ms;//exec with interval
    //char *rx_buffer;
}httpTrsxExec;// = 
//{
//    EM_WAIT_NEW_EXEC_MODE_E,//-1
//    IDLE,//0
//    0,
//    NULL
//};

int8_t httpTrsx_getStatus(void)
{
    return httpTrsxExec.status;
}
void httpTrsx_setExecInterval_ms(unsigned long interval_ms)
{
    httpTrsxExec.exec_interval_ms = interval_ms;
}

void httpTrsx_setExecMode(HTTP_TRSX_SET_EXEC_MODE execMode)
{
    httpTrsxExec.execMode = execMode.k;
}
//void httpTrsx_setStatus(int8_t status)
//{
//    httpTrsx_t.status = status;
//}
//unsigned long httpTrsx_getExec_interval_ms(void)
//{
//    return httpTrsx_t.exec_interval_ms;
//}
//HTTP_TRSX_SET_EXEC_MODE_E httpTrsx_getExecMode(void)
//{
//    return httpTrsx_t.execMode;
//}
//void httpTrsx_setRxBuffer(char *rx_buffer)
//{
//    httpTrsx_t.rx_buffer = rx_buffer;
//}
//char *httpTrsx_getRxBuffer(void)
//{
//    return httpTrsx_t.rx_buffer;
//}
/*
return:
0: Busy in HTTP job (synchronize RUN_ONCE, RUN_INTERVAL, STOP)
1: End one HTTP job (end transaction): Is the time for parsing the http_trx_rx_buffer[]
*/
//int8_t http_trx_job(JSON *json, uint8_t npairs)
int8_t httpTrsx_job(TRSXWR *trsxwr, JSON *json, uint8_t npairs, char *outmsg)
{
    static HTTP_TRSX_SET_EXEC_MODE_E last_exec_mode;
    static int8_t runInterval_sm0=0;
    static unsigned long tmr_run_interval;
    int8_t cod_ret = 0;
    
    if (httpTrsxExec.status == IDLE)
    {
        last_exec_mode = httpTrsxExec.execMode;

        if (last_exec_mode > EM_WAIT_NEW_EXEC_MODE_E)
        {
            if (last_exec_mode == EM_RUN_INTERVAL_E)
            {
                if (runInterval_sm0 == 0)
                {
                    httpTrsxExec.status = RUNNING;
                    runInterval_sm0++;	
                    /*execute 1 HTTP transaction, then comeback to here to continue with runInterval_sm0=1*/
                }
                else if (runInterval_sm0 == 1)//for next evaluation
                {
                    tmr_run_interval = __millis();
                    runInterval_sm0++;
                }
                else if (runInterval_sm0 == 2)
                {
                    if ( (__millis()-tmr_run_interval) >= httpTrsxExec.exec_interval_ms)
                    {
                        runInterval_sm0 = 0x00;
                    }
                }
            }
            else if (last_exec_mode == EM_RUN_ONCE_E)
            {
                httpTrsxExec.status = RUNNING;
            }
            else//STOP
            {
                httpTrsx_setExecMode(EM_WAIT_NEW_EXEC_MODE);
                runInterval_sm0 = 0x00;	//reset
            }
        }
    }
    else
    {
        if ( httpTrsx_do1trsx(trsxwr, json, npairs, outmsg) == 1)//end?
        {
            httpTrsxExec.status = IDLE;

            if (last_exec_mode == EM_RUN_ONCE_E)
                {httpTrsx_setExecMode(EM_STOP);}
            
            cod_ret = 1;//1 transaction was completed
        }
    }
    return cod_ret;
}

//////////////////////////////////////////////////////////////////////////////////
#ifdef SOCKET_DEBUG
void ShowSocketStatus(void)
{

    //    ActivityWriteSPICSC("ETHERNET SOCKET LIST");
    //    ActivityWriteSPICSC("#:Status Port Destination DPort");
    //    ActivityWriteSPICSC("0=avail,14=waiting,17=connected,22=UDP");
    //    ActivityWriteSPICSC("1C=close wait");
    String l_line = "";
    l_line.reserve(64);
    char l_buffer[10] = "";

    for (uint8_t i = 0; i < MAX_SOCK_NUM; i++)
    {
        l_line = "#" + String(i);

        uint8_t s = W5100.readSnSR(i); //status
        l_line += ":0x";
        sprintf(l_buffer,"%x",s);
        l_line += l_buffer;
        //
        // if(s == 0x1C)
        //{close(i);}
        //
        l_line += " ";
        l_line += String(W5100.readSnPORT(i)); //port

        l_line += " D:";
        uint8_t dip[4];
        W5100.readSnDIPR(i, dip); //IP Address
        for (int j=0; j<4; j++)
        {
            l_line += int(dip[j]);
            if (j<3) l_line += ".";
        }
        l_line += " (";
        l_line += String(W5100.readSnDPORT(i)); //port on destination
        l_line += ") ";

        //        if (G_SocketConnectionTimes[i] != 0)
        //            l_line += TimeToHHMM(G_SocketConnectionTimes[i]);

        Serial.println(l_line);

        //ActivityWriteSPICSC(l_line);
    }
}
#endif