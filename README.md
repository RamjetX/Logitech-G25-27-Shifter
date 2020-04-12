# Logitech-G25-27-Shifter
Logitech G25/27 Shifter library for Arduino 1.6.6+


   The RamjetX G25 Shifter Library for Arduino Leonardo 32u4

   This project has used base code from http://www.isrtv.com/forums/topic/13189-diy-g25-shifter-interface-with-h-pattern-sequential-and-handbrake-modes/

   My work was simply to adapt and create the arduino library interface so that it was useable and configuarble with the
   Arduino environment. I did this back in 2014. Thought someone would make a better one along the way and never got back to it.

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
   
   You will need the Joystick Lib for Arduino to make this compile straight away. 
   
   Use the example in the file. It should just work. But let me know if it doesn't. I made a push this afternoon with screaming kids to update it and push it to Git
   
   Appologies for taking so long to get this to Github. I'd uploaded it years ago, and then life happened. 

   RamjetX



	public:
		G25Shifter(int shifter_x, int shifter_y, int data_input_pin, int data_latch_pin, int data_clock_pin); // constructor
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
		
	private:
		void readButtons();
		void checkHandbrakeMode();
