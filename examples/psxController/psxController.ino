/*  PSX Controller Library example file

  PSX Controller Library by Danjovic, 2019 

  Based on PSX Controller Decoder Library (Psx.h)
  Written by: Kevin Ahrendt June 22nd, 2008
  
  Controller protocol implemented using Andrew J McCubbin's analysis.
  http://www.gamesx.com/controldata/psxcont/psxcont.htm
  
  Shift command is based on tutorial examples for ShiftIn and ShiftOut
  functions both written by Carlyn Maw and Tom Igoe
  http://www.arduino.cc/en/Tutorial/ShiftIn
  http://www.arduino.cc/en/Tutorial/ShiftOut

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <psxController.h>      // Includes the Psx Library 
#define dataPin   8             // Any pins can be used 
#define cmndPin   9
#define attPin   11
#define clockPin 10

// Initialize the library
psxController PSX (dataPin, cmndPin, attPin, clockPin);

//
// WARNING: Serial print is RAM hungry. The library itself takes less than 30 bytes of RAM.
//
void setup()
{  
 Serial.begin(9600);
 Serial.println ("PSX Controller Library Test Program");
}

void loop()
{
  Serial.print("Requesting controller data: ");
  if (PSX.read()) {
   Serial.print("ID can be used to test for specific controllers. ID=");
   Serial.println(PSX.controllerID,HEX);
   Serial.println("\nTest buttons is very simple: ");
   Serial.print ("TRIANGLE: "); Serial.print(PSX.report.buttons.TRIANGLE);
   Serial.print ("CIRCLE: "); Serial.print(PSX.report.buttons.CIRCLE);   
   Serial.print ("CROSS: "); Serial.print(PSX.report.buttons.CROSS);   
   Serial.print ("SQUARE: "); Serial.print(PSX.report.buttons.SQUARE); 
/* Available buttons to test
   UP       TRIANGLE   L1   START      
   DOWN     CIRCLE     L2   SELECT
   LEFT     CROSS      R1   R3
   RIGHT    SQUARE     R2   L3
*/
   
   Serial.println("\nAs well as read axes");
   Serial.print ("Right Analog X axis: "); Serial.println(PSX.report.axes.rX);
   Serial.print ("Right Analog Y axis: "); Serial.println(PSX.report.axes.rY);
   Serial.print ("Left  Analog X axis: "); Serial.println(PSX.report.axes.lX);
   Serial.print ("Left  Analog Y axis: "); Serial.println(PSX.report.axes.lY);
 
   Serial.println("\n\nButton data can be also be read as a raw binary");
   Serial.print("Raw buttons data: ");
   Serial.println(PSX.report.buttons.all,BIN);

   Serial.println("\nAxes can also be read as raw data");
   Serial.print  ("Raw axes:" );
   Serial.print  (PSX.report.axes.raw[0]); 
   Serial.print  (PSX.report.axes.raw[1]); 
   Serial.print  (PSX.report.axes.raw[2]); 
   Serial.println(PSX.report.axes.raw[3]);   

    
    } else {
       Serial.print("fail... ID=");
       Serial.println(PSX.controllerID,HEX);
       Serial.println("Setting a different delay and trying again...");
       PSX.setDelay(50);  // delay value in microsseconds
      }
  delay (100);
      
}
