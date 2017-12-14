//Viral Science
//RFID
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h>
#include "Adafruit_DRV2605.h"
 
#define SS_PIN 10
#define RST_PIN 9

Adafruit_DRV2605 drv;
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

int sensorPin = A0;
int sensorValue = 0;
int ledPin = 3;
bool logStatus = false;
unsigned long previousMillis = 0;
long interval = 1000;
int ledState = LOW;
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Put your card to the reader...");
  Serial.println();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  drv.begin();
  
  drv.selectLibrary(1);
  
  // I2C trigger by sending 'go' command 
  // default, internal trigger when sending GO command
  drv.setMode(DRV2605_MODE_INTTRIG); 
}

uint8_t effect = 46;

void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    if(logStatus == true){
      sensorValue = analogRead(sensorPin);
      unsigned long currentMillis = millis();
      if((sensorValue >= 0) && (sensorValue <= 512)) //if it's under 512
      { 
        interval = 855; 
      }
      else {
        interval = 599;
      }
    
     // wait a bit
      if (currentMillis - previousMillis >= interval){
          previousMillis = currentMillis;

          // set the effect to play
         drv.setWaveform(0, effect);  // play effect 
         drv.setWaveform(1, 0);       // end waveform
        
         // play the effect!
         drv.go();

          if (ledState == LOW){
            ledState = HIGH;
          } else {
            ledState = LOW;
          }

          digitalWrite(ledPin, ledState);
        }
    } else {
      digitalWrite(ledPin, LOW);
    }
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  if (content.substring(1) == "E2 BC 84 59") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Log In");
    logStatus = true;
  }
 
 else   {
    Serial.println("Log out");
    logStatus = false;
  }

}
