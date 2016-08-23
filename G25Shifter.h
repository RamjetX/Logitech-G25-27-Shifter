// G25 Shifter Library By RamjetX

#ifndef G25Shifter_h
#define G25Shifter_h

// H-shifter mode analog axis thresholds
#define HS_XAXIS_12        450	// Stick left for gears 1 and 2 (was 400)
#define HS_XAXIS_56        680	// Stick right for gears 5 and 6
#define HS_YAXIS_135       750	// Stick forward for gears 1, 3, and 5
#define HS_YAXIS_246       300	// Stock backward for gears 2,4,6 and Reverse

// Sequential shifter mode analog axis thresholds
#define SS_UPSHIFT_BEGIN   600
#define SS_UPSHIFT_END     600
#define SS_DOWNSHIFT_BEGIN 410
#define SS_DOWNSHIFT_END   410

// Handbrake mode analog axis limits
#define HB_MAXIMUM         530
#define HB_MINIMUM         400
#define HB_RANGE           (HB_MAXIMUM-HB_MINIMUM)

// Digital inputs definitions
#define DI_REVERSE         1
#define DI_MODE            3
#define DI_RED_CENTERRIGHT 4
#define DI_RED_CENTERLEFT  5
#define DI_RED_RIGHT       6
#define DI_RED_LEFT        7
#define DI_BLACK_TOP       8
#define DI_BLACK_RIGHT     9
#define DI_BLACK_LEFT      10
#define DI_BLACK_BOTTOM    11
#define DI_DPAD_RIGHT      12
#define DI_DPAD_LEFT       13
#define DI_DPAD_BOTTOM     14
#define DI_DPAD_TOP        15

// Shifter state
#define DOWN_SHIFT         -1
#define NO_SHIFT           0
#define UP_SHIFT           1

// Shifter mode
#define SHIFTER_MODE       0
#define HANDBRAKE_MODE     1

#include "Arduino.h"

class G25Shifter
{
	public:
		G25Shifter(int shifter_x, int shifter_y, int data_input_pin, int data_latch_pin, int data_clock_pin, int led1_pin); // constructor
		bool getButton(int button);	// returns True/False of that button asked for
		int getAllButtons(); // returns the int value of ALL the buttons current state
		int getGear();	// return current gear
		int getBrake();	// returns 1
		int getShifterMode();	//returns if H-Shifter or Squential Shifter
		int getHShifter();	// returns gear from a H-Shifter
		int getSeqShifter(); // returns -1,0,1 for shift down, no shift, shift up
		int getHandbrake();	// returns the current Y axis of the shift handle for analog handbrake
		int calibrateShifter(); // Simple analog threshold calibration for the stick gear gates.
		void update();	// updates all of the information from the G25.
		void print(); // Prints all the debug values to serial port
		void G25Setled1(bool led1); // Sets the LED's on the G25
		
	private:
		
		void readButtons();
		void checkHandbrakeMode();
		

};

#endif