#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include "esp_01.h"
#include "botoAlarma.h"
//AQUEST MÒDUL REQUEREIX TINDRE UNA CONNEXIO A UN SERVIDOR JA ESTABLER, ES A DIR
//HAVER CRIDAT setup_ESP01();

static bool flag=false;

void init_BotoAlarma(void){
  DDRD &=~ (1<< DDD2);
  DDRD |= (1<< DDD6);
  PORTD &=~(1<<PORTD2); 
  PORTD &=~(1<<PORTD6); 
   
  EICRA &=~ (1<<ISC01);
  EICRA |= (1<<ISC00);
  EIMSK |=(1<<INT0);
  sei();

  //setup_ESP01("vodafoneADC0","BLGP74L9ZT2LAW","192.168.0.164", "5000");
   //setup_ESP01("Redmi","ohyq8158","192.168.43.50", "5000");
 
}
  
ISR(INT0_vect){
  if(PIND &(1<<PIND2)){
    PORTD &=~(1<<PORTD6);
  }
  else if((PIND &(1<<PIND2))==0b00000000   ){
    PORTD |=(1<<PORTD6);
    sendstring_ESP01( "!!!!!!!! DANGER : CAIGUDA DETECTADA !!!!!!!!" );
  }  

}
