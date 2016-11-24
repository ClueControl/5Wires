/*
  
/*

  5Wires- an Arduino Library for creating a "connect 5 wires" puzzle
  2016 Shawn Yates
  www.cluecontrol.com
  
  Want to automate this puzzle?  
  Check out our website and let us know if you need help!
  feedback@cluecontrol.com
  
  written and tested with Arduino 1.6.9
 
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

   -------------------

   The general idea is that to solve the puzzle, 5 specific paires of wires
   need to be connected.  If any pair is not connected correctly, the puzle
   is not solved.

*/

/*
// pin assignments
 * Signal     Pin                
 *            Arduino Uno     
 * ------------------------
 * 0        Serial RX
 * 1        Serial TX
 * 2        Ouptut 1
 * 3        Output 2
 * 4        SD crd SS if using Ethernet
 * 5        Output 3
 * 6        Output 4
 * 7        Output 5
 * 8        Input 1
 * 
 * 9        Input 2
 * 10         ChipSelect for Ethernet
 * 11         SPI MOSI for Ethernet
 * 12         SPI MISO for Ethernet
 * 13         SPI SCK  for Ethernet
 * 14  (A0)     Input 3
 * 15  (A1)     Input 4
 * 16  (A2)     Input 5

 
 
 The Ethernet Shield SPI bus (10,11,12,13) with 10 as select
 also uses 4 as select for the SD card on the ethernet shield
 high deselects for teh chip selects.  These will be reserved
 incase the unit is ever connected to ClueControl via the 
 ModBusTCP connection
 
*/

//Define the pin numbers

#define Input1     8   //coil numbers for the various drawers
#define Input2     9   //must be unique throughout all puzzles on this network
#define Input3     14
#define Input4     15
#define Input5     16

#define Output1      2
#define Output2      3
#define Output3      5
#define Output4      6
#define Output5      7


// Global variables and objects
bool Pair1Latch;
bool Pair2Latch;
bool Pair3Latch;
bool Pair4Latch;
bool Pair5Latch;

bool PairRead;
bool PuzzleSolved;
bool SolvedLatch;

void setup() {
	
	
	pinMode(Input1,INPUT_PULLUP);
	pinMode(Input2,INPUT_PULLUP);
	pinMode(Input3,INPUT_PULLUP);
	pinMode(Input4,INPUT_PULLUP);
	pinMode(Input5,INPUT_PULLUP);
	
	pinMode(Output1, OUTPUT);
	pinMode(Output2, OUTPUT);
	pinMode(Output3, OUTPUT);
	pinMode(Output4, OUTPUT);
	pinMode(Output5, OUTPUT);
	
	digitalWrite (Output1, HIGH);
	digitalWrite (Output2, HIGH);
	digitalWrite (Output3, HIGH);
	digitalWrite (Output4, HIGH);
	digitalWrite (Output5, HIGH);
	
	Serial.begin(9600);  // USB serial for debugging
	
}

void loop() {
	
	//check the state of each pair one at a time.  
	
	CheckPair (Input1, Output1, Pair1Latch);
	CheckPair (Input2, Output2, Pair2Latch);
	CheckPair (Input3, Output3, Pair3Latch);
	CheckPair (Input4, Output4, Pair4Latch);
	CheckPair (Input5, Output5, Pair5Latch);
	
	
	//check the new value of the puzzle solved criteria
	PuzzleSolved = (Pair1Latch && Pair2Latch && Pair3Latch && Pair4Latch && Pair5Latch);
	
	//If the latch and the new value are not equal, take action
	if (SolvedLatch != PuzzleSolved){
		
		if (PuzzleSolved){
			//decide what to do if the puzzle is solved
			Serial.println("The puzzle has been solved!");
		} else {
			
			//decide what to do if the puzzle becomes unsolved
			Serial.println("The puzzle is no longer solved");
		}
		
		SolvedLatch = PuzzleSolved;
		
	}
	
}

void CheckPair (byte InputPin, byte OutputPin, bool& PairLatch){
	
	//notice that the PairLatch is passed by ref, that way we can update the value of the latch
	
	digitalWrite (OutputPin, LOW);            //drive the output pin low, then see if that low is felt on the 
	//corrosponding input pin
	
	//when the wire is connected, the input will be LOW,
	//the latch holds the answer to the question "Is this pair connected"
	
	PairRead = not digitalRead(InputPin);     // read the pin, invert it because it has an internal pull-up
	if (PairRead != PairLatch)            // now see if the trigger value has changed.  if it has, take actions as needed
	{
		Serial.print("Input Pin# ");
		Serial.print(InputPin);
		Serial.print(" Connected: ");
		Serial.println(PairRead);
		
		PairLatch = PairRead;                  //save the new value
	}
	digitalWrite (OutputPin, HIGH);           //put the output pin back to high
	
}


