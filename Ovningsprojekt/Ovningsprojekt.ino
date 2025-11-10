/*
Name: övningsprojekt
Author: Tor Johansson
Date: 2025-10-13
Description: This project uses a ds3231 to measure time and displays the time to an 1306 oled display,
Further, it measures temprature with ds3231 and displays a mapped value to a 9g-servo-motor.
*/

// Include Libraries
#include <RTClib.h>
#include <Wire.h>
#include "U8glib.h"
#include <Servo.h>

Servo myservo;

int val;


// Init constants

// Init global variables

// Construct objects
RTC_DS3231 rtc;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC


void setup() {
  // init communication
  Serial.begin(9600);
  Wire.begin();

  // Init Hardware
  rtc.begin();

  // Settings
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  u8g.setFont(u8g_font_unifont); //gör så texten ser ut som den gör

  myservo.attach(9); //lägger servo på pin 9

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT); //gör så lampan kan använda pinnen
}

void loop() {
  String timeStr = getTime();
  float temp = getTemp();
  
  oledWrite(timeStr, temp);
  Serial.println(temp);
  servoWrite(temp);

  delay(1000);

  if(getTemp() < 10){ //om graderna är under 10 så lysser den blåa lampan
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  }
  else if(getTemp() > 25){ //om graderna är över 25 så lysser den röda lampan
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
  }
  else{ //om graderna är mellan 10 och 25 så lysser den gröna lampan
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
  }
}



//This function reads time from an ds3231 module and package the time as a String
//Parameters: Void
//Returns: time in hh:mm:ss as String
String getTime() {
  DateTime now = rtc.now();
  return String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
}

/*
This function takes temprature from ds3231 and returns as a float
*Parameters: Void
*Returns: temprature as float 
*/
float getTemp() {
  return rtc.getTemperature(); //Ger tillbaka tempen från rtc
  
}

/*
This function takes a string and draws it to an oled display
*Parameters: - text: String to write to display
*Returns: void
*/
void oledWrite(String timeStr, float temp) {
  u8g.firstPage();
  do {
    u8g.drawStr(10,20, "Tid:");
    u8g.drawStr(50, 20, timeStr.c_str());

    u8g.drawStr (10,40, "Temp:");
    String tempStr = String(temp, 1) + " C";
    u8g.drawStr(60, 40, tempStr.c_str());
  } while (u8g.nextPage());
}

/*
takes a temprature value and maps it to corresppnding degree on a servo
*Parameters: - value: temprature
*Returns: void
*/
void servoWrite(float value) {
  val = map(val, 0, 1023, 0, 179);
  myservo.write(val);

  delay(15);
}