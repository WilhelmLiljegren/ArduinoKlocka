/*
* Name: clock and temp project
* Author: Wilhelm Liljegren
* Date: 2024-10-10
* Description: This project uses a ds3231 to measure time and displays the time to an 1306 oled display,
* Further, it measures temprature with a analog temprature module and displays a mapped value to a 9g-servo-motor
*/

// Include Libraries
#include <RTClib.h>
#include <Wire.h>
#include "U8glib.h"
#include <Servo.h>
// Init constants
int Vo;
float R1 = 10000;  // value of R1 on board
float logR2, R2, T;
float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741;  //steinhart-hart coeficients for thermistor
int pos = 0;                                                     //variable to store the servo position

// Init global variables
int ThermistorPin = A0;
int LedpinRed = 2;
int LedpinBlue = 3;
// construct objects
RTC_DS3231 rtc;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);
Servo myservo;

void setup() {
  // init communication
  Serial.begin(9600);
  Wire.begin();
  // Init Hardware
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  myservo.attach(9);
  u8g.setFont(u8g_font_unifont);
  pinMode(LedpinRed, OUTPUT);
  pinMode(LedpinBlue, OUTPUT);
 
}
void loop() {
if (T > 30) {  //alarm the high temperature
 oledWrite(getTime());
myservo.write(360);
 digitalWrite(LedpinBlue, HIGH); 
  delay(10);
  digitalWrite(LedpinBlue, LOW);
  delay(10);
  digitalWrite(LedpinRed, HIGH);
  delay(10);
  digitalWrite(LedpinRed, LOW);
  delay(10);
 }
 


 if(T > 20 && T<30){
  myservo.write(getTemp());
  oledWrite(String(getTemp()));
  digitalWrite(LedpinBlue, LOW); //To kill the lights
digitalWrite(LedpinRed, LOW);
}

}


/*
*This function reads time from an ds3231 module and package the time as a String
*Parameters: Void
*Returns: time in hh:mm:ss as String
*/
String getTime() {

  DateTime now = rtc.now();
  return String(now.hour()) + " " + (String(now.minute()) + " " + String(now.second()));
}

/*
* This function reads an analog pin connected to an analog temprature sensor and calculates the corresponding temp
*Parameters: Void
*Returns: temprature as float
*/
float getTemp() {
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);  //calculate resistance on thermistor
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));  // temperature in Kelvin
  T = T - 273.15;                                              //convert Kelvin to Celcius
 // Serial.print("Temperature: ");
 // Serial.print(T);
 // Serial.println(" C");

  return (T);
  delay(500);
}
/*
* This function takes a string and draws it to an oled display
*Parameters: - text: String to write to display
*Returns: void
*/
void oledWrite(String text) {

  u8g.firstPage();

  do {
    u8g.drawStr(0, 36, text.c_str());
  } while (u8g.nextPage());
}

/*
*If the temperature reaches over 
*/
void servoWrite(float angle) {
  myservo.write(angle);
}

