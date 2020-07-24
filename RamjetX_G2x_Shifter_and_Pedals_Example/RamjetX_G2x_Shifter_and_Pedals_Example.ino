/* Tutorial Sketch for G2x Shifter and Pedals
    RamjetX 12/04/2020

  Change Log
  V0.1 - Basic setup
  V0.2 - G25 Shifter Working
  V0.3 - Pedals Working including Variable Brake Pressure Adjustment. Setup is in each tab
  V0.4 - G27 Shifter working... Check the Pinout in the comments for details.
  V0.5 - Added definitions to allow you to toggle if you want Brake force adjustment or not.
  V0.6 - Modified the library to make things cleaner for the Sequential Shifter
  V0.7 - Added Gamepad_Button_Index to the Shifter code to automatically increment and assign the next button on the gamepad. Maybe easier to read?
          Atleast it tracks the buttons that has been assigned better and you don't have to think about it in the for loops now...
  V0.8 - Reworked Handbrake mode
          - Uses Left and Center Right for HandBrake Mode, and Right and Center Left for Shifter Mode
          - Added Deadzone for handbrake.
*/

/*
 * Libraries being used
 */
#include <Joystick.h>
#include <G25Shifter.h>

/*
   Definitions to toggle options in the code
*/
//#define debug // Uncomment this line if you want Serial to be enabled
//#define brakeForceEnable  // Uncomment this line if you are using an extra pot for brakeforce adjustment

/*
 * Setup the Joystick options you want. 
 */
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   32, 1,                        // Button Count, Hat Switch Count
                   false, true, true,    // X, Y, Z  ...Axis
                   false, false, false,    // Rx, Ry, Rz  ... more Axis
                   false, true,              // rudder, throttle  ... even more Axis
                   false, true, false);   // accelerator, brake, steering ... More Axis than WW2!

/*                   
 *                    
 */
void setup() {
#ifdef debug
  // put your setup code here, to run once:
  Serial.begin(9600);
#endif
  pedal_Setup();  // Additional Setup in the pedal tab
  g2x_Shifter_Setup();  // Additional Setup in the Shifter tab
  buttons_Setup();  // Additional Setup in the Buttons tab
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1);
  get_G25_Shifter(); // everything about the shifter is done here
  get_Pedals(); // everything about the pedal is done here
  //get_Buttons(); // everything about the extra buttons is done here
  
}//end loop
