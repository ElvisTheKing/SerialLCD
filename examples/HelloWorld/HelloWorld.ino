#include <Arduino.h>
#include <SerialLCD.h>

// initialize the library
SerialLCD slcd;//this is a must, assign soft serial pins

void setup() {
  // set up
  Serial1.begin(9600);
  slcd.begin((Stream*) &Serial1);
  // Print a message to the LCD.
  slcd.print("hello, world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  slcd.setCursor(0, 1);
  // print the number of seconds since reset:
  slcd.print(millis()/1000,DEC);
}



