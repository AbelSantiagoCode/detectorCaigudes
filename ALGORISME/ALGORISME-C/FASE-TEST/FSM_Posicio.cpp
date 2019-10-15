
#include <stdio.h>
#include <stdint.h>
#include "esp_01.h"
//#include "Arduino.h"
#include "serial_device.h"

#include "FSM_Posicio.h"

#define RANGE 11
#define LLINDAR_1 0.7
#define LLINDAR_2 0.5
#define G2ADXL(X) (uint8_t)(X/0.0078)
#define LLINDAR1 G2ADXL(LLINDAR_1)
#define LLINDAR2 G2ADXL(LLINDAR_2)


typedef struct {
  uint8_t x_range[RANGE];
  uint8_t y_range[RANGE];
  uint8_t z_range[RANGE];
  uint8_t values_range[RANGE];
} range_t;

typedef enum {Desconegut, Tumbat, Erecte} posicio_t;

typedef enum {s0, s1} estat_t;


//PROTOTIPES

static void reset_buffer(void);
static posicio_t update_posicio(void);
static void check_fall(void);
static void run_FSM(event_t event);
static uint8_t maxim (const int *llista);



// GLOBAL VARIABLES
static range_t buffer;
static posicio_t posicio_actual;
static posicio_t posicio_anterior;



static uint8_t maxim (const uint8_t *llista) {
  uint8_t maxValue = 0;
  uint8_t posicio = 0;



  for (uint8_t i = 0; i < RANGE; i++) {
    if (llista[i] >= maxValue) {

      maxValue = llista[i];
      posicio = i;
    }
  }

  return posicio;
}


void send_event(event_t event)
{
  run_FSM(event);
}

void posicio_init(void)
{

  float x = 0;

  //Serial.begin(115200);
  //serial_init();
  setup_ESP01("vodafoneADC0","BLGP74L9ZT2LAW","192.168.0.164", "5000");

  for (int i = 0 ; i < RANGE; i++)
  {
    buffer.values_range[i] = G2ADXL(x);
    x += 0.1;
  }
  reset_buffer();

  posicio_anterior = Desconegut;
  posicio_actual = Desconegut;
}


static void reset_buffer(void)
{
  for (int i = 0 ; i < RANGE; i++)
  {
    buffer.x_range[i] = 0;
    buffer.y_range[i] = 0;
    buffer.z_range[i] = 0;
  }
}

void setBuffers(int x[], int y[], int z[]) // int -> uint8_t
{
  for (uint8_t i = 0; i < RANGE; i++)
  {
    buffer.x_range [i] = x[i];
    buffer.y_range [i] = y[i];
    buffer.z_range [i] = z[i];
  }
}

static posicio_t update_posicio(void)
{
  uint8_t pos_x, maxX;
  uint8_t pos_y, maxY;
  uint8_t pos_z, maxZ;



  pos_x = maxim(buffer.x_range);
  pos_y = maxim(buffer.y_range);
  pos_z = maxim(buffer.z_range);

  maxX = buffer.values_range[pos_x];
  maxY = buffer.values_range[pos_y];
  maxZ = buffer.values_range[pos_z];


  reset_buffer();

  if (maxX >= LLINDAR1 && maxY <= LLINDAR2 && maxZ <= LLINDAR2)
  {
    return Erecte;
  }
  else if (maxX <= LLINDAR2 && maxY <= LLINDAR2 && maxZ >= LLINDAR1) // tumbat de cara o esquena
  {
    return Tumbat;
  }
  else if (maxX <= LLINDAR2 && maxY >= LLINDAR1 && maxZ <= LLINDAR2) // tumbat de costat dret o esquerra
  {
    return Tumbat;
  }
  else
  {
    return Desconegut;
  }
}

static void check_fall(void)
{
  if (posicio_anterior == Erecte && posicio_actual == Tumbat)
  {
    
    sendstring_ESP01("!!!!!!!! DANGER : CAIGUDA DETECTADA !!!!!!!!" );
    //serial_println("!!!!!!!! DANGER : CAIGUDA DETECTADA !!!!!!!!" );
  }
  else if (posicio_anterior == Tumbat && posicio_actual == Tumbat)
  {
    sendstring_ESP01( "!!!!!!!! DANGER : CAIGUDA DETECTADA !!!!!!!!" );
    //serial_println( "!!!!!!!! DANGER : CAIGUDA DETECTADA !!!!!!!!");
  }
  else if (posicio_anterior == Erecte && posicio_actual == Erecte)
  {
    sendstring_ESP01( "!!!!!!!! ALERTA-COBRA : CAIGUDA DE CUL DETECTADA !!!!!!!!" );
    //serial_println( "!!!!!!!! ALERTA-COBRA : CAIGUDA DE CUL DETECTADA !!!!!!!!" );
  }
  else if (posicio_anterior == Desconegut && posicio_actual == Tumbat)
  {
    sendstring_ESP01( "******** ALERT : POSSIBLE CAIGUDA DETECTADA ********"   );
    //serial_println("******** ALERT : POSSIBLE CAIGUDA DETECTADA ********" );
  }
  else if (posicio_anterior == Desconegut && posicio_actual == Desconegut)
  {
    
    sendstring_ESP01("######## CHECK : SITUACIO EXTRANYA ########");
    //serial_println("######## CHECK : SITUACIO EXTRANYA ########" );
  }
}

static void run_FSM(event_t event)
{
  static estat_t estat = s0;
  switch (estat) {
    case s0:
      if (event == Time_out) {

        posicio_anterior = update_posicio();

        //sendstring_ESP01( "Posicio anterior");
        //serial_println("Posicio anterior");


        if (posicio_anterior == Tumbat) { //DEBBUG
          sendstring_ESP01("Tumbat"  );
          //serial_println( "Tumbat" );
        }

        else if (posicio_anterior = Erecte) {
          sendstring_ESP01( "Erecte" );
          //serial_println( "Erecte"  );
        }

        else {
          sendstring_ESP01("Desconegut" );
          //serial_println( "Desconegut");
        }
      }

      else if (event == Threshold)
      {
        estat = s1;

        sendstring_ESP01("Threshold");
        //serial_println("Threshold");
      }
      break;

    case s1:
      if (event == Time_out)
      {
        estat = s0;
        posicio_actual = update_posicio();

       //sendstring_ESP01( "POSICIO POSTERIOR");
        //serial_println("POSICIO POSTERIOR");
        
        if (posicio_actual == Tumbat) { //DEBBUG

          sendstring_ESP01( "Tumbat" );
          //serial_println( "Tumbat" );
        }
        else if (posicio_actual = Erecte) {

          sendstring_ESP01("Erecte");
          //serial_println("Erecte");
        }
        else {

          sendstring_ESP01("Desconegut");
          //serial_println("Desconegut");
        }

        check_fall();
      }
      break;
    default :
      break;
  }
}
