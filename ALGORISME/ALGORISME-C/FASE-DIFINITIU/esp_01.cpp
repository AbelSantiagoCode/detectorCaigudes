#include "esp_01.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>

#include "serial_device.h"


static void check_ok(void);

static void check_ok(void){
  uint8_t c;
  uint8_t count=0;
  bool flag=true;
  while(flag){
    
     if (serial_can_read()) {
        c= serial_get();    
        switch(count){
          case 0:
             if(c=='O'){
                count=1;                
             }              
          case 1: 
             if (c=='K'){
                flag = false;
             }
             count =0;
        }
     }
  }
}

void setup_ESP01(const char * nameWifi,const char * passwordWifi,const char * IPWifi,const char * PortWifi){

  serial_init(); // baudrate of 115200
  char command[100] = "AT+CWJAP_DEF=\"\0"; 

  //----------------------------------------------------
  serial_print("AT+UART_DEF=115200,8,1,0,0\r\n");
  check_ok();


  //---------------------------------------------------
  serial_print("AT+CWMODE=1\r\n");
  //check_ok();


  //---------------------------------------------------
  strcat(command, nameWifi);
  strcat(command, "\",\"\0");
  strcat(command, passwordWifi);
  strcat(command, "\"\r\n");
  serial_print(command);
  check_ok();

  //---------------------------------------------------  
  serial_print("AT+CIPMUX=0\r\n");
  check_ok();

  //---------------------------------------------------
  strcpy(command,"AT+CIPSTART=\"TCP\",\"\0");
  strcat(command,IPWifi);
  strcat(command,"\",\0");
  strcat(command,PortWifi);
  strcat(command,"\r\n");
  serial_print(command);
  check_ok();

}

void sendstring_ESP01(const char * string){
  char command[100]="AT+CIPSEND="; 
  char stringLen[10];
  int len;
  len = (int) strlen(string);
  sprintf(stringLen,"%i",len);
  strcat(command,stringLen);
  strcat(command,"\r\n");
  //strcat(string,"\r\n");
  
  serial_print(command);
  check_ok();

  serial_print(string);
  check_ok();
}
