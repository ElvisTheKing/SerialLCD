/*
  SerialLCD.cpp - Serial LCD driver Library
  2011 Copyright (c) Seeed Technology Inc.  All right reserved.

  Original Author: Jimbo.We
  Contribution: Visweswara R
  
  Modified by Peter Gamache (KC0TFB) to support Arduino IDE 1.0 and unify
  support for all firmware (Grove SLCD firmware 0.9b, 1.0b, 1.1b)

  Modified by Sergey Konyukhovskiy to support HardwareSerial and other Stream
  classes using stream dependency injection

  Note that not all firmware supports all features.  Power and Backlight
  commands may be ignored by earlier firmware.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <SerialLCD.h>
#include <Arduino.h>

SerialLCD::SerialLCD()
{

}


void SerialLCD::begin(Stream *s)
{
    stream = s;
    delay(2);

    noPower();
    delay(1);
    power();
    backlight();
    delay(1);

    // stream->write(SLCD_CONTROL_HEADER);   
    stream->write(SLCD_INIT_ACK);
    while(1)
    {
        if (stream->available() > 0 && stream->read()==SLCD_INIT_DONE)
            break;
    }
    delay(2);
}

//Turn off the back light
void SerialLCD::noBacklight()
{
    stream->write(SLCD_CONTROL_HEADER);   
    stream->write(SLCD_BACKLIGHT_OFF);   
}

//Turn on the back light
void SerialLCD::backlight()
{
    stream->write(SLCD_CONTROL_HEADER);   
    stream->write(SLCD_BACKLIGHT_ON);   
}

//Turn off the LCD
void SerialLCD::power()
{
    stream->write(SLCD_CONTROL_HEADER);   
    stream->write(SLCD_POWER_ON);   
}

//Turn on the LCD
void SerialLCD::noPower()
{
    stream->write(SLCD_CONTROL_HEADER);   
    stream->write(SLCD_POWER_OFF);   
}

// Clear the display
void SerialLCD::clear()
{
    stream->write(SLCD_CONTROL_HEADER);   
    stream->write(SLCD_CLEAR_DISPLAY);
    delay(2);//this command needs more time;  
}

// Return to home(top-left corner of LCD)
void SerialLCD::home()
{
    stream->write(SLCD_CONTROL_HEADER);
    stream->write(SLCD_RETURN_HOME);  
    delay(2);//this command needs more time;  
}

// Set Cursor to (Column,Row) Position
void SerialLCD::setCursor(uint8_t column, uint8_t row)
{
    delay(2);//this command needs more time;  
    stream->write(SLCD_CONTROL_HEADER); 
    stream->write(SLCD_CURSOR_HEADER); //cursor header command
    while(1)
    {
        if (stream->available() > 0 && stream->read()==SLCD_CURSOR_ACK)
            break;
    }
    stream->write(column);
    stream->write(row);
//one more to make sure the cursor is right
    delay(2);//this command needs more time;  
    stream->write(SLCD_CONTROL_HEADER); 
    stream->write(SLCD_CURSOR_HEADER); //cursor header command
    while(1)
    {
        if (stream->available() > 0 && stream->read()==SLCD_CURSOR_ACK)
            break;
    }
    stream->write(column);
    stream->write(row);
}

// Switch the display off without clearing RAM
void SerialLCD::noDisplay() 
{
    stream->write(SLCD_CONTROL_HEADER);
    stream->write(SLCD_DISPLAY_OFF);    
}

// Switch the display on
void SerialLCD::display() 
{
    stream->write(SLCD_CONTROL_HEADER);
    stream->write(SLCD_DISPLAY_ON);    
}

// Switch the underline cursor off
void SerialLCD::noCursor() 
{
    stream->write(SLCD_CONTROL_HEADER);
    stream->write(SLCD_CURSOR_OFF);     
}

// Switch the underline cursor on
void SerialLCD::cursor() 
{
    stream->write(SLCD_CONTROL_HEADER);
    stream->write(SLCD_CURSOR_ON);     
}

// Switch off the blinking cursor
void SerialLCD::noBlink() 
{
    stream->write(SLCD_CONTROL_HEADER);
    stream->write(SLCD_BLINK_OFF);     
}

// Switch on the blinking cursor
void SerialLCD::blink() 
{
    stream->write(SLCD_CONTROL_HEADER);
    stream->write(SLCD_BLINK_ON);     
}

// Scroll the display left without changing the RAM
void SerialLCD::scrollDisplayLeft(void) 
{
    stream->write(SLCD_CONTROL_HEADER);
    stream->write(SLCD_SCROLL_LEFT);
}

// Scroll the display right without changing the RAM
void SerialLCD::scrollDisplayRight(void) 
{
    stream->write(SLCD_CONTROL_HEADER);
    stream->write(SLCD_SCROLL_RIGHT);
}

// Set the text flow "Left to Right"
void SerialLCD::leftToRight(void) 
{
    stream->write(SLCD_CONTROL_HEADER);
    stream->write(SLCD_LEFT_TO_RIGHT);
}

// Set the text flow "Right to Left"
void SerialLCD::rightToLeft(void) 
{
    stream->write(SLCD_CONTROL_HEADER);
    stream->write(SLCD_RIGHT_TO_LEFT);
}

// This will 'right justify' text from the cursor
void SerialLCD::autoscroll(void) 
{
    stream->write(SLCD_CONTROL_HEADER);
    stream->write(SLCD_AUTO_SCROLL);
}

// This will 'left justify' text from the cursor
void SerialLCD::noAutoscroll(void) 
{
    stream->write(SLCD_CONTROL_HEADER);
    stream->write(SLCD_NO_AUTO_SCROLL);
}

// Print Commands

void SerialLCD::print(uint8_t b)
{
    stream->write(SLCD_CHAR_HEADER);
    stream->write(b);
}
void SerialLCD::print(const char b[])
{
    stream->write(SLCD_CHAR_HEADER);
    stream->write(b);
}

void SerialLCD::print(unsigned long n, uint8_t base)
{
    unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars.
    unsigned long i = 0;

    if (base == 0) print(n);

    else if(base!=0)
    {
        if (n == 0) {
            print('0');
            return;
        }

        while (n > 0) {
            buf[i++] = n % base;
            n /= base;
        }

        for (; i > 0; i--)
            print((char) (buf[i - 1] < 10 ?
                          '0' + buf[i - 1] :
                          'A' + buf[i - 1] - 10));
    }
    //delay(1);
}