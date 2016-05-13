This project is based on Arduino, and my compiler is <Microsoft Visual Studio 2013> (Visual.Micro.Arduino.Studio.vsix must be instaled before you open this program).

*This project is updated from "ari3":
  Updated items include:
	*A new communication protocol between remote and command center
	*A correction of Bugs	
	*Better names of program files

*Communication protocol:
	SUMMARY: The new communication protocol is based on binary operation. 	This protocol brings a much faster communication speed (shorter delay) 	by simplify the command from characters into binary information. This 	simplify means the wireless system needs less time to transmit 	commands, and computers can analyze commands much easier. Also as a 	result of 	using binary operation to do data verification, the 	communication accuracy is also much better than ¡°air3¡±. Lastly, there 	is a new programming port to contain a customized encryption algorithm 	at a function called ¡°printout¡±.
	DATA_STRUCTURE: 
	Start key (I put ¡®$¡¯ as the start key) 
	Mode (1byte) 
	State on x axis (In the form of int16_t (float*100)) 
	State on y axis (same form as state on x axis)
	State on z axis (state on x axis) 
	Total power (uint8_t)
	Data verification (one byte: (uint8_t) (mode + statex + statey + statez 		+ tpower))
	End key (I put ¡®&¡¯ as the end key)

*Bug (fixed):
	*Memory overflow at ¡°air3-control-datarecieve-LINE *125

