#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

#define BUTTON_PIN        5  // Button

#define LONGPRESS_LEN    25  // Min nr of loops for a long press
#define DELAY            20  // Delay per loop in ms

enum { EV_NONE=0, EV_SHORTPRESS, EV_LONGPRESS };

boolean button_was_pressed; // previous state
int button_pressed_counter; // press running duration

unsigned long lastSecond = 0;
unsigned int seconds = 0;
unsigned int minutes = 0;
unsigned int hours = 0;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(BUTTON_PIN, INPUT);
  button_was_pressed = false;
  button_pressed_counter = 0;
}

void loop() {
  checkForButtonPress();
  updateTime();
  displayTime();
  updateColor();
  doBillTime();
}

// Checks if a button event has occurred, and updates the time accordingly if so
void checkForButtonPress(){
  int event = handleButton();
  
  // Check if button pressed
  switch (event) {
    case EV_SHORTPRESS:
      minutes++;
      break;
    case EV_LONGPRESS:
      hours++;
      break;
  }
}

// Increments the seconds, minutes and hours if a new second has occurred
void updateTime(){
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
}

// Prints the current time values on the LCD
void displayTime(){
  lcd.setCursor(0,0);
  lcd.print(hours);
  lcd.print(" hours, ");
  lcd.print(minutes);
  lcd.print(" mins");
  if(!isBillTime()){
    lcd.setCursor(0,1);
    lcd.print("and ");
    lcd.print(seconds);
    lcd.print(".");
    lcd.print((millis() - lastSecond) / 100);
    lcd.print(" seconds ");
  }
}

// Sets the backlight colour based on the current time
void updateColor(){
   if(seconds % 10 < 5)
     lcd.setRGB(51,237,33);
   else
     lcd.setRGB(245,245,17);
}

boolean isBillTime(){
  return seconds % 20 < 2;
}

void doBillTime(){
  if(isBillTime()){
    lcd.setCursor(0,1);
    lcd.print(" CONGRATS BILL! "); 
  }
}

// Determines if a short or long press has occurred
int handleButton()
{
  int event;
  int button_now_pressed = digitalRead(BUTTON_PIN); // pin high -> pressed

  if (!button_now_pressed && button_was_pressed) {
    if (button_pressed_counter < LONGPRESS_LEN)
      event = EV_SHORTPRESS;
    else
      event = EV_LONGPRESS;
  }
  else
    event = EV_NONE;

  if (button_now_pressed)
    ++button_pressed_counter;
  else
    button_pressed_counter = 0;

  button_was_pressed = button_now_pressed;
  return event;
}

