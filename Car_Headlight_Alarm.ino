/*
   by: Ryan Forcier (Jim Lindblom)
   date: July 18,2014
   license: This code is public domain.



   Circuit:
   Arduino -------------- Serial 7-Segment
     5V   --------------------  VCC
     GND  --------------------  GND
     SDA  --------------------  SDA 
     SCL  --------------------  SCL 
*/

#include <Wire.h> // Include the Arduino SPI library

boolean startToggle=0;
int playPausePin=10;
int timeUpPin=11;
int timeDownPin=12;
int leftLightPin=A0;
int rightLightPin=A1;
int lightTimeOut=5;
int lightCycle=0;

const byte s7sAddress = 0x71;
unsigned int counter = 0;  // This variable will count up to 65k
char tempString[10];  // Will be used with sprintf to create strings

void setup()
{
  I2Csetup();
  ButtonSetup();
  pinMode(leftLightPin,OUTPUT);
  pinMode(rightLightPin,OUTPUT);
}

void loop()
{
   
  if (digitalRead(playPausePin) == LOW)
  {
    playPausePinLogic();
  }
  
  if (digitalRead(timeUpPin) == LOW)
  {
    timeUpPinLogic();
  }
  
  if (digitalRead(timeDownPin) == LOW)
  {
    timeDownPinLogic();
  }
 
  if (startToggle == true)
  {
    if(counter > 0)
    {
      counter--; 
    }
    else
    {
      flashingLights();
    }
  }
  
  sprintf(tempString, "%4d", counter);
  s7sSendStringI2C(tempString);
  delay(50);
      
}

void ButtonSetup()
{
  Serial.begin(9600);
  pinMode(playPausePin,INPUT);
  pinMode(timeUpPin,INPUT);
  pinMode(timeDownPin,INPUT);
}

void playPausePinLogic()
{

  while(digitalRead(timeUpPin) == LOW)
  {}
  delay(100);
  
  if (startToggle== false)
  {
      startToggle=true;
  }
  else
  {
      startToggle=false;
  }
  
  
}

void timeUpPinLogic()
{
  delay(500);
  counter++;
  

}
    
void timeDownPinLogic()
{
  delay(500);
  if (counter>0){
    counter--;
  }
  
}

void s7sSendStringI2C(String toSend)
{
  Wire.beginTransmission(s7sAddress);
  for (int i=0; i<4; i++)
  {
    Wire.write(toSend[i]);
  }
  Wire.endTransmission();
}

void I2Csetup()
{
  Wire.begin();  // Initialize hardware I2C pins
  clearDisplayI2C();  // Clears display, resets cursor
  setDecimalsI2C(0b00010000);  // Turn on only colon
  setBrightnessI2C(255);  // High brightness
  clearDisplayI2C();  // Clear the display before jumping into loop
}

void clearDisplayI2C()
{
  Wire.beginTransmission(s7sAddress);
  Wire.write(0x76);  // Clear display command
  Wire.endTransmission();
}

void setBrightnessI2C(byte value)
{
  Wire.beginTransmission(s7sAddress);
  Wire.write(0x7A);  // Set brightness command byte
  Wire.write(value);  // brightness data byte
  Wire.endTransmission();
}

void setDecimalsI2C(byte decimals)
{
  Wire.beginTransmission(s7sAddress);
  Wire.write(0x77);
  Wire.write(decimals);
  Wire.endTransmission();
}

void flashingLights()
{
  
  for (int cycle = 1; cycle < lightTimeOut+1; cycle++)
  {
  digitalWrite(leftLightPin,HIGH);
  digitalWrite(rightLightPin,LOW);
  delay(500);
  digitalWrite(leftLightPin,LOW);
  digitalWrite(rightLightPin,LOW);
  delay(25);
  digitalWrite(leftLightPin,LOW);
  digitalWrite(rightLightPin,HIGH);
  delay(500);
  digitalWrite(leftLightPin,LOW);
  digitalWrite(rightLightPin,LOW);
  delay(25);
  }
  
  startToggle=false;
  
}
