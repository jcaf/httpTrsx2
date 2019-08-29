#if defined(__AVR__) && defined(__GNUC__)
    #include <Arduino.h>
    #include <Ethernet.h>

    #define __millis() millis()
#else
#endif

#include <string.h>
#include <stdint.h>
#include "httpTrsx2.h"

#if defined(__AVR__) && defined(__GNUC__)
    void httpTrsx_setClient(TRSX *trsx, Client* client)
    {
        trsx->client = client;
    }
#else
#endif

#ifdef HTTPTRSX_DEBUG

    static HTTPTRSXDEBUG httpTrsxDebug;

	#if defined(__AVR__) && defined(__GNUC__)
		void httpTrsx_UARTdebug_setPrintFx(PTRFX_retVOID_arg1_PCHAR_arg2_INT8_T UART_print)
		{
			httpTrsxDebug.UART_print = UART_print;
		}
	#else
		void httpTrsx_UARTdebug_setPrintFx(PTRFX_retVOID_arg1_PCHAR UART_print)
		{
			httpTrsxDebug.UART_print = UART_print;
		}
	#endif

	void httpTrsx_UARTdebug_setPrintCharFx(PTRFX_retVOID_arg1_CHAR fx)
	{
		httpTrsxDebug.UART_printChar = fx;
	}
	//
	void httpTrsx_UARTdebug_enabled(TRSX *trsx, BOOLEAN_T _bool)
	{
	    trsx->dbg.bf.enabled = _bool.k;
	    //httpTrsxDebug.dbg.UART_print = UART_print;
	}
	void httpTrsx_UARTdebug_print(TRSX *trsx, char *str, int8_t mode)
	{
	    if (trsx->dbg.bf.enabled)
	        {httpTrsxDebug.UART_print(str, mode);}
	}

	void httpTrsx_UARTdebug_printChar(TRSX *trsx, char c)
	{
	    if (trsx->dbg.bf.enabled)
	        {httpTrsxDebug.UART_printChar(c);}
	}

