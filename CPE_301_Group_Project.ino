// libraries
#include <LiquidCrystal.h>
#include <Servo.h>
#include "DHT.h"
#include "RTClib.h"

// registers
volatile unsigned char *myTCCR1A = (unsigned char *) 0x80;
volatile unsigned char *myTCCR1B = (unsigned char *) 0x81;
volatile unsigned char *myTCCR1C = (unsigned char *) 0x82;
volatile unsigned char *myTIMSK1 = (unsigned char *) 0x6F;
volatile unsigned int  *myTCNT1  = (unsigned  int *) 0x84;
volatile unsigned char *myTIFR1 =  (unsigned char *) 0x36;

// port F pins A0-A7
volatile unsigned char* port_f = (unsigned char*) 0x31; 
volatile unsigned char* ddr_f = (unsigned char*) 0x30; 
volatile unsigned char* pin_f  = (unsigned char*) 0x2F; 

// objects and globals
int ticks = 0;

LiquidCrystal lcd(6, 7, 8, 9, 10, 11);

const int dhtPin = 12;
const int trigPin = 4;
const int echoPin = 5;
const int redLED = 2;
const int greenLED = 3;
const int enablePin = 52;
const int dirPin = 53;

Servo motor;
int pos = 0;

bool warning = false;
bool buttonPress = false;
bool handsPresent = false;

#define DHTTYPE DHT11
DHT dht(dhtPin, DHTTYPE);

long duration;
int distance;

RTC_DS1307 rtc;


void setup() 
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  dht.begin();
  rtc.begin();

  *ddr_f &= 0b11111110; // set A0 as input
  *port_f |= 0b00000001; // enable pullups

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(greenLED, INPUT);
  pinMode(redLED, INPUT);

  pinMode(enablePin,OUTPUT);
  pinMode(dirPin,OUTPUT);
  
  // make sure RTC connected
  if (! rtc.begin()) {
    Serial.println("ERROR: RTC not connected");
    Serial.flush();
    abort();
  }
  
  // adjust time to match device that the code is compiled on
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  motor.attach(43);
}

void loop() 
{
  if((*pin_f & 1)==0){
    buttonPress = true;
  }
  
  // Temperature
  // 
  // 
  // read temperature in Fahrenheit
  float f = dht.readTemperature(true);

  // check for error
  if (isnan(f)) 
  {
    lcd.setCursor(0,0); 
    lcd.print("ERROR");
    return;
  }

  // print temp to LCD
  lcd.setCursor(0,0); 
  lcd.print("Temp = ");
  lcd.print(f);
  lcd.print(" F");
  // 
  // 
  // End Temperature

// Ultrasonic Sensor
  //
  // 
  // Write a pulse to the HC-SR04 Trigger Pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure the response from the HC-SR04 Echo Pin
  duration = pulseIn(echoPin, HIGH);
  
  // Determine distance from duration
  // Use 343 metres per second as speed of sound
  distance = duration*0.034/2;
    
  if(distance <= 10 && distance > 0)
  {
    handsPresent = true;
    warning = true;
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
  }
  
  if(distance > 10 && warning)
  {
    handsPresent = false;
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);
    my_delay(100000);
    digitalWrite(redLED, LOW);
    my_delay(100000);
  }
  
  // Prints "Distance: <value>" on the second line of the LCD
  lcd.setCursor(0,1);
  lcd.print("Distance: "); 
  lcd.print(distance);
  lcd.print(" cm");
  // 
  // 
  // End Ultrasonic Sensor

  if(buttonPress & !handsPresent){
    motorOn();
  }

 

  // RTC Module
  // 
  // 
  DateTime now = rtc.now();
  //delay(1000);
  
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.print(now.second(), DEC);
  Serial.print(" (");
  Serial.print(now.month(), DEC);
  Serial.print("/");
  Serial.print(now.day(), DEC);
  Serial.print("/");
  Serial.print(now.year(), DEC);
  Serial.println(")");
  // 
  // 
  // End RTC Module


  // Fan 
  // 
  // 
 // fan on 
 // digitalWrite(enablePin,HIGH); //enable on
 // digitalWrite(dirPin,HIGH); //one way

 // fan off 
  //digitalWrite(enablePin,LOW);
  //digitalWrite(enablePin,LOW)
  // 
  // 
  // End Fan 

}

void motorOn(){
  for (pos = 0; pos <= 100; pos += 1) { // goes from 0 degrees to 100 degrees
    // in steps of 1 degree
    motor.write(pos);              // tell servo to go to position in variable 'pos'
    my_delay(1500);                    
  }
  for (pos = 100; pos >= 0; pos -= 1) { // goes from 100 degrees to 0 degrees
    motor.write(pos);              // tell servo to go to position in variable 'pos'
    my_delay(1500);                      
  }
}

void my_delay(unsigned int ticks)
{
  // stop timer
  *myTCCR1B &= 0xF8; 
  // set counts 
  *myTCNT1 = (unsigned int) (65536 - ticks);
  // set normal mode
  *myTCCR1A &= 0x00;
  // set prescaler (4), start timer 
  *myTCCR1B |= 0x04; 
  // wait for overflow flag
  while((*myTIFR1 & 0x01)== 0);
  // stop timer
  *myTCCR1B &=0xF8;
  // reset TOV
  *myTIFR1 |= 0x01;
}

void countdown(int sec)
{
  for(int i=sec; i>0; --i){
    lcd.print(i);
    lcd.print("seconds");
    my_delay(1000); // delay one sec
  }
}

void reset()
{
  bool warning = false;
  bool buttonPress = false;
  bool handsPresent = false;
}
