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

	2, 3, 4, 5
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


Joshua Weitzel, February 2025