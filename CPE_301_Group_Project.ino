// libraries
#include <LiquidCrystal.h>
#include "DHT.h"
#include "RTClib.h"


LiquidCrystal lcd(6, 7, 8, 9, 10, 11);

const int dhtPin = 12;
const int trigPin = 4;
const int echoPin = 5;
const int yellowLED = 2;
const int greenLED = 3;

bool LEDstart = false;

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

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(greenLED, INPUT);
  pinMode(yellowLED, INPUT);

  
  // make sure RTC connected
  if (! rtc.begin()) {
    Serial.println("ERROR: RTC not connected");
    Serial.flush();
    abort();
  }
  
  // adjust time to match device that the code is compiled on
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
}

void loop() 
{
  // Temperature
  // 
  // 
  lcd.clear();
  // read temperature in Fahrenheit
  float f = dht.readTemperature(true);

  // check for error
  if (isnan(f)) 
  {
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
  
  
  if(distance <= 10)
  {
    LEDstart = true;
    digitalWrite(greenLED, HIGH);
    digitalWrite(yellowLED, LOW);
  }
  
  if(distance > 10 && LEDstart)
  {
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH);
    delay(500);
    digitalWrite(yellowLED, LOW);
  }
  
  
  // Prints "Distance: <value>" on the second line of the LCD
  // for debugging purposes right now
  lcd.setCursor(0,1);
  lcd.print("Distance: "); 
  lcd.print(distance);
  lcd.print(" cm");
  // 
  // 
  // End Ultrasonic Sensor

  
  // RTC Module
  // 
  // 
  DateTime now = rtc.now();

  
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



// ignore for now
  
  /*if((*pin_? & 1) == 0) // button pin is 0
    // do nothing
  else
    start();*/
}

/* void start(){
  // turn on motor
    // set stepper pins to outputs
    *ddr_? |= 0x00 << /*pin numbers;
    *port_? |= 1 << step_number;
    step_number++; 
}*/
