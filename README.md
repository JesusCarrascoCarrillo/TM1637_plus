TM1637_plus
======
Arduino library for TM1637 (LED Driver) by RobotDyn


Description
-----------
An Arduino library for 7-segment display modules based on the TM1637 chip, such as Seeed Studio's [Grove 4 digit display](http://www.seeedstudio.com/depot/grove-4digit-display-p-1198.html). The TM1637 chip also has keyboard input capability, but it's not implemented in this library.

Hardware Connection
-------------------
The display modules has two signal connection (and two power connections) which are CLK and DIO. These pins can be connected to any pair of digital pins on the Arduino. When an object is created, the pins should be configured. There is no limitation on the number of instances used concurrently (as long as each instance has a pin pair of its own)

Installation
------------
The library is installed as any Arduino library, by copying the files into a directory on the library search path of the Arduino IDE

Usage
-----
The library provides a single class named TM1637. An instance of this class provides the following functions:

* `TM1637` - init the class with the pin CLK and the pin DIO
* `init` - To clear the display
* `display` - Display a digit or 4 digit in array
* `display_sym` - display a sym in the position, the sym is a 7 segment
                                       a
																	    ---
																	 f |   | b
																	    ---
																	 e | g | c
																	    ---
																		   d	
* `set` - Sets the brightness and other parameters of the display
* `point` - Display a point in the position or all point in the dispaly

The information given above is only a summary. Please refer to TM1637Display.h for more information. An example is included, demonstrating the operation of most of the functions.
