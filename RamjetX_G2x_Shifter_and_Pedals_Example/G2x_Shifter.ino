/*
   Logitech Pedals G25 Shifter DB9 male Connector
   DB pin 1 = Loopback device detection
   DB pin 2 = shifter_Data_Input
   DB pin 3 = shifter_Data_Latch
   DB pin 4 = shifter_X
   DB pin 5 = not connected
   DB pin 6 = Ground Data
   DB pin 7 = shifter_Data_Clock
   DB pin 8 = shifter_Y
   DB pin 9 = VCC 5V <--------

   Logitech Pedals G27 Shifter DB9 male Connector
   DB pin 1 = shifter_Data_Clock
   DB pin 2 = shifter_Data_Input
   DB pin 3 = shifter_Data_Latch
   DB pin 4 = shifter_X
   DB pin 5 = not connected
   DB pin 6 = Ground Data
   DB pin 7 = Loopback device detection
   DB pin 8 = shifter_Y
   DB pin 9 = VCC 5V <--------
*/

// Connect a G2x Shifter - Class Header is: G25Shifter(int shifter_x, int shifter_y, int data_input_pin, int data_latch_pin, int data_clock_pin)
#define shifter_X_Pin A8
#define shifter_Y_Pin A9
#define shifter_Data_Input_Pin 5
#define shifter_Data_Latch_Pin 6
#define shifter_Data_Clock_Pin 7

G25Shifter G25(shifter_X_Pin, shifter_Y_Pin, shifter_Data_Input_Pin, shifter_Data_Latch_Pin, shifter_Data_Clock_Pin);


void g2x_Shifter_Setup() {
  Joystick.begin();
  Joystick.setYAxis(0);
}

/*
   Here is where all the good stuff happens on the shifter... it also assigns the Joystick buttons the right value.
*/
void get_G25_Shifter() {

  G25.update(); // Fresh data.... Yum!

#ifdef debug
  G25.print(); // Uncomment this line if you want to see the serial debug from the G25 library.
#endif

  /*
       This Gamepad_Button_Index is a temporary value that is used as each for loop assigned a button to the gamepad.
       It will be incremented as each button is assigned and be ready for the next button on the gamepad to be assigned.
       Note: The switch hat does not use this variable as it's assigned as a different 'type' of button.
  */
  int Gamepad_Button_Index = 0;


  //Set the Gear Position
  for (int i = 1; i < 10; i++) { // start looking for gears
    if (G25.getGear() == i) { // if gears 1 - 7 don't exist... it must be 0 or neutral
      Joystick.setButton(Gamepad_Button_Index, 1);
      Gamepad_Button_Index++; // Move to the next USB button
    }
    else {
      Joystick.setButton(Gamepad_Button_Index, 0); // Zero is neutral
      Gamepad_Button_Index++; // Move to the next USB button
    }
  }// End gears


  // Populate the Black and Red buttons
  for (int i = 4; i < 12; i++) { // first 4 buttons aren't used. They're for other options in the pcb
    Joystick.setButton(Gamepad_Button_Index, G25.getButton(i));
    Gamepad_Button_Index++;
  }// black and red buttons


  // Set the HatSwitches 0 to the Shifter Directional D - Pad ...
  int hatValue = 0; // make a single var to capture the position of all the hat direction buttons
  for (int i = 12; i < 16; i++) { // get the button values from 8 to 11 that are the D-Pad
    if (G25.getButton(i) == 1) {
      hatValue = hatValue + (1 << i - 11); //Shifting bit in by index minus 11... so 1,2,3,4 shifts. = 0,2,4,8,16 values
    }
    switch (hatValue) {
      case 2:
        Joystick.setHatSwitch(0, 90);
        break;
      case 8:
        Joystick.setHatSwitch(0, 180);
        break;
      case 4:
        Joystick.setHatSwitch(0, 270);
        break;
      case 16:
        Joystick.setHatSwitch(0, 360);
        break;
      default:
        Joystick.setHatSwitch(0, -1); //JOYSTICK_HATSWITCH_RELEASE
        break;
    }
  }// end getting the D-Pad buttons

  Joystick.setYAxis(G25.getHandbrake());

}// end get_G25_Shifter()
