#include <inttypes.h>
#include <avr/io.h>
#include "serial_device.h"

#define NUM (F_CPU/16)

#define BAUDRATE_L(x) UINT8_C((NUM/x-1)      & 0xff)
#define BAUDRATE_H(x) UINT8_C((NUM/x-1) >> 8 & 0xf)

/*
 * Initialize the UART0
 */
void serial_init(void) {

  // SET BAUD RATE TO 115200 BPS 
  UBRR0=16;
  UCSR0A |= (1<<U2X0);
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);

  // SET BAUD RATE TO 230400 BPS 
  //UBRR0=8;
  //UCSR0A |= (1<<U2X0);
  //UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  // set baud rate to 9600 bps. Ignore lower rates.
  //UBRR0H = BAUDRATE_H(9600);
  //UBRR0L = BAUDRATE_L(9600);
  // set normal baud rate 
  //UCSR0A = UINT8_C(0);
  /*
  UCSR0C = 
    (_BV(UCSZ01)   | _BV(UCSZ00)) &   // 8 bit frame
    ~_BV(UMSEL01) & ~_BV(UMSEL00) &   // asincronous mode
    ~_BV(UPM01)   & ~_BV(UPM00)   &   // no parity
    ~_BV(USBS0)   ;                   // 1 stop bit
  
  // Enable transmision and reception
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);*/
}


uint8_t serial_get(void) {
  // polling on RXC until data received
  loop_until_bit_is_set(UCSR0A, RXC0);  	
  return UDR0;
}


bool serial_can_read(void) {
  // test whether there is something to read
  return bit_is_set(UCSR0A, RXC0);
}


void serial_put(uint8_t c) {
  // wait last transmision finishes
  loop_until_bit_is_set(UCSR0A, UDRE0);
  // send new byte
  UDR0 = c;
}


void serial_println(const char * string){
  for( uint8_t i=0; string[i]!='\0';i++ ){
    serial_put((uint8_t)string[i]);
    
  }
  serial_put( (uint8_t) '\n');
  serial_put((uint8_t) '\r');
  
}

void serial_print(const char * string){  
  for( uint8_t i=0; string[i]!='\0';i++ ){
    serial_put((uint8_t)string[i]);
    
  }
  
}


