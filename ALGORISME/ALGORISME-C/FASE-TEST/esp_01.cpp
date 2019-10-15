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
        //serial_put(c);
        
        switch(count){
          case 0:
             //PORTD &=~(1<<PORTD4);
             if(c=='O'){
                //serial_println("rebut o");
                count=1;
                
                
             }
             //else{
               // serial_put(c);
             //} 
              
          case 1: 
             //PORTD &=~(1<<PORTD4);
             if (c=='K'){
                //serial_println("rebut OK");
                flag = false;
                //PORTD |=(1<<PORTD6);
             }
             count =0;
        }
     }
  }
}

void setup_ESP01(const char * nameWifi,const char * passwordWifi,const char * IPWifi,const char * PortWifi){
  DDRD |= (1<< DDD4)|(1<<DDD6)|(1<<DDD5);
  PORTD &=~(1<<PORTD4); 
  PORTD &=~(1<<PORTD5); 
  PORTD &=~(1<<PORTD6); 

  //PORTD |=(1<<PORTD4);
  serial_init(); // baudrate of 115200
  char command[100] = "AT+CWJAP_DEF=\"\0"; 

  //----------------------------------------------------
  serial_print("AT+UART_DEF=115200,8,1,0,0\r\n");
  check_ok();
  PORTD |=(1<<PORTD4);
  //_delay_ms(10);

  //---------------------------------------------------
  serial_print("AT+CWMODE=1\r\n");
  //check_ok();
  //PORTD |=(1<<PORTD5);
  //_delay_ms(10);

  //---------------------------------------------------
  strcat(command, nameWifi);
  strcat(command, "\",\"\0");
  strcat(command, passwordWifi);
  strcat(command, "\"\r\n");
  serial_print(command);
  check_ok();
  //_delay_ms(10000);

  //---------------------------------------------------  
  serial_print("AT+CIPMUX=0\r\n");
  check_ok();
  PORTD |=(1<<PORTD5);

  //_delay_ms(10);


  //---------------------------------------------------
  //serial_print("AT+CIPSTART=\"TCP\",\"192.168.43.197\",5000\r\n");
 
  strcpy(command,"AT+CIPSTART=\"TCP\",\"\0");
  strcat(command,IPWifi);
  strcat(command,"\",\0");
  strcat(command,PortWifi);
  strcat(command,"\r\n");
  serial_print(command);
  check_ok();
  PORTD |=(1<<PORTD6);

  //_delay_ms(2000);*/
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
  //_delay_ms(10);

  serial_print(string);
  check_ok();
  //_delay_ms(10);
}
