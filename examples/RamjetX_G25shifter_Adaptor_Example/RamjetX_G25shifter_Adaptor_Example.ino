/*
   The RamjetX G25 Shifter Library for Arduino Leonardo 32u4

   This project has used base code from http://www.isrtv.com/forums/topic/13189-diy-g25-shifter-interface-with-h-pattern-sequential-and-handbrake-modes/

   My work was simply to adapt and create the arduino library interface so that it was useable and configuarble with the
   Arduino environment.

   Alot of work when into creating a usable library and whilst I don't claim to have originally written the capture and
   decode work. Being a simple Bit-Shift register for the buttons and two Analog sticks for the shift gates, it did seem
   pointless to rewrite it as I'd essentially have written something very similar anyway.

   With that said, do pop by the link above and give the original author a big thanks for compiling and releasing his findings too.
   I have YouTube and forum place where I do embedded systems and simulation work also. Feel free to visit, say hi, and jump in a game
   with us.

   Find me at:
   https://www.youtube.com/channel/UCnJO2Gx_moUTu8oA-fNJXMA
   and if you'd like to contact me... RamjetX in discord. Find me lurking around the OZNZ or Simhub Discord channels

   Enjoy the Library. It is work in progress and subtle changes will likely occur to enhance functionality over time.

   RamjetX
*/

// RamjetX G25Shifter Library
#include <G25Shifter.h>
// RamjetX 16bit Joystick Pluggable HID Library for 1.6.6 IDE+
#include <Joystick.h>

/*
   Instructions:-

   Connect the G25 Shifter by calling the G25Shifter Library and giving it a name.

   Below I called my G25Shifter, simply G25... and it is referred to as G25 when you asking it to do stuff.

   The constructor wants you to enter the pins used for the following pins
   
   G25Shifter(int shifter_x, int shifter_y, int data_input_pin, int data_latch_pin, int data_clock_pin, int led1_pin)
*/

// Create Object G25
G25Shifter G25(A0, A1, 15, 14, 16, 3); // These are the pins I've decided to use on my board... you may change these to your liking

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   32, 0,                  // Button Count, Hat Switch Count
                   true, true, false,     // X and Y, but no Z Axis
                   false, false, false,   // No Rx, Ry, or Rz
                   false, false,          // No rudder or throttle
                   false, false, false);  // No accelerator, brake, or steering


void setup() {
  Joystick.begin();
  //Serial.begin(9600); // You can use this with the debug routine below to see the live data from the shifter
}


/*Main Program loop*/
void loop() {

  //Shifter is ready to update and get information from. Remember to update atleast once before checking the information received from it.
  G25.update();

  // Puts the Gear Selected into Button 1-7 for Gear 1,2,3,4,5,6,R. All lower bank are OFF in Neutral.
  int gear = G25.getGear();

  /*
  // Loop through all of the gear buttons. 
  // Set every button to zero that does not match the currently selected gear. 
  // Neutral is 0. Gears 1 to 6, 7 is reverse and sequential shift up is 8 - and shift down is 9
  // 
  */
  for (int i = 1; i < 10; i++) { // Loop through all of the gear buttons. Set every button to zero that does not match the currently selected gear. Gears 1 to 6, 7 for reverse Includes sequential gears 8 - 9
    if (i != gear) {
      Joystick.setButton(i, 0);
    }
    else {
      Joystick.setButton(i, 1);
    }
  }//end gear


  // Get the rest of the buttons from the shifter. Last button for gears is 9... so start at 10.
  
  for (int i = 10; i <= 18; i++) { // get the button values from 4 to 12. Note, G25 Buttons 0-3 are not physical buttons you can use, they are either Missing from the board or Reverse mode and Sequential switch. Not mapped to the game since you don't really have control over them.
    Joystick.setButton(i,G25.getButton(i));
  }

  //G25.print();    // Uncomment this if you want to print the serial debug information to the serial port. It's good for checking the X/Y values of the stick to tune the gate values if you have a dodgy stick.

  delay(10); // small delay just slows the refresh rate to help with flooding the PC with updates that aren't needed in a hurry from a shifter.

}// end loop
