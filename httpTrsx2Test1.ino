#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include "httpTrsx2.h"

//NIC setup
uint8_t IP[4]= {192,168,1,60};//fallback
uint8_t MAC[6]= { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//Connect to server - setting
//char server[] = "api.quanticoservices.net";
uint8_t IPaddr_server[]={192, 168, 1, 54};


//void json_cIntegerArr
void json_cFloatArr(float *v, int size, char *outbuff)
{
    char buff[20];
    
    int i;
    strcpy(outbuff,"[");
    
    for (i=0; i< size; i++)
    {
		#if defined(__AVR__) && defined(__GNUC__)
    	dtostrf(v[i], 0, 2, buff);
		#elif
        snprintf(buff, sizeof(buff), "%.2f", v[i]);
		#endif
        
        strcat(outbuff, buff);//alternatevly use strlcat
        
        if (i < size-1)
            strcat(outbuff, ",");
    }
    strcat(outbuff,"]");
}

void spi_deselect_devices(void)//only for QUANTICO BOARD
{
    #ifdef Arduino_h
    #define WIZNET_CS 10
    #define SDCARD_CS 4
    #define LTC6804_CS 5
    pinMode(WIZNET_CS, OUTPUT);
    pinMode(SDCARD_CS, OUTPUT);
    pinMode(LTC6804_CS, OUTPUT);
    digitalWrite(WIZNET_CS, HIGH);
    digitalWrite(SDCARD_CS, HIGH);
    digitalWrite(LTC6804_CS, HIGH);
    #endif
}
void UART_setup(void)
{
    #if defined(__AVR__) && defined(__GNUC__)
    Serial.begin(9600);
    while (!Serial) {;}// wait for serial port to connect. Needed for native USB port only
    #elif
    #endif
}

#if defined(__AVR__) && defined(__GNUC__)
    
    void UART_print(char *str, int8_t mode)
    {
        if (mode == 0)
            Serial.print(str);
        else
            Serial.print( reinterpret_cast <const __FlashStringHelper *> (str));
    }
#elif
    void UART_print(char *str)
    {

    }
#endif

EthernetClient client;

//
#define JSON_SIZEMAX 1//4
JSON jsonw[JSON_SIZEMAX];
JSON jsonr[JSON_SIZEMAX];
char strval[30];
char strval1[30];

#define TRSX_NUMMAX 2
TRSX trsx[TRSX_NUMMAX];

void setup(void)
{
	char buff[30];
    spi_deselect_devices();
    
    UART_setup();
	#ifdef HTTPTRSX_DEBUG
    httpTrsx_UARTdebug_setPrintFx(UART_print);//library point to this funcion()
	#endif    
    
    //1) local network setting
    NIC_begin(MAC, IP);//by default DHCP
    NIC_getMyIP(buff, sizeof(buff));
    UART_print(PSTR("My IP: "),1); UART_print(buff,0); UART_print(PSTR("\n"),1);
    delay(1000);
    
    //2) Set trsx[0] 
	httpTrsx_setClient(&trsx[0], (Client*)&client);//Only for Arduinochar strval[30];//client.setTimeout(1000);
    httpTrsx_setupServerByIP(&trsx[0], IPaddr_server, 80);
    httpTrsx_setURI(&trsx[0], "/jsondecode.php");
    httpTrsx_setHost(&trsx[0], "192.168.1.54");
    httpTrsx_setApiKey(&trsx[0], "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6IjE1MzU0MjczNTVfcGFibG8iLCJkZXZpY2VfaWQiOiI1YjdmMjc3ZmVmNGFkNjgxYjIwM2I0NDQiLCJlbWFpbCI6InBhYmxvZG9uYXlyZUBnbWFpbC5jb20iLCJpYXQiOjE1NjQwODgwMjR9.G8BWFQ1O_KH4hVfibYSlGd-UqQLdWZ1d_sxonbhqANc");
    httpTrsx_setExecInterval_ms(&trsx[0], 1000);//ms
	httpTrsx_setExecMode(&trsx[0], EM_RUN_INTERVAL);//RUN_ONCE EM_RUN_INTERVAL
	#ifdef HTTPTRSX_DEBUG
	httpTrsx_UARTdebug_enabled(&trsx[0], TRUE);
	#endif
	
	//2) Set trsx[1] 
	httpTrsx_setClient(&trsx[1], (Client*)&client);//Only for Arduinochar strval[30];//client.setTimeout(1000);
	httpTrsx_setupServerByIP(&trsx[1], IPaddr_server, 80);
	httpTrsx_setURI(&trsx[1], "/jsondecode1.php");
	httpTrsx_setHost(&trsx[1], "192.168.1.54");
	httpTrsx_setApiKey(&trsx[1], "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6IjE1MzU0MjczNTVfcGFibG8iLCJkZXZpY2VfaWQiOiI1YjdmMjc3ZmVmNGFkNjgxYjIwM2I0NDQiLCJlbWFpbCI6InBhYmxvZG9uYXlyZUBnbWFpbC5jb20iLCJpYXQiOjE1NjQwODgwMjR9.G8BWFQ1O_KH4hVfibYSlGd-UqQLdWZ1d_sxonbhqANc");
	httpTrsx_setExecInterval_ms(&trsx[1], 1000);//ms
	httpTrsx_setExecMode(&trsx[1], EM_RUN_INTERVAL);//RUN_ONCE EM_RUN_INTERVAL
	#ifdef HTTPTRSX_DEBUG
	httpTrsx_UARTdebug_enabled(&trsx[1], TRUE);
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
/*
 * */
//++++++++++++++++++++++++++++++++++++++++++++++++++
int8_t charIsNumber(char c)
{
	if ((c>='0') && (c<='9'))
		return 1;
	else
		return 0;
}

int8_t charIsLetter(char c)
{
	if ( ( (c>='A') && (c<='Z')) || ( (c>='a') && (c<='z') ) )
		return 1;
	else
		return 0;
}

//////////////////////////////////////////


///////////////////////////////////////////////////////////////////
int8_t jsonDecode(char *stream, uint16_t streamSize, JSON *json)
{
	static int16_t i;
	uint16_t iname = 0;
	uint16_t ivalue = 0;
	int8_t cod_ret = 0;

	int8_t sm0 = 0;
	int8_t sm1 = 0;

	while (1)
	{
		if (sm0 == 0)
		{
			if (stream[i] == '\"')
			{
				iname= i+1;//save initial position
				sm0++;
			}
		}
		else if (sm0 == 1)//cannot be " or any diferent to 'A-Z''a'b or"0.9"
		{
			if (stream[i] == '\"')//= 'A-Z') ) //letras y numeros
				sm0 = 0x00;
			else
				sm0++;
		}
		else if (sm0 == 2)
		{
			if (stream[i] == '\"')
			{
				sm0++;
			}
			else
            {
                if ( !charIsLetter(stream[i]) && !charIsNumber(stream[i]) )
                {
                    sm0 = 0x00;
                }
            }
		}
		else if (sm0 == 3)
		{
			if (stream[i] == ':')
			{
				stream[i-1] = '\0';
				sm0++;
			}
			else
			{
				sm0 =0;
			}
		}
		else if (sm0 == 4)
		{
			if (stream[i] == '\"')
			{
				ivalue = i+1;
				sm0 = 5;//string
			}
			else if (charIsNumber(stream[i]))
			{
				ivalue = i+1;
				sm0 = 6; //number
			}
			else if (stream[i] == '[')
			{
				ivalue = i;
				sm0 = 7;
			}
			else
			{
				sm0 = 0x00;
			}
		}
		//string----------------------------
		else if (sm0 == 5)
		{
			if (sm1 == 0)
			{
				if (stream[i] == '\"')
				{
					sm1++;
				}
			}
			else if (sm1 == 1)
			{
				if ((stream[i] == ',') || (stream[i] == '}'))
				{
                    stream[i-1] = '\0';
                    //
                    json->name = &stream[iname];
                    json->strval = &stream[ivalue];
                    sm1 = 0;
                    sm0 = 0;
                    //
                    break;

				}
			}
		}
		//number----------------------------
		else if (sm0 == 6)
		{

		}
		//vector----------------------------
		else if (sm0 == 7)
		{

		}

		//while end
		if (++i >= streamSize)
		{
			i = 0x00;
			sm0 = 0x0;
			sm1 = 0x0;
			cod_ret = 1;
			break;
		}
	}
	return cod_ret;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++

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
			//test en todos los casos
			//++++++++++++++++++++++++++
			do
			{
				json.name = (void*)NULL;
				cod = jsonDecode(stream, sizeof(stream), &json);
				if (json.name != NULL)
				{
					//printf("name is: %s\n", json.name);
					//printf("value is: %s\n", json.strval);
					Serial.println("\n Json decoded:");
					Serial.println(json.name);
					Serial.println(json.strval);
					Serial.println();
				}
			}while (cod == 0);
			//++++++++++++++++++++++++++
			
			if (cod_ret == 1)//finish transmision
			{
				sm0 = 0x00;
			}
			
		}
	}
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
void loop(void)
{
	//httpTrsx_test1();
	//httpTrsx_man();
	httpTrsx_man1();
}

