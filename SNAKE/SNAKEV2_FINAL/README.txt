This SNAKE game is meant to be played on a 128 x 64 4-pin OLED I2C display hooked up to an Arduino
Here is a detailed guide on how to setup an Arduino to use the program

The pin connections for the display are as follows:

Display		Arduino
VCC	---->	5V
GND	---->	GND
SDA	---->	A4
SCL	---->	A5

The button pins BPIN constants defined at the top
The buttons are connected to the Arduino like this:

	BPIN...
 ||	  ||
|------------|
|	     |
|   Button   |
|	     |
|____________|
 ||	  ||
GND	10Kohm
	  ||
	  5V

The 4 buttons are the connected the same way to the GND and the 5V source,
the only difference is that they will connect to different pins on the Arduino
specified by the BPIN constants defined at the top of the SNAKE file. 
Thr default pins are 2, 3, 4, and 5.

Joshua Weitzel, February 2025
