/* 

G25Shifter Source code by RamjetX
New Library for handling the G25 and G27 Shifter

*/
#include "Arduino.h"
#include "G25Shifter.h"

//initialize variables used for logic processing

// Gear
int gear = 0;

// Shifter state
int shift = NO_SHIFT;

// Handbrake mode
int shifterMode = SHIFTER_MODE;

// I/O Pins being used to communicate with the Shifter
int shifter_x;
int shifter_y;
int data_input_pin;
int data_latch_pin;
int data_clock_pin;
int led1_pin;


// Analog Axis
int shifter_x_Value;
int shifter_y_Value;

// Array that stores all the button values pulled in by the shift register.
int buttons_Value_Array[16];

// Constructor to make a G25 Shifter object 
G25Shifter::G25Shifter(int G25_shifter_x, int G25_shifter_y, int G25_data_input_pin, int G25_data_latch_pin, int G25_data_clock_pin, int G25_led1_pin)
{
// Populate the Pins being used
	shifter_x = G25_shifter_x;
	shifter_y = G25_shifter_y;
	data_input_pin = G25_data_input_pin;
	data_latch_pin = G25_data_latch_pin;
	data_clock_pin = G25_data_clock_pin ;
	led1_pin = G25_led1_pin;

	
	// G25 shifter analog inputs configuration
	pinMode(shifter_x, INPUT);   // stick X axis
	pinMode(shifter_y, INPUT);   // stick Y axis

	// G25 button shift register 
	pinMode(data_input_pin, INPUT);     // Data in
	pinMode(data_latch_pin, OUTPUT);    // Parallel/serial latch mode
	pinMode(data_clock_pin, OUTPUT);    // Clock

	// LED output mode configuration
	pinMode(led1_pin, OUTPUT);            // LED

	// Digital outputs initialization
	digitalWrite(led1_pin, LOW);			// Light up the LED
	digitalWrite(data_clock_pin, HIGH);		// Send the first clock edge to the button shift register 
	digitalWrite(data_latch_pin, HIGH);		// High allows serial read of all the buttons in the shift register



};// end constructor



/*Function: getButton()

Returns value of the Button Shift Register
params:
int button
returns the True/False of that button
*/
bool G25Shifter::getButton(int button){
	//update(); //Refresh the current buttons array
	return buttons_Value_Array[button];
}// end getButton


/*Function getAllButtons()

Returns an Int containing the value of the ALL the Buttons in the Shift Register
params: none
returns: Int value of the whole 16bits of buttons.
*/
int G25Shifter::getAllButtons(){
	//update(); //Refresh the current buttons array
	int buttonValue = 0;
	for (int i = 0; i < 16; i++)         // Iteration over all 16 buttons
	{
		buttonValue += (buttons_Value_Array[i] << i );
	}
	return buttonValue;
}// end getAllButtons


/*Function getGear
	returns current gear
*/
int G25Shifter::getGear(){
	if (buttons_Value_Array[DI_MODE] == 0){
		return getHShifter();
	}
	
	if (buttons_Value_Array[DI_MODE] == 1){
		return getSeqShifter();
	}
	return 1;
}


/*Function getShifterMode
	returns 0 H-Shifter or 1 Sequential  
*/
int G25Shifter::getShifterMode(){
	return buttons_Value_Array[DI_MODE]; //Returns the current Shifter Mode
}


/*Function getBrake
	returns 1 - not yet implemented
*/
int G25Shifter::getBrake(){
	return 1;
}


/*Function readButtons()
	Populates the buttons_Value_Array[] with digitalReads from the Shift Register
	- Adapted from example.
*/
void G25Shifter::readButtons(){
  digitalWrite(data_latch_pin, LOW);         // Switch to parallel mode: digital inputs are read into shift register
  delayMicroseconds(10);               		 // Wait for signal to settle
  digitalWrite(data_latch_pin, HIGH);        // Switch to serial mode: one data bit is output on each clock falling edge

  // Read all the buttons from the G25 Button Shift Register.
  for (int i = 0; i < 16; i++)         		// Iteration over both 8 bit registers
  {
    digitalWrite(data_clock_pin, LOW);      // Generate clock falling edge
    delayMicroseconds(10);             		// Wait for signal to settle
	
	// Read data bit and store it into bit array
    buttons_Value_Array[i] = digitalRead(data_input_pin);   
	
	// button read, end the clock cycle and prepare to read the next shift register value
    digitalWrite(data_clock_pin, HIGH);     // Generate clock rising edge
    delayMicroseconds(10);             		// Wait for signal to settle
  }
  return;
}


/*Function checkHandbrakeMode
	Looks for combination of keypresses on shifter to convert the stick movement
	in squential mode into a analog handbrake
*/
void G25Shifter::checkHandbrakeMode(){
// Custom combo to activate Handbrake mode logic
// To de/activate see combo here


  if (buttons_Value_Array[DI_RED_CENTERLEFT] != 0)      // Is left center red button depressed?
  {
    if (buttons_Value_Array[DI_RED_CENTERRIGHT] != 0)   // Is right center red button also depressed?
    {
      if (buttons_Value_Array[DI_RED_RIGHT] != 0)       // Is rightmost red button also depressed?
      {
        shifterMode = HANDBRAKE_MODE;         			// Handbrake mode is activated if the 3 rightmost red buttons are depressed
      }
      if (buttons_Value_Array[DI_RED_LEFT] != 0)        // Is leftmost red button also depressed?
      {
        shifterMode = SHIFTER_MODE;          			// Handbrake mode is deactivated if the 3 leftmost red buttons are depressed
      }	
    }
  }
}// end checkHandbrakeMode


