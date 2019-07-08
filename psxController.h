/*  PSX Controller Library (PsxController.h)
  
    By Danjovic, 2019 

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


#ifndef psxController_h
#define psxController_h

#include "Arduino.h"

#define defaultDelay 10   // microseconds


typedef union{
	
	uint16_t all;
	
    struct { // buttons
        // first data byte 
        uint8_t LEFT   : 1;
        uint8_t DOWN   : 1;
        uint8_t RIGHT   : 1;        
        uint8_t UP     : 1;  
        uint8_t START  : 1;
        uint8_t L3     : 1;
        uint8_t R3     : 1;     
        uint8_t SELECT : 1;   
            
        // second data byte
        uint8_t SQUARE   : 1;
        uint8_t CROSS    : 1;
        uint8_t CIRCLE   : 1;
        uint8_t TRIANGLE : 1;
        uint8_t R1       : 1;
        uint8_t L1       : 1;
        uint8_t R2       : 1;
        uint8_t L2       : 1;
	} ;	
	
} buttons_t;

typedef union {
    uint8_t raw[4];

	struct { // axes
        // 3rd-6th data byte
        int8_t rX; // Right X
        int8_t rY; // Right Y
        int8_t lX; // Left X
        int8_t lY; // Left Y         
    };      
} axes_t;


typedef union{
    // 6 bytes we get from the controller
    uint8_t rawData[6];
    
    struct {
		buttons_t buttons;
		axes_t    axes;
    };
} psxReport_t;




class psxController
{
	public:
		psxController(const uint8_t dat,const uint8_t cmd,const uint8_t att,const uint8_t clk);
		void setDelay (uint8_t delay);	
		bool read(void);
		uint8_t controllerID;
		psxReport_t report;
		

	private:
		uint8_t shift(uint8_t  _dataOut);

		uint8_t _dataPin;
		uint8_t _cmndPin;
		uint8_t _attPin;
		uint8_t _clockPin;		
		uint8_t _delay;
		
		uint8_t _i;
		uint8_t _dataIn;
		uint8_t _temp;
};


#endif
