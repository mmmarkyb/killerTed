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
bool logStatus = false;
unsigned long previousMillis = 0;
long interval = 1000;
int ledState = LOW;

//Ambient Light
int potentPin = A1;    // select the input pin for the potentiometer
int ambientPin = 3;      // select the pin for the LED
int lightLevel = 0;

//Heart light
int data = 6;
int clock = 5;
int latch = 4;

int bigHeart[] = {B01100110, B10011001, B10000001, B10000001, B10000001, B01000010, B00100100, B00011000};
int littleHeart[] = {B00000000, B00100100, B01011010, B01000010, B01000010, B00100100, B00011000, B00000000};
int nothing[] = {0,0,0,0,0,0,0,0};

void ambient();
void show(int[]);
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Put your card to the reader...");
  Serial.println();
  pinMode(ambientPin, OUTPUT);
  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(latch, OUTPUT);
  
  drv.begin();
  
  drv.selectLibrary(1);
  
  // I2C trigger by sending 'go' command 
  // default, internal trigger when sending GO command
  drv.setMode(DRV2605_MODE_INTTRIG); 
  show(nothing);
}

uint8_t effect = 46;

void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    if(logStatus == true){
       analogWrite(ambientPin, 0);
      sensorValue = analogRead(sensorPin);
      unsigned long currentMillis = millis();
      if((sensorValue >= 0) && (sensorValue <= 512)) //if it's under 512
      { 
        interval = 500; 
      }
      else {
        interval = 250;
      }
    
     // wait a bit
      if (currentMillis - previousMillis >= interval){
          previousMillis = currentMillis;

          // set the effect to play
         drv.setWaveform(0, effect);  // play effect 
         drv.setWaveform(1, 0);       // end waveform
        
         // play the effect!
         drv.go();
          for( int k = 0; k < interval; k++){
            show(littleHeart);
          }
          drv.go();
          for( int k = 0; k < interval; k++){
           show(bigHeart);
          }
          show(nothing);
  /*        if (ledState == LOW){
            ledState = HIGH;
          } else {
            ledState = LOW;
          }*/

          //digitalWrite(ledPin, ledState);
        }
    } else {
      //digitalWrite(ledPin, LOW);
      ambient();
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
  Serial.println("Ambient Light");
}

void ambient() {
int lightLevel = analogRead(potentPin); //Read the
                                        // lightlevel
 lightLevel = map(lightLevel, 0, 900, 0, 255); 
         //adjust the value 0 to 900 to
         //span 0 to 255



 lightLevel = constrain(lightLevel, 0, 255);//make sure the 
                                           //value is betwween 
                                           //0 and 255
 analogWrite(ambientPin, lightLevel);  //write the value
}

void show(int red []){
  for (int i=0;i<8;i++){
  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, ~(1<<i));
  shiftOut(data, clock, LSBFIRST, red[i]);
  digitalWrite(latch, HIGH);
  } 
}
