

#include "FSM_Mostreig.h"
#include "FSM_Posicio.h"
#include "botoAlarma.h"
/*

AQUEST PRORGRAMA IMPLEMENTA EL DETECTOR DE CAIUGUDES.TAMBE S'HA IMPLEMENTAT UNA CONNEXIO TCP A UN SERVIDOR DETERMINAT A PARTIR DEL MÒDUL ESP01 AMB EL FI DE COMUNICAR LES CAIUGUDES DETECTADES
*/
void setup(){
  posicio_init(); 
  init_mostreig();
  init_BotoAlarma();
    
}

void loop(){
  run_FSM();
}






