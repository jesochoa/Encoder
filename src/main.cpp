/*
    No funciona bien si no tiene el codigo de antirrebote 
    ya que mi pote no tiene condensadores
*/

#include <Arduino.h>

volatile boolean TurnDetected;
volatile boolean up;

               
#define PinCLK   2        // Used for generating interrupts using CLK signal
#define PinDT    4        // Used for reading DT signal
#define PinSW    7        // Used for the push button switch

void isr ()  {                    // Interrupt service routine is executed when  CLK is LOW
  //rutina antirrebote
  static unsigned long UltimaInterrupcion = 0; //La declaro Static para que no se pierda el valor
  unsigned long TiempoInterrupcion = millis();
  if (TiempoInterrupcion - UltimaInterrupcion > 10 ){
    up = digitalRead(PinDT);
    TurnDetected = true;
  }
  UltimaInterrupcion = TiempoInterrupcion;//Actualizo el tiempo de la interrupcion
}


void setup ()  {
 pinMode(PinCLK,INPUT);
 pinMode(PinDT,INPUT);  
 pinMode(PinSW,INPUT_PULLUP);
 attachInterrupt (0,isr,LOW);   // interrupt 0 is always connected to pin 2 on Arduino UNO
 Serial.begin (9600);
 Serial.println("Start");
}

void loop ()  {
 static long virtualPosition=0;    // without STATIC it does not count correctly!!!

 if (!(digitalRead(PinSW))) {      // check if pushbutton is pressed
   virtualPosition=0;              // if YES, then reset counter to ZERO
   Serial.print ("Reset = ");      // Using the word RESET instead of COUNT here to find out a buggy encoder
   
   Serial.println (virtualPosition);
   while (!(digitalRead(PinSW))){} //No sale hasta que suelte el pulsador 
 }  
 
 if (TurnDetected)  {        // do this only if rotation was detected
   if (up)
     virtualPosition += 10;
   else
     virtualPosition--;
   TurnDetected = false;          // do NOT repeat IF loop until new rotation detected
   Serial.print ("Count = ");  
   Serial.println (virtualPosition);
 }
}
