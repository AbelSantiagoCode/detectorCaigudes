#include <stdbool.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdint.h>


#include "SparkFun_ADXL345.h"
#include "FSM_Mostreig.h"
#include "FSM_Posicio.h"
#include "adxl.h"

#define G2ADXL(X) (int)(X/0.0078)
#define TIME_OUT 200
#define TIME_OUT_2 20
#define MIDA 11


static int values_range[]={0,12,25,38,51,64,76,91,102,115,128};
static int x_range[]={0,0,0,0,0,0,0,0,0,0,0};
static int y_range[]={0,0,0,0,0,0,0,0,0,0,0};
static int z_range[]={0,0,0,0,0,0,0,0,0,0,0};

void init_mostreig(){
  init_adxl();
}

bool first_threshold(const int x, const int y, const int z){

    if ((x>=91) && (y>=91)){
        return true;
    }
    else if((x>=91) && (z>=91)){
        return true;
    }
    else if((y>=91) && (z>=91)){
        return true;
    }
    else{
        return false;
    }
}

static uint8_t maxim (const int *llista){
      uint8_t maxValue=0;
      uint8_t posicio;

      for (uint8_t i=0; i<MIDA;i++){
        if(llista[i]>= maxValue){
            maxValue=llista[i];
            posicio=i;
        }
      }
      return posicio;      
}

void check_zone_threshold(){

    uint8_t pos_x;
    uint8_t pos_y;
    uint8_t pos_z;
    pos_x = maxim(x_range);
    pos_y = maxim(y_range);
    pos_z = maxim(z_range);
    
    if((values_range[pos_x]>=91 && values_range[pos_y]>=91)){
        send_event(Threshold);
    }
    else if (values_range[pos_x]>=91 && values_range[pos_z]>=91){
        send_event(Threshold);
    }
    else if (values_range[pos_y]>=91 && values_range[pos_z]>=91){
        send_event(Threshold);
    }
}

void classificar_rangX(const int x){

    if (x>=128){
        x_range[10]=x_range[10]+1;
    }
    else if (x>=115){
        x_range[9]=x_range[9]+1;
    }
    else if (x>=102){
        x_range[8]=x_range[8]+1;
    }
    else if (x>=91){
        x_range[7]=x_range[7]+1;
    }
    else if (x>=76){
        x_range[6]=x_range[6]+1;
    }
    else if (x>=64){
        x_range[5]=x_range[5]+1;
    }
    else if (x>=51){
        x_range[4]=x_range[4]+1;
    }
    else if (x>=38){
        x_range[3]=x_range[3]+1;
    }
    else if (x>=25){
        x_range[2]=x_range[2]+1;
    }
    else if (x>=12){
        x_range[1]=x_range[1]+1;
    }
   else if (x>=0){
        x_range[0]=x_range[0]+1;
    }
}
void classificar_rangY(const int y){

    if (y>=128){
        y_range[10]=y_range[10]+1;
    }
    else if (y>=115){
        y_range[9]=y_range[9]+1;
    }
    else if (y>=102){
        y_range[8]=y_range[8]+1;
    }
    else if (y>=91){
        y_range[7]=y_range[7]+1;
    }
    else if (y>=76){
        y_range[6]=y_range[6]+1;
    }
    else if (y>=64){
        y_range[5]=y_range[5]+1;
    }
    else if (y>=51){
        y_range[4]=y_range[4]+1;
    }
    else if (y>=38){
        y_range[3]=y_range[3]+1;
    }
    else if (y>=25){
        y_range[2]=y_range[2]+1;
    }
    else if (y>=12){
        y_range[1]=y_range[1]+1;
    }
   else if (y>=0){
        y_range[0]=y_range[0]+1;
    }
}

void classificar_rangZ(const int z){

    if (z>=128){
        z_range[10]=z_range[10]+1;
    }
    else if (z>=115){
        z_range[9]=z_range[9]+1;
    }
    else if (z>=102){
        z_range[8]=z_range[8]+1;
    }
    else if (z>=91){
        z_range[7]=z_range[7]+1;
    }
    else if (z>=76){
        z_range[6]=z_range[6]+1;
    }
    else if (z>=64){
        z_range[5]=z_range[5]+1;
    }
    else if (z>=51){
        z_range[4]=z_range[4]+1;
    }
    else if (z>=38){
        z_range[3]=z_range[3]+1;
    }
    else if (z>=25){
        z_range[2]=z_range[2]+1;
    }
    else if (z>=12){
        z_range[1]=z_range[1]+1;
    }
   else if (z>=0){
        z_range[0]=z_range[0]+1;
    }
}

void reset_buffer_2(){
    for(int i=0;i<MIDA;i++)
    {
      x_range[i]=0;
      y_range[i]=0;
      z_range[i]=0;
    }
    

}
void run_FSM(){
    static int state=1;
    static int x,y,z;
    static uint8_t finestra=0;

    while(true){

        while(!read_adxl(&x,&y,&z)); // espera nova dades segons la frequencia de mostreig
        classificar_rangX(abs(x));   // modificar llista x_range, es a dir, modificar la llista de contadors
        classificar_rangY(abs(y));
        classificar_rangZ(abs(z));
        
        if (state==1){
           
            if (finestra==TIME_OUT){             
                setBuffers(x_range,y_range,z_range); // FSM POSICIO
                reset_buffer_2();
                finestra=0;
                send_event(Time_out); // FSM POSICIO
            }
            else if (first_threshold(abs(x),abs(y),abs(z))){
                state=2;
                reset_buffer_2();
                finestra=0;
            }
            
        }
        else{
            if (finestra==TIME_OUT_2){             
                state=1;
                check_zone_threshold();
                reset_buffer_2();
                finestra=0;
            }
        }
        finestra=finestra+1;
    }
}



