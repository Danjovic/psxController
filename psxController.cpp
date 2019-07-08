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

#include "psxController.h"

psxController::psxController(const uint8_t dat,const uint8_t cmd,const uint8_t att,const uint8_t clk) {
	pinMode(dat, INPUT);
	digitalWrite(dat, HIGH);	// Turn on internal pull-up

	pinMode(cmd, OUTPUT);

	pinMode(att, OUTPUT);
	digitalWrite(att, HIGH);	

	pinMode(clk, OUTPUT);
	digitalWrite(clk, HIGH);	

	_dataPin  = dat;
	_cmndPin  = cmd;	
	_attPin   = att;	
	_clockPin = clk;
	
	_delay    = defaultDelay;
	
	controllerID = 0xFF; 
    report.rawData[0] = 0; // buttons
    report.rawData[1] = 0;
    report.rawData[2] = 127; // rX  
    report.rawData[3] = 127; // rY
    report.rawData[4] = 127; // lX
    report.rawData[5] = 127; // lY		

}

void psxController::setDelay (uint8_t delay) {
	_delay = delay;	
	}



// Does the actual shifting, both in and out simultaneously
uint8_t psxController::shift(uint8_t  _dataOut)							
{
	_dataIn = 0;

	for (_i = 0; _i <= 7; _i++)
	{

		digitalWrite(_clockPin, (_dataOut & 1) ); // write lsBit
		_dataOut >>=1;                            // next Bit
/*		
		if ( _dataOut & (1 << _i) ) 
		   digitalWrite(_cmndPin, HIGH);	// Writes out the _dataOut bits
		else 
		   digitalWrite(_cmndPin, LOW);
*/
		digitalWrite(_clockPin, LOW);
		
		delayMicroseconds(_delay);  // take some time before reading the bits

		if (digitalRead(_dataPin))
		{
			_dataIn = _dataIn | (B10000000 >> _i);		// Shifts the read data into _dataIn
		}

		digitalWrite(_clockPin, HIGH);

		delayMicroseconds(_delay);
	}
	return _dataIn;
}



bool psxController::read(void) {

	digitalWrite(_attPin, LOW);

    // Initialize report
    report.rawData[0] = 0; // buttons
    report.rawData[1] = 0;
    report.rawData[2] = 127; // rX  
    report.rawData[3] = 127; // rY
    report.rawData[4] = 127; // lX
    report.rawData[5] = 127; // lY	

	shift(0x01);                // START command
	controllerID = shift(0x42); // REQUEST DATA command
	if ( shift(0xFF) != 0x5A) { // did "here comes the data" ? 
		digitalWrite(_attPin, HIGH);  // no, abort further reading
		return false;
		}     

    switch (controllerID) {
		case 0x41: // Digital 
		           report.rawData[0] = ~shift(0xFF);
                   report.rawData[1] = ~shift(0xFF);
		   //        report.rawData[2] = 127; // rX     TODO map UDLR to rX,rY
		   //        report.rawData[3] = 127; // rY
		   //        report.rawData[4] = 127; // lX
		   //        report.rawData[5] = 127; // lY
		           break;
		 case 0x23: //  NeGcon
		 case 0x73: //  Analog controller, Red mode  
		            report.rawData[0] = ~shift(0xFF);
                    report.rawData[1] = ~shift(0xFF);
		            report.rawData[2] = shift(0xFF);
		            report.rawData[3] = shift(0xFF);
		            report.rawData[4] = shift(0xFF);
		            report.rawData[5] = shift(0xFF);
		            break;
		 case 0x53: //  Analog controller, Green mode 	
		            report.rawData[0] = ~shift(0xFF) & ~((1<<0)|(1<<1)|(1<<2)); // mask unused bits
                    _temp             = ~shift(0xFF);
		            report.rawData[2] = shift(0xFF);
		            report.rawData[3] = shift(0xFF);
		            report.rawData[4] = shift(0xFF);
		            report.rawData[5] = shift(0xFF);
		                                                              // reorder buttons
		            report.rawData[1] = _temp & (1<<0)|(1<<5)|(1<<6); // kep bits 0,5,6
		            if (_temp & (1<<1) ) report.rawData[1] |= (1<<2); // L1 on bit1 go to bit 2
		            if (_temp & (1<<2) ) report.rawData[1] |= (1<<7); // [] on bit2 go to bit 7
		            if (_temp & (1<<3) ) report.rawData[1] |= (1<<4); // /\ on bit3 go to bit 4
		            if (_temp & (1<<4) ) report.rawData[1] |= (1<<3); // R1 on bit4 go to bit 3
		            if (_temp & (1<<7) ) report.rawData[1] |= (1<<1); // R2 on bit7 go to bit 1
		            break;
		            
		            
		 case 0x5A: //  PSX Mouse  	
                                         shift(0xFF); // discard first byte	
                    _temp             = ~shift(0xFF); // read L and R buttons
                    report.rawData[0] = 0; 
                    report.rawData[1] = 0;                     
		            if (_temp & (1<<1) ) report.rawData[1] |= (1<<5); // L on bit1 go to bit 5 ()
		            if (_temp & (1<<2) ) report.rawData[1] |= (1<<6); // R on bit2 go to bit 6 ><                                   
		            report.rawData[2] = shift(0xFF); // Delta X
		            report.rawData[3] = shift(0xFF); // Delta Y
		          //  report.rawData[4] = 127; 
		          //  report.rawData[5] = 127; 

		            break;
		           
		 Default:  	digitalWrite(_attPin, HIGH);
		            return false;
		}
	digitalWrite(_attPin, HIGH);
	return true;  // true se leu tudo
}
