#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Servo.h>  // the servo library
#define servoPin 9
Servo servo;
#define CACHE_LAT 40.307560
#define CACHE_LON -3.734874
#define DEBUG 1
/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
   servo.attach(servoPin);
   delay(100);
   servo.write(680);//544 a 2000
   delay(1000);
   servo.write(2400);
   delay(1000);
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
}

void loop()
{
  static int milisegundosAnt=0;
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
  
    if (gps.encode(ss.read()))
    {
      displayInfo();
      double courseToCache = TinyGPSPlus::courseTo(gps.location.lat(),gps.location.lng(),CACHE_LAT, CACHE_LON);
      double rumbo= (courseToCache - gps.course.deg())*100;
      Serial.print ("Rumbo:"+ String(rumbo));
      int milisegundos=map(rumbo,-9000,9000,2400,600);
      #ifdef DEBUG
        
        Serial.println (" msg:"+ String(milisegundos));
      #endif 
      if(milisegundos != milisegundosAnt)
      {
          ss.end();
          servo.attach(servoPin);
          delay(100);
          servo.writeMicroseconds (milisegundos);
          delay(1000);
          servo.detach();
          ss.begin(9600);
          milisegundosAnt=milisegundos;
      }
  }
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo()
{
      #ifdef DEBUG
  if (gps.location.isValid())
  {

    Serial.print(F("Location: ")); 
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    Serial.print(F(","));
    Serial.print(gps.course.deg());
    
  } 
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
  #endif
}
