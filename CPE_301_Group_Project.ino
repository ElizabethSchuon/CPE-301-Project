/*
CPE 301 Group Project
Spring 2021
Elizabeth Schuon
Jullian Flienger
-----------------------------------
Comment notes:
All self doccumenting comments with a space betwen the '//' and the comment were made by Jullian
All self doccumenting comments without one were made by Elizabeth
Comments directed at eachother in the code are signed with a /ES or /JF corrisponding to the person who wrote them
*/

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
const int motorPin = 13;
//the numbering on these two needs fixing
const int fanPin = 14;
const int buttPin = 15;

bool LEDstart = false;
bool buttPress = false;
//this is the variable I am using in the loop thing below /ES
int sensorCheck = 0;

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
  
  pinMode(motorPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(buttPin, INPUT);

  
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
  //button stuff
  while(!buttPress){
    //occurs as long as the button has not been pressed 
    if(buttPin){
      //make buttPress true so it does not continue to loop
      buttPress = true;
      //turn on motor to simulate water running
      digitalWrite(motorPin, HIGH);
      
    }
    //no else condition, nothing should happen if the botton is not pressed 
    //it doesn't matter if the button is pressed more than once, so no debouncing needed 
  }
  
  //needs to be in a loop for if hands are in place, i think that this is all that will need to be coninously updated /ES
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
          //is this your conditional statement where you turn the green LED on /ES
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
  
  //I am going to make kinda a part code part psudo code loop here to illistrate /ES
  //
  /*
    while(sensorCheck <= 30){
      update the tempurature output of water
      measure distance with ultrasonic sensor
      if( hands in range){
        sensorCheck++;
      }
      else{
        sensorCheck = 0;
      }
      delay(1000);
      //I set it to delay one second /ES
      //could be more or less frequent if desired /ES
      //If delay changes the 30 will need to change /ES
    }
  */
  //
  //end of illustrative loop
  
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

  //resetting stuff
    //turn off fan
    //turn off LEDs
    //I don't know exactly what may need to be turned of for the ultrasonic and tempurature things /ES
    //turn off any ultrasonic stuff
    //turn off tempurature display
    //Turn off tempurature sensor


// ignore for now /JF
  
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
