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
    char buff[10];
    
    int i=0;
    strcpy(outbuff,"[");
    
    for (i=0; i< size; i++)
    {
        //sprintf(buff, "%.2f", v[i]);
        snprintf(buff, sizeof(buff), "%.2f", v[i]);
        
        strcat(outbuff, buff);//alternatevly use strlcat
        
        if (i != size-1)
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

#define JSON_SIZEMAX 1//4
JSON json[JSON_SIZEMAX];

#define TRSX_NUMMAX 2
TRSX trsx[TRSX_NUMMAX];

void setup(void)
{
  char buff[30];
    spi_deselect_devices();
    
    UART_setup();
    
    //1) local network setting
    NIC_begin(MAC, IP);//by default DHCP
    NIC_getMyIP(buff, sizeof(buff));
    UART_print(buff,0);
    delay(1000);
    
    //2) Client config connection to server
    
    httpTrsx_setClient(&trsx[0], (Client*)&client);//Only for Arduino
    //client.setTimeout(1000);
    #ifdef HTTPTRSX_DEBUG
        httpTrsx_UARTdebug_setPrintFx(UART_print);//library point to this funcion()
        httpTrsx_UARTdebug_enabled(&trsx[0], TRUE);
    #endif
    
    httpTrsx_setupServerByIP(&trsx[0], IPaddr_server, 80);
    
    
    //3) Set HTTP transaction
    httpTrsx_setURI(&trsx[0], "/jsondecode.php");
    httpTrsx_setHost(&trsx[0], "192.168.1.54");
    httpTrsx_setApiKey(&trsx[0], "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6IjE1MzU0MjczNTVfcGFibG8iLCJkZXZpY2VfaWQiOiI1YjdmMjc3ZmVmNGFkNjgxYjIwM2I0NDQiLCJlbWFpbCI6InBhYmxvZG9uYXlyZUBnbWFpbC5jb20iLCJpYXQiOjE1NjQwODgwMjR9.G8BWFQ1O_KH4hVfibYSlGd-UqQLdWZ1d_sxonbhqANc");

    
    //
    json[0].name = "m1";
    
    // float value[4] = {1,2,3,4};
    // char strval[30];
    // json_cFloatArr(value, sizeof(strval)/sizeof(strval[0]), strval);
    //json[0].strval = strval;
    json[0].strval = "[756,88.6,4.8,4.69]";

    //4) Http transaction setting
    httpTrsx_setExecInterval_ms(&trsx[0], 1000);//ms
    httpTrsx_setExecMode(&trsx[0], EM_RUN_INTERVAL);//RUN_ONCE
}

char outmsg[HTTP_TRSX_RX_BUFFER_MAX_SIZE];


int c;
void loop(void)
{
    
    if (httpTrsx_job(&trsx[0], json, 1, outmsg))
    {
        if (++c == 3)
        {
            httpTrsx_setExecMode(&trsx[0], EM_STOP);
        }
    }
    
}
