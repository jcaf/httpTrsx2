#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include "httpTrsx2.h"
//Arduino MEGA pinout
#define LED1 34 //PC3
#define LED2 35 //PC2
#define LED3 36 //PC1
#define LED4 37 //PC0
#define LED5 38 //PD7
#define LED6 39 //PG2


/*NIC setup*/
uint8_t IP[4]= {192,168,1,60};//fallback
uint8_t MAC[6]= { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

/*Connect to server - setting*/
//char server[] = "api.quanticoservices.net";
uint8_t IPaddr_server[]={192, 168, 1, 54};

void UART_setup(void)
{
    #if defined(__AVR__) && defined(__GNUC__)
		Serial.begin(230400);//9600
		while (!Serial) {;}// wait for serial port to connect. Needed for native USB port only
    #else
    #endif
}
void UART_printChar(char c)
{
	#if defined(__AVR__) && defined(__GNUC__)
		Serial.write(c);
	#else
	#endif
}
#if defined(__AVR__) && defined(__GNUC__)
 	 
    void UART_print(char *str, int8_t mode)
    {
        if (mode == 0)
        {
        	Serial.print(str);
        }
        else
        {
			#ifdef FS_STRING
        	Serial.print(reinterpret_cast <const __FlashStringHelper *> (str));
			#else
        	Serial.print(str);
			#endif
        }
    }
	#define UART_printStrk(s) do{UART_print(s, 1);}while(0)
	#define UART_printStr(s) do{UART_print(s, 0);}while(0)
    
#else
    void UART_printStr(char *str)
    {
    }
#endif

//
#define JSON_SIZEMAX 1//4
JSON jsonw[JSON_SIZEMAX];
JSON jsonr[JSON_SIZEMAX];
char strval[30];
char strval1[30];

#define TRSX_NUMMAX 2
TRSX trsx[TRSX_NUMMAX];
//
void spi_deselect_devices(void);//only for QUANTICO BOARD
EthernetClient client;
//
void setup(void)
{
	digitalWrite(LED3, LOW);
	pinMode(LED3,OUTPUT);

	char buff[30];
    spi_deselect_devices();
    UART_setup();
    
	#ifdef HTTPTRSX_DEBUG
    httpTrsx_UARTdebug_setPrintFx(UART_print);//library point to this funcion()
    httpTrsx_UARTdebug_setPrintCharFx(UART_printChar);
    #endif    
    
    //1) local network setting
    NIC_begin(MAC, IP);//by default DHCP
    NIC_getMyIP(buff, sizeof(buff));
    UART_printStrk(FS("My IP: "));UART_printStr(buff); UART_printStrk(FS("\n"));
    delay(1000);
    
    //2) Set trsx[0] 
	httpTrsx_setClient(&trsx[0], (Client*)&client);//Only for Arduinochar strval[30];//client.setTimeout(1000);
    httpTrsx_setupServerByIP(&trsx[0], IPaddr_server, 80);
    httpTrsx_setURI(&trsx[0], "/jsondecode.php");
    httpTrsx_setHost(&trsx[0], "192.168.1.54");
    //
    //httpTrsx_setApiKey(&trsx[0], "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6IjE1MzU0MjczNTVfcGFibG8iLCJkZXZpY2VfaWQiOiI1YjdmMjc3ZmVmNGFkNjgxYjIwM2I0NDQiLCJlbWFpbCI6InBhYmxvZG9uYXlyZUBnbWFpbC5jb20iLCJpYXQiOjE1NjQwODgwMjR9.G8BWFQ1O_KH4hVfibYSlGd-UqQLdWZ1d_sxonbhqANc");
    httpTrsx_setHdrLine(&trsx[0], "api_key_write: eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6IjE1MzU0MjczNTVfcGFibG8iLCJkZXZpY2VfaWQiOiI1YjdmMjc3ZmVmNGFkNjgxYjIwM2I0NDQiLCJlbWFpbCI6InBhYmxvZG9uYXlyZUBnbWFpbC5jb20iLCJpYXQiOjE1NjQwODgwMjR9.G8BWFQ1O_KH4hVfibYSlGd-UqQLdWZ1d_sxonbhqANc");
    //
    httpTrsx_setExecInterval_ms(&trsx[0], 000);//ms
	httpTrsx_setExecMode(&trsx[0], EM_RUN_INTERVAL);//RUN_ONCE EM_RUN_INTERVAL
	#ifdef HTTPTRSX_DEBUG
	httpTrsx_UARTdebug_enabled(&trsx[0], TRUE);//
	#endif
	
	//2) Set trsx[1] 
	httpTrsx_setClient(&trsx[1], (Client*)&client);//Only for Arduinochar strval[30];//client.setTimeout(1000);
	httpTrsx_setupServerByIP(&trsx[1], IPaddr_server, 80);
	httpTrsx_setURI(&trsx[1], "/jsondecode1.php");
	httpTrsx_setHost(&trsx[1], "192.168.1.54");
	//
	//httpTrsx_setApiKey(&trsx[1], "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6IjE1MzU0MjczNTVfcGFibG8iLCJkZXZpY2VfaWQiOiI1YjdmMjc3ZmVmNGFkNjgxYjIwM2I0NDQiLCJlbWFpbCI6InBhYmxvZG9uYXlyZUBnbWFpbC5jb20iLCJpYXQiOjE1NjQwODgwMjR9.G8BWFQ1O_KH4hVfibYSlGd-UqQLdWZ1d_sxonbhqANc");
	httpTrsx_setHdrLine(&trsx[1], "api_key_read: eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6IjE1MzU0MjczNTVfcGFibG8iLCJkZXZpY2VfaWQiOiI1YjdmMjc3ZmVmNGFkNjgxYjIwM2I0NDQiLCJlbWFpbCI6InBhYmxvZG9uYXlyZUBnbWFpbC5jb20iLCJpYXQiOjE1NjQwODgwMjR9.G8BWFQ1O_KH4hVfibYSlGd-UqQLdWZ1d_sxonbhqANc");
	//
	httpTrsx_setExecInterval_ms(&trsx[1], 000);//ms
	httpTrsx_setExecMode(&trsx[1], EM_RUN_INTERVAL);//RUN_ONCE EM_RUN_INTERVAL
	#ifdef HTTPTRSX_DEBUG
	httpTrsx_UARTdebug_enabled(&trsx[1], TRUE);//TRUE
	#endif
    
	jsonw[0].name = "m1";
	float value[4] = {1,2,3,4};
    json_cFloatArr(value, 4, strval);
    jsonw[0].strval = strval;
    //json[0].strval = "[756,123.5,4.8,4.69]";
    
    jsonr[0].name = "m1";
	float value1[4] = {9,8,7,6};
	json_cFloatArr(value1, 4, strval1);
	jsonr[0].strval = strval1;
}
/*
void httpTrsx_test1(void)
{
	static int c;
	//if (httpTrsx_job(&trsx[0], json, 1, outmsg))
	if (httpTrsx_job(&trsx[0], jsonw, 1, (void*)NULL))
		if (++c == 3)
			{httpTrsx_setExecMode(&trsx[0], EM_STOP);}
}
*/

/*
void httpTrsx_man(void)
{
	static int8_t sm0;
	
	if (sm0 == 0)
	{
		if (httpTrsx_job(&trsx[0], jsonw, 1, (void*)NULL))
		{
			sm0++;
		}
	}
	else if (sm0 == 1)
	{
		if (httpTrsx_job(&trsx[1], jsonr, 1, (void*)NULL))
		{
			sm0 = 0x00;
		}
	}
}
*/

void httpTrsx_man1(void)
{
	static int8_t sm0;
	int8_t cod_ret=0;
	
	JSON json;
	#define STREAMSIZE 64
	char stream[STREAMSIZE];
	int8_t cod=0;
	
	if (sm0 == 0)
	{
		if (httpTrsx_job(&trsx[0], jsonw, 1, (void*)NULL, 0) == 1)
		{
			sm0++;
		}
	}
	else if (sm0 == 1)
	{
		cod_ret = httpTrsx_job(&trsx[1], jsonr, 1, stream, sizeof(stream));
		if (cod_ret > 0)
		{
			do
			{
				json.name = (void*)NULL;
				cod = jsonDecode(stream, sizeof(stream), &json);
				if (json.name != NULL)
				{
//					UART_printStr(json.name);
//					UART_printStrk(FS("\n"));
//					UART_printStr(json.strval);
//					UART_printStrk(FS("\n"));
//					UART_printStrk(FS("\n"));
					UART_printStrk(FS("\n"));
					if (strcmp(json.name, "m1") == 0)
					{
//						UART_printStrk(FS("m1 Json value: "));
//						UART_printStr(json.strval);
//						UART_printStrk(FS("\n"));
					}
					else if (strcmp(json.name, "ch2") == 0)
					{
//						UART_printStrk(FS("ch2 Json value: "));
//						UART_printStr(json.strval);
//						UART_printStrk(FS("\n"));
					}
					else if (strcmp(json.name, "num") == 0)
					{
//						UART_printStrk(FS("num Json value: "));
//						UART_printStr(json.strval);
//						UART_printStrk(FS("\n"));

						double doub= strtod(json.strval, NULL);
						if ( ((int)doub) > 0)//False
						{
							//UART_printStrk(FS("LED TURN ON\n"));
							digitalWrite(LED3,HIGH);
						}
						else
						{
							//UART_printStrk(FS("LED TURN OFF\n"));
							digitalWrite(LED3,LOW);
						}
					}
				}
			}while (cod == 0);
			//
			if (cod_ret == 1)//finish transmision
			{
				sm0 = 0x00;
			}
		}
	}
}
void loop(void)
{
	//httpTrsx_test1();
	//httpTrsx_man();
	httpTrsx_man1();
}

void spi_deselect_devices(void)//only for QUANTICO BOARD
{
    #define WIZNET_CS 10
    #define SDCARD_CS 4
    #define LTC6804_CS 5
    pinMode(WIZNET_CS, OUTPUT);
    pinMode(SDCARD_CS, OUTPUT);
    pinMode(LTC6804_CS, OUTPUT);
    digitalWrite(WIZNET_CS, HIGH);
    digitalWrite(SDCARD_CS, HIGH);
    digitalWrite(LTC6804_CS, HIGH);
}