#include "adxl.h"
#include "SparkFun_ADXL345.h"
#include "tmr0.h" // configuracio frequencia de mostreig
#include <avr/interrupt.h>
#include <stdbool.h>


#define valortimer 155
ADXL345  adxl = ADXL345();             // PER COMUNICACIO I2C
static volatile bool flag_lectura=false;

void init_adxl(void){

  adxl.powerOn();                     // ENGEGUEM ADXL

  adxl.setRangeSetting(4);           // DIEM EL RANG DE G A LA QUE VOLEM TREBALLAR
                                      // ACCEPTA VALORS ENTRE 2g, 4g, 8g or 16g
                                      // VELORS ALTS = rang de mesura mes apli
                                      // VALORS PETITS = rang de mesura més acutat
   
  
    /*  CONFIGURACIONS TIMER 0 */
  setup_tmr0(valortimer,1024);
    //(ocr0a, tmr0_pre)
    //tmr0_pre 1,default=8,64,256,1024
    //TMR0=prescaler*(ocr0a+1)*T_clk
  
  sei();
  
}


ISR(TIMER0_COMPA_vect){   // frequencia de 100 Hz
  
  flag_lectura=true;   

}


bool read_adxl(int *x, int *y, int *z){

    if (flag_lectura==true){
        flag_lectura=false;
        adxl.readAccel(x, y, z);         // llegir valors de laccelarometre x,y,z
        return true;
    }
    return false;
}

