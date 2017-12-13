#include <Wire.h>
#include "Adafruit_DRV2605.h"

int sensorPin = A0;
int sensorValue = 0;
int ledPin = 3;

Adafruit_DRV2605 drv;



void setup() {
  
  Serial.begin(9600);
  Serial.println("DRV test");
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  drv.begin();
  
  drv.selectLibrary(1);
  
  // I2C trigger by sending 'go' command 
  // default, internal trigger when sending GO command
  drv.setMode(DRV2605_MODE_INTTRIG); 
}

uint8_t effect = 46;

void loop() {
  Serial.print("Effect #"); Serial.println(effect);
  sensorValue = analogRead(sensorPin);
  int delayTime = 500;
  
  if((sensorValue >= 0) && (sensorValue <= 512)) //if it's under 512
 { 
  delayTime = 1000;
  digitalWrite(ledPin, LOW);
  
 }
 else {
  delayTime = 500;
  digitalWrite(ledPin, HIGH);
  
 }

  // set the effect to play
  drv.setWaveform(0, effect);  // play effect 
  drv.setWaveform(1, 0);       // end waveform

  // play the effect!
  drv.go();

  // wait a bit
  delay(delayTime);
}

//light press: values between 400-500;
//medium press: values between 200-400;
//hard press: values between 0-200;
//=> NO PRESS: values between 500 -