#endif
/******************************************************************************************/
/*Print standard (RAM)*/
void http_print(TRSX *trsx, const char *s)
{
    #if defined(__AVR__) && defined(__GNUC__)
        trsx->client->print(s);

        #ifdef HTTPTRSX_DEBUG
            httpTrsx_UARTdebug_print(trsx, s, 0);
        #endif
    #else

    #endif
}
/******************************************************************************************/
void http_printk(TRSX *trsx, char *s)
{
    #ifdef FS_STRING

		trsx->client->print(reinterpret_cast <const __FlashStringHelper *> (s) );

		#ifdef HTTPTRSX_DEBUG
            httpTrsx_UARTdebug_print(trsx, s, 1);
        #endif
    #else
		http_print(trsx, s);
    #endif
}
/******************************************************************************************/

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
    #if defined(__AVR__) && defined(__GNUC__)
    snprintf(str, sizebuff, "%d.%d.%d.%d", Ethernet.localIP()[0], Ethernet.localIP()[1], Ethernet.localIP()[2], Ethernet.localIP()[3]);
    #else
    #endif
}
/******************************************************************************************/
/******************************************************************************************/
int16_t tcpClient_getBytesAvailable(TRSX *trsx)
{
    #if defined(__AVR__) && defined(__GNUC__)
    	return trsx->client->available();
    #else
    #endif
}
int8_t tcpClient_connected(TRSX *trsx)
{
    #if defined(__AVR__) && defined(__GNUC__)
    	return trsx->client->connected();
    #else
    #endif
}
void tcpClient_stop(TRSX *trsx)
{
    #if defined(__AVR__) && defined(__GNUC__)
    	trsx->client->stop();
    #else
    #endif
}
char httpClient_readChar(TRSX *trsx) 
{
    #if defined(__AVR__) && defined(__GNUC__)
    	return trsx->client->read();
    #else
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
void http_send_msgbody(TRSX *trsx, JSON *json, uint8_t npairs)
{
    int i;
    http_printk(trsx, FS("{"));
    for (i=0; i<npairs; i++)
    {
        http_printk(trsx, FS("\""));
        http_print(trsx, json[i].name);
        http_printk(trsx, FS("\":"));
        http_print(trsx, json[i].strval);
        
        if (i< (npairs-1))
            {http_printk(trsx, FS(","));}
    }
    http_printk(trsx, FS("}"));
}
/******************************************************************************************/
void httpTrsx_setHost(TRSX *trsx, char *host)
{
    trsx->Host = host;
}
void httpTrsx_setURI(TRSX *trsx, char *URI)
{
    trsx->URI = URI;
}
void httpTrsx_setApiKey(TRSX *trsx, char *ApiKey)    
{
    trsx->ApiKey = ApiKey;
}
void httpTrsx_setHdrLine(TRSX *trsx, char *HdrLine)
{
    trsx->HdrLine = HdrLine;
}
/******************************************************************************************/
int8_t httpTrsx_requestMsg(TRSX *trsx, JSON *json, uint8_t npairs)//send the request message to HTTP server
{
    int8_t cod_ret = 0;
    char buff[20];
    /*1) Request Line*/
    http_printk(trsx, FS("POST "));http_print(trsx, trsx->URI);http_printk(trsx, FS(" HTTP/1.1\r\n"));
    
    /*2) Header lines*/
    http_printk(trsx, FS("Host: "));http_print(trsx, trsx->Host);http_printk(trsx, FS("\r\n"));
    //http_printk(trsx, FS("Connection: keep-alive\r\n"));//HTTP persistent connection
    http_printk(trsx, FS("Connection: close\r\n"));   
    http_printk(trsx, FS("Content-Type: application/json\r\n"));
    http_printk(trsx, FS("User-Agent: Agent/1.00\r\n"));

    if (trsx->ApiKey != NULL)
    {
    	http_printk(trsx, FS("api_key: "));http_print(trsx, trsx->ApiKey);http_printk(trsx, FS("\r\n"));
    }

    if (trsx->HdrLine != NULL)
        {http_print(trsx, trsx->HdrLine);http_printk(trsx, FS("\r\n"));}

    http_printk(trsx, FS("Content-Length: "));
    uint32toa(json_getContentLength(json, npairs) ,  buff, sizeof(buff)/sizeof(buff[0]));
    http_print(trsx, buff);http_printk(trsx, FS("\r\n"));
    
    /*3) Send New Line*/
    http_printk(trsx, FS("\r\n"));
    /*4) Send msg body*/
    http_send_msgbody(trsx, json, npairs);http_printk(trsx, FS("\r\n"));
    
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

//1=finish
//2=outmsg is already to parse
int8_t httpTrsx_responseMsg(TRSX *trsx, char *outmsg, uint16_t outmsgSize)
{
    unsigned long tmr_readbuffer;
    char c;
    uint16_t idx;
    int8_t cod_ret = 0;

    int sk=0;
    
    if (trsx->respMsg.sm0 == 0)
    {
        trsx->respMsg.timer.responseMsg_totalTimeout = __millis();
        //trsx->respMsg.idx = 0;

        trsx->respMsg.sm1 = 0;
        trsx->respMsg.sm0++;

        //++--
		while (1)//(tcpClient_getBytesAvailable(trsx) > 0)
		{
			if (sk == 0)
			{
				if (httpClient_readChar(trsx) == '\r')
					sk++;
			}
			if (sk == 1)
			{
				if (httpClient_readChar(trsx) == '\n')
					sk++;
				else
					sk = 0;
			}
			if (sk == 2)
			{
				if (httpClient_readChar(trsx) == '\r')
					sk++;
				else
					sk=  0;
			}
			if (sk == 3)
			{
				if (httpClient_readChar(trsx) == '\n')
					{sk++;
					break;
					}
				else
					sk = 0;
			}
		}
		//--+
    }
    if (trsx->respMsg.sm0 == 1)
    {
        tmr_readbuffer = __millis();
        idx = 0;
        do
        {
            if (tcpClient_getBytesAvailable(trsx) > 0)//buffer>0
            {
                c = httpClient_readChar(trsx);
				#ifdef HTTPTRSX_DEBUG
                	httpTrsx_UARTdebug_printChar(trsx, c);
				#endif
                    
                if (outmsg!= NULL)
                {
                    outmsg[idx] = c;
                    if (++idx >= outmsgSize)//as circular buffer
                    	{idx = 0;}
                    //
                    cod_ret = 2;
                }
            }
            else
            {
                break;
            }
        }
        while ((__millis() - tmr_readbuffer) <= KTIMEOUT_READBUFFER);  //CPU assigned for window

        if (trsx->respMsg.sm1 == 0)
        {
            if (!tcpClient_connected(trsx))
            {
                trsx->respMsg.timer.afterServerDisconneted_flushbuffer = __millis();
                trsx->respMsg.sm1++;
            }
        }
        else
        {
            if ( (__millis() - trsx->respMsg.timer.afterServerDisconneted_flushbuffer) >= KTIMEOUT_AFTERSERVERDISCONNECTED_FLUSHBUFFER ) //let a time to read all rx buffer
            {
                tcpClient_stop(trsx);
                trsx->respMsg.sm0 = 0;
                cod_ret = 1;
            }
        }

        //connection time-out
        if ( (__millis() - trsx->respMsg.timer.responseMsg_totalTimeout) >= KTIMEOUT_RESPONSEMSG_TOTALTIMEOUT) //abort and stop conection
        {
            tcpClient_stop(trsx);
            trsx->respMsg.sm0 = 0;
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


#ifdef SOCKET_DEBUG
#include "utility/w5100.h"
#include "utility/socket.h"
void ShowSocketStatus(void);
#endif

void httpTrsx_setupServerByDomain(TRSX *trsx, char *domain, uint16_t port)
{
    trsx->domain = domain;
    trsx->port = port;
}
void httpTrsx_setupServerByIP(TRSX *trsx, uint8_t *IP, uint16_t port)
{
    trsx->IP = IP;
    trsx->port = port;
}
int8_t tcpClient_connection(TRSX *trsx)
{
    int8_t cod_ret;
    
    #if defined(__AVR__) && defined(__GNUC__)
		if (trsx->domain != NULL)
			cod_ret = trsx->client->connect(trsx->domain, trsx->port);
		else
			cod_ret = trsx->client->connect(trsx->IP, trsx->port);
	#else
	#endif

	return cod_ret;
}

//1=finish
//2=outmsg is already to parse
int8_t httpTrsx_do1trsx(TRSX *trsx, JSON *json, uint8_t npairs, char *outmsg, uint16_t outmsgSize)
{
    static int8_t sm0;
    int8_t cod_ret = 0;

    if (sm0 == 0)// client opens a connection
    {
        if (tcpClient_connection(trsx))
        {
            sm0++;
        }
        else
        {
        	//httpTrsxDebug.UART_print("ERROR: Conexion ", 1);
        	Serial.println(PSTR("ERROR: Conexion"));
        	//
            #ifdef SOCKET_DEBUG
            Serial.println(F("Socket status in error before client.stop():"));
            ShowSocketStatus();
            Serial.println(F("Socket status in error after client.stop()"));
            #endif

            tcpClient_stop(trsx);
            cod_ret = 1;
        }
    }
    if (sm0 == 1)//client->server: send request message
    {
        if (httpTrsx_requestMsg(trsx, json, npairs))
        {
            sm0++;
        }
    }
    if (sm0 == 2)//server->client: receive response message
    {
        //if (httpTrsx_responseMsg(trsx, outmsg))
        cod_ret = httpTrsx_responseMsg(trsx, outmsg, outmsgSize);
        //1=finish
		//2=outmsg is already to parse
        if (cod_ret == 1)
        {
            sm0 = 0x00;
            cod_ret = 1;
        }
    }
    
    #ifdef SOCKET_DEBUG
    ShowSocketStatus();
    #endif
    
    return cod_ret;
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
int8_t httpTrsx_getStatus(TRSX *trsx)
{
    return trsx->exec.status;
}
void httpTrsx_setExecInterval_ms(TRSX *trsx, unsigned long interval_ms)
{
    trsx->exec.execInterval_ms = interval_ms;
}
void httpTrsx_setExecMode(TRSX *trsx, HTTP_TRSX_SET_EXEC_MODE execMode)
{
    trsx->exec.execMode = execMode.k;
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
int8_t httpTrsx_job(TRSX *trsx, JSON *json, uint8_t npairs, char *outmsg, uint16_t outmsgSize)
{
    int8_t cod_ret = 0;
    
    if (trsx->exec.status == IDLE)
    {
        trsx->exec.last_execMode = trsx->exec.execMode;

        if (trsx->exec.last_execMode > EM_WAIT_NEW_EXEC_MODE_E)
        {
            if (trsx->exec.last_execMode == EM_RUN_INTERVAL_E)
            {
                if (trsx->exec.runInterval_sm0 == 0)
                {
                    trsx->exec.status = RUNNING;
                    trsx->exec.runInterval_sm0++;	
                    /*execute 1 HTTP transaction, then comeback to here to continue with runInterval_sm0=1*/
                }
                else if (trsx->exec.runInterval_sm0 == 1)//for next evaluation
                {
                    trsx->exec.tmr_runInterval = __millis();
                    trsx->exec.runInterval_sm0++;
                }
                else if (trsx->exec.runInterval_sm0 == 2)
                {
                    if ( (__millis()-trsx->exec.tmr_runInterval) >= trsx->exec.execInterval_ms)
                    {
                        trsx->exec.runInterval_sm0 = 0x00;
                    }
                }
            }
            else if (trsx->exec.last_execMode == EM_RUN_ONCE_E)
            {
                trsx->exec.status = RUNNING;
            }
            else//STOP
            {
                httpTrsx_setExecMode(trsx, EM_WAIT_NEW_EXEC_MODE);
                trsx->exec.runInterval_sm0 = 0x00;	//reset
            }
        }
    }
    else
    {
    	//if ( httpTrsx_do1trsx(trsx, json, npairs, outmsg) == 1)//end?
    	cod_ret = httpTrsx_do1trsx(trsx, json, npairs, outmsg, outmsgSize);
        //1=finish
    	//2=outmsg is already to parse
		if (cod_ret == 1)//end?
        {
            trsx->exec.status = IDLE;

            if (trsx->exec.last_execMode == EM_RUN_ONCE_E)
                {httpTrsx_setExecMode(trsx, EM_STOP);}
            
            //cod_ret = 1;//1 transaction was completed
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


