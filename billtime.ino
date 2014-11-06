#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

unsigned long lastSecond = 0;
unsigned int seconds = 0;
unsigned int minutes = 0;
unsigned int hours = 0;

void setup() {
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
}

void loop() {
    if(millis() - lastSecond > 1000)
    {
        lastSecond += 1000;
        seconds++;
    }
  
    if(seconds > 59){
        seconds = 0;
        minutes++; 
    }
    
    if(minutes > 59){
        minutes = 0;
        hours++; 
    }
  
    lcd.setCursor(0,0);
    lcd.print(hours);
    lcd.print(":");
    lcd.print(minutes);
    lcd.print(":");
    lcd.print(seconds);
}

