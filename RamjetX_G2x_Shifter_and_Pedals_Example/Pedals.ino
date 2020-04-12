#define throttle_Pin A0 //DB9 pin 2
#define clutch_Pin A1   //DB9 pin 3
#define brake_Pin A2    //DB9 pin 4
#define brake_Adjust_Pin A3

int throttle, brake, clutch, brake_Offset, brake_Adjusted ; // Just creating some global variables for these values...

void pedal_Setup() {
  pinMode(throttle_Pin, INPUT); // Throttle
  pinMode(clutch_Pin, INPUT); // Clutch
  pinMode(brake_Pin, INPUT); // Brake
  pinMode(brake_Adjust_Pin, INPUT); // Brake Force Adjust Pot   *** <------- NEW use a 10K linear Pot ************

  Joystick.setBrake(0);
  Joystick.setZAxis(0);
  Joystick.setThrottle(0);

}// end pedal_Setup

void get_Pedals() {

  // Brake
  brake = analogRead(brake_Pin);
#ifdef brakeForceEnable
  brake_Offset = analogRead(brake_Adjust_Pin) / 2; // This will get the value of the brake force adjust pot, divide it by 2, and scale back the brake pedal to a maximum half of normal range
  brake_Adjusted = map(brake, 0, 1023, 0, 1023 - brake_Offset); // Note: The maximum range the brake pedal will report is controlled by the offset from the adjustment pot. Make sure this is set to maximum when calibrating in windows!!!!
  Joystick.setBrake(constrain(brake_Adjusted, 0, 1023)); // I constrain the value before I send it... makes sure it can't accidently overflow and do weird braking stuff.
#else
  brake_Adjusted = map(brake, 0, 1023, 0 , 1023); // Note: The maximum range the brake pedal will report is controlled by the offset from the adjustment pot. Make sure this is set to maximum when calibrating in windows!!!!
  Joystick.setBrake(constrain(brake, 0, 1023)); // I constrain the value before I send it... makes sure it can't accidently overflow and do weird braking stuff.
#endif


  // Clutch
  clutch = analogRead(clutch_Pin);
  clutch = map(clutch, 0, 1023, 0 , 1023); // If you need to invert the output... swap the last two 0 , 1023 values... it'll remap inverted.
  clutch = constrain(clutch, 0, 1023);
  Joystick.setZAxis(clutch);

  // Throttle
  throttle = analogRead(throttle_Pin);
  throttle = map(throttle, 0, 1023, 0 , 1023); // If you need to invert the output... swap the last two 0 , 1023 values... it'll remap inverted.
  throttle = constrain(throttle, 0, 1023);
  Joystick.setThrottle(throttle);

}// end get_Pedals
