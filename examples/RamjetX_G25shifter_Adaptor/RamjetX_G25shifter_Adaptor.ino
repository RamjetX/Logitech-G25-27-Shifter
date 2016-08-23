/*
 * The RamjetX G25 Shifter Library for Arduino Leonardo 32u4
 * 
 * This project has used base code from http://www.isrtv.com/forums/topic/13189-diy-g25-shifter-interface-with-h-pattern-sequential-and-handbrake-modes/
 * 
 * My work was simply to adapt and create the arduino library interface so that it was useable and configuarble with the
 * Arduino environment.
 * 
 * Alot of work when into creating a usable library and whilst I don't claim to have originally written the capture and 
 * decode work. Being a simple Bit-Shift register for the buttons and two Analog sticks for the shift gates, it did seem 
 * pointless to rewrite it as I'd essentially have written something very similar anyway.
 * 
 * With that said, do pop by the link above and give the original author a big thanks for compiling and releasing his findings too.
 * I have YouTube and forum place where I do embedded systems and simulation work also. Feel free to visit, say hi, and jump in a game
 * with us.
 * 
 * Find me at:
 * https://www.youtube.com/channel/UCnJO2Gx_moUTu8oA-fNJXMA
 * and my forum place of dev work
 * http://oznzsimracing.com/forums/m/16887284/viewthread/23526371-ramjetx-simrig-other-stuff
 * 
 * Enjoy the Library. It is work in progress and subtle changes will likely occur to enhance functionality over time.
 * 
 * RamjetX
 */

// RamjetX G25Shifter Library
#include <G25Shifter.h>
// RamjetX 16bit Joystick Pluggable HID Library for 1.6.6 IDE+
#include <Joystick.h>

/*
 * Instructions:-
 * 
 * Connect the G25 Shifter by calling the G25Shifter Library and giving it a name. 
 * 
 * Below I called my G25Shifter, simply G25... and it is referred to as G25 when you asking it to do stuff.
 * 
 * The constructor wants you to enter the pins used for the following pins
 * G25Shifter(int shifter_x, int shifter_y, int data_input_pin, int data_latch_pin, int data_clock_pin, int led1_pin)
 */

// Create Object G25 
G25Shifter G25(A0, A1, 15, 14, 16, 3);

void setup() {
  /*
   * Initialise the My pluggable HID Joystick with basic 0 values. 
   * 
   * These are the available axis and buttons you can use to do stuff with.
   */
  Joystick.joySt.xAxis = 0;
  Joystick.joySt.yAxis = 0;
  Joystick.joySt.zAxis = 0;
  Joystick.joySt.xRotAxis = 0;
  Joystick.joySt.yRotAxis = 0;
  Joystick.joySt.zRotAxis = 0;
  Joystick.joySt.throttle = 0;
  Joystick.joySt.rudder = 0;
  Joystick.joySt.hatSw1 = 9;
  Joystick.joySt.hatSw2 = 9;  
  Joystick.joySt.buttonsLower = 0; // Buttons 0-15
  Joystick.joySt.buttonsUpper = 0; // Buttons 16-32
}


/*Main Program loop*/
void loop() {

  //new Shifter is ready to update and get information from. Remember to update atleast once before checking the information received from it.
  G25.update();

  // Clears the button values ready to be populated again below.
  Joystick.joySt.buttonsLower = 0; // PC Buttons 1-15
  Joystick.joySt.buttonsUpper = 0; // PC Buttons 16-32

  // Puts the Gear Selected into Button 1-7 for Gear 1,2,3,4,5,6,R. All lower bank are OFF in Neutral. (Buttons 8-15 aren't used)
  if (G25.getGear()) {  // Check to see if the gear selected isn't 0, i.e. neutral.
    Joystick.joySt.buttonsLower = (1 << (G25.getGear() - 1)); // getGear returns the "actual" gear. So subtract 1 from the gear value when placing the Binary mode Bit into the Joystick.joyStick.Joystick.joySt.buttonsLower register.
  }

  // Get the rest of the buttons from the shifter. 
  for (int i = 4; i < 16; i++) { // get the button values from 4 to 12. Note, Buttons 0-3 are not physical buttons you can use, they are either Missing from the board or Reverse mode and Sequential switch. Not mapped to the game since you don't really have control over them.
    Joystick.joySt.buttonsUpper = Joystick.joySt.buttonsUpper + (G25.getButton(i) << (i - 4)); // (i-4) will push all the buttons into the bank nicely and offset the fact we don't use the first 4
  }

  // This statement then sends the PC the whole Joystick state update... ie, refreshes Windows of the new values.
  Joystick.setState(&Joystick.joySt);
  
  //G25.print();    // Uncomment this if you want to print the serial debug information to the serial port. It's good for checking the X/Y values of the stick to tune the gate values if you have a dodgy stick.
  
  delay(10); // small delay just slows the refresh rate to help with flooding the PC with updates that aren't needed in a hurry from a shifter.

}// end loop
