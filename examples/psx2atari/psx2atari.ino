/*  PSX Controller Library example file

  PSX Controller to atari converter
  Danjovic, 2019 

  This program converts a Playstation Digital or Dualshock controller
  into an Atari Controller with Boost Grip

  Funcion Mapping:
  PSX Controller   Atari
  D-PAD            UP/DOWN/LEFT/RIGHT
  Left Analog      UP/DOWN/LEFT/RIGHT
  X (cross)        FIRE button
  () (circle)      THUMB button (boost grip)
  R1               TRIGGER button (boost grip)
  
Pinout:
Arduino(/AVR)
 3/PD3  Dualshock ATT 
 4/PD4  Dualshock CLK
 5/PD5  Dualshock CMD   
 6/PD6  Dualshock DATA   

 
 8/PB0  atari UP    (output)           
 9/PB1  atari DOWN  (output)             
10/PB2  atari LEFT  (output)             
11/PB3  atari RIGHT (output)             
12/PB4  atari FIRE  (output)            
13/PB5  atari TRGB  (output)             

*/

#include <psxController.h>      // Includes the Psx Library 

const int attPin = 3; 
const int clkPin = 4;
const int cmdPin = 5;
const int datPin = 6;

const int atariFire    =  7; 
const int atariUp      =  8;          
const int atariDown    =  9;          
const int atariLeft    = 10;
const int atariRight   = 11;
const int atariTrigger = 12;
const int atariThumb   = 13; 


// Initialize the library
psxController psx (datPin, cmdPin, attPin, clkPin);

#define THRESHOLD 32  // threshold around center analog sticks


void setup() {
  atariDisconnect(); // disable all pins going to Atari
}

 

void loop() {
  if (psx.read()) {

    //// D-Pad
    if (psx.report.buttons.UP) {    // D-pad UP  
      pinMode(atariUp, OUTPUT); 
      digitalWrite(atariUp,LOW);
      } else pinMode(atariUp,INPUT_PULLUP);

    if (psx.report.buttons.DOWN) {    // D-pad DOWN
      pinMode(atariDown, OUTPUT); 
      digitalWrite(atariDown,LOW);
      } else pinMode(atariDown,INPUT_PULLUP);

    if (psx.report.buttons.LEFT) {    // D-pad LEFT  
      pinMode(atariLeft, OUTPUT); 
      digitalWrite(atariLeft,LOW);
      } else pinMode(atariLeft,INPUT_PULLUP);

    if (psx.report.buttons.RIGHT) {    // D-pad RIGHT
      pinMode(atariRight, OUTPUT); 
      digitalWrite(atariRight,LOW);
      } else pinMode(atariRight,INPUT_PULLUP);


    //// Left Analog as DPAD
    if (psx.report.axes.lX > (127 + THRESHOLD)) {    // Left Analog X axis
      pinMode(atariRight, OUTPUT); 
      digitalWrite(atariRight,LOW);
      } 

    if (psx.report.axes.lX < (127 - THRESHOLD)) {    // Left Analog X axis
      pinMode(atariLeft, OUTPUT); 
      digitalWrite(atariLeft,LOW);
      } 

    if (psx.report.axes.lY > (127 + THRESHOLD)) {    // Left Analog Y axis
      pinMode(atariDown, OUTPUT); 
      digitalWrite(atariDown,LOW);
      } 

    if (psx.report.axes.lY < (127 - THRESHOLD)) {    // Left Analog Y axis
      pinMode(atariUp, OUTPUT); 
      digitalWrite(atariUp,LOW);
      } 

    //// Buttons
    if (psx.report.buttons.CROSS) {    // Button X (cross) as FIRE  
      pinMode(atariFire, OUTPUT); 
      digitalWrite(atariFire,LOW);
      } else pinMode(atariUp,INPUT_PULLUP);

    if (psx.report.buttons.CIRCLE) {    // Button () (circle) as THUMB, active HIGH  
      digitalWrite(atariThumb,HIGH);
      } else digitalWrite(atariThumb,LOW);
      
    if (psx.report.buttons.R1) {    // Button R1 as Trigger, active HIGH  
      digitalWrite(atariTrigger,HIGH);
      } else digitalWrite(atariTrigger,LOW);

    // take some time before next sample
    delay(5); 
    
    } else {
      atariDisconnect();
      delay(100);
      }

}

void atariDisconnect() { // disable all pins going to Atari
  // Active Low pins
  pinMode(atariFire,    INPUT_PULLUP);
  pinMode(atariUp,      INPUT_PULLUP);
  pinMode(atariDown,    INPUT_PULLUP);
  pinMode(atariLeft,    INPUT_PULLUP);
  pinMode(atariRight,   INPUT_PULLUP);

  // Active HIGH pins
  pinMode(atariTrigger, OUTPUT); digitalWrite(atariTrigger,LOW);
  pinMode(atariThumb,   OUTPUT); digitalWrite(atariTrigger,LOW);     
  }
