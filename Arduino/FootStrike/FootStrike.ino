#include <Arduino_FreeRTOS.h>

void FootStrike(); // define task

const int PS1 = A3; //pressure sensor pin 1
const int PS2 = A4; //pressure sensor pin 2

void setup() { // setup code to run once

  Serial.begin(9600);
  
  xTaskCreate(
    footStrike
    ,  "FootStrike"
    ,  128
    ,  NULL
    ,  1
    ,  NULL );
}

void loop()
{
  //tasks are used instead
}

void footStrike() // task
{
  // initialise analogue pins for pressure sensor 
  pinMode(PS1, INPUT);
  pinMode(PS2, INPUT);

  // Task will run continuosly while power on
  for (;;) 
  {
      // pressure sensor ADC reading
      int ps1ADC = analogRead(PS1);
      int ps2ADC = analogRead(PS2);

    // if above at rest threshold, write to console  
    if (ps1ADC > 3){
      Serial.println ("ADC 1: " + String(ps1ADC));
      Serial.println();
    }
    if (ps2ADC > 3){
      Serial.println ("ADC 2: " + String(ps2ADC));
      Serial.println();
    }
    delay(100);
  }
}