/*Function getHShifter
	Determines and returns the gear selected by the H-Shifter
	- Adapted from example.
	
	H-shifter 0 is Netural
	Gears 
*/
int G25Shifter::getHShifter(){

// Current gear calculation
  gear = 0;                        // Default value is neutral
  
//*********** H Shifter Mode ************
  if (buttons_Value_Array[DI_MODE] == 0)       // H-shifter mode? 0 = H-Shifter. 1 = Sequential Shifter
  {	// H-Pattern
	if (shifter_x_Value < HS_XAXIS_12)               // Shifter on the left?
	{
	  if (shifter_y_Value > HS_YAXIS_135) gear = 1;  // 1st gear	// forward
	  if (shifter_y_Value < HS_YAXIS_246) gear = 2;  // 2nd gear	// back
	}// end on left
	
	else if (shifter_x_Value > HS_XAXIS_56)          // Shifter on the right?
	{
	  if (shifter_y_Value > HS_YAXIS_135) gear = 5;  // 5th gear	//forward
	  
	  if (shifter_y_Value < HS_YAXIS_246){
		gear = 6;  									// 6th gear		//back
		if (buttons_Value_Array[DI_REVERSE]){		// stick is in 6th/Reverse location. Check which to apply.
			gear = 7; 								// 7th Gear or Reverse
		}
	  }
	}// end on right
	
	else                               // Shifter is in the middle
	{
	  if (shifter_y_Value > HS_YAXIS_135) gear = 3;  // 3rd gear
	  if (shifter_y_Value < HS_YAXIS_246) gear = 4;  // 4th gear
	}// end in middle
  }
  return gear;
}// end getHShifter


/*Function getSeqShifter
returns -1 Downshift,0 No shift, 1 shift
*/
int G25Shifter::getSeqShifter(){
gear = 0;
if (buttons_Value_Array[DI_MODE] == 1)       // H-shifter mode? 0 = H-Shifter. 1 = Sequential Shifter
    if (shifterMode == SHIFTER_MODE)          // Shifter mode or in Handbrake Mode
    {
	
      if (shift == NO_SHIFT)           // Current state: no shift
      {
        if (shifter_y_Value > SS_UPSHIFT_BEGIN)      // Shifter to the front?
        {
          gear = 8;                    // Shift-up
          shift = UP_SHIFT;            // New state: up-shift
        }
        if (shifter_y_Value < SS_DOWNSHIFT_BEGIN)    // Shifter to the rear?
        {
          gear = 9;                    // Shift-down
          shift = DOWN_SHIFT;          // New state: down-shift
        }
      }//
	  
      if (shift == UP_SHIFT)           // Current state: up-shift?
      {
        if (shifter_y_Value > SS_UPSHIFT_END)
		{
			gear = 8; // Beyond lower up-shift threshold: up-shift
        }
		else{
		shift = NO_SHIFT;         // Else new state: no shift
		}
      }//
	  
      if (shift == DOWN_SHIFT)         // Current state: down-shift
      {
        if (shifter_y_Value < SS_DOWNSHIFT_END)
		{
			gear = 9; // Below higher down-shift threshold: down-shift
        }
		else{ 
		shift = NO_SHIFT;                // Else new state: no shift
		}
	  }//
	  
    }
	return gear;
}// end getSeqShifter


// To be worked on... returning the raw value of Y at this stage centers at half and isn't mapped correctly.
int G25Shifter::getHandbrake(){
	if (shifterMode == HANDBRAKE_MODE){
		return shifter_y_Value;
	}
}// end getHandbrake


/*Function calibrateShifter() 
	- Calibrates Stick for gear gate thresholds
	Not yet implemented. Currently returning -1 to indicate error.
*/
int G25Shifter::calibrateShifter(){
	return -1;
}

/*Function update - Refreshes all the buttons and axis
	Updates all the variables reading from the Shifter
	Perform this before asking for new data.
*/
void G25Shifter::update(){
	shifter_x_Value = analogRead(shifter_x);
	shifter_y_Value = analogRead(shifter_y);
	readButtons();
}

/*Function print
	- Serial prints the current debug values for the X/Y axis
	  and all the buttons.
*/
void G25Shifter::print(){
	Serial.print("Shifter X = ");
	Serial.println(shifter_x_Value);
	Serial.print("Shifter Y = ");
	Serial.println(shifter_y_Value);
	Serial.print("Buttons = ");
	for (int i=0; i<16; i++){
		Serial.print(getButton(i));
    }
	Serial.println("");
	Serial.print("H-Shifter Gear = ");
	Serial.println(getHShifter());
	Serial.print("Sequential-Shifter = ");
	Serial.println(getSeqShifter());
	Serial.print("Gear Function = ");
	Serial.println(getGear());
	Serial.println("");
	
}


void G25Shifter::G25Setled1(bool led1)
{
  digitalWrite(led1_pin,led1);
  
  //Blink the on-board LED
  //if(++led==100) led=0;                     // Period is 100 cycles * 10ms = 1s
  //if(led==0) digitalWrite(LED_PIN, LOW);    // LED is off for 50 cycles
  //if(led==50) digitalWrite(LED_PIN, HIGH);  // LED is on for 50 cycles

}