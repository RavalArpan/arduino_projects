#include <Arduino.h>
#include <NewPing.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define TRIGGER_PIN 2
#define ECHO_PIN 3

#define MAX_DISTANCE 400

int potPin = A1, ldrPin = A0, LEDpin = 4, buzzerPin = 5, servoPin = 9, servoPos;
float potVal, ldrStatus;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void ledOn(){
ldrStatus = analogRead(ldrPin);
Serial.print("ldrValue = ");
Serial.println(ldrStatus);
if (ldrStatus >= 20){
  digitalWrite(LEDpin, HIGH);
}
else{
  digitalWrite(LEDpin, LOW);
}
}

void buzz(){
  float distance = sonar.ping_cm();
  float buzzDistance;
  potVal = analogRead(potPin);
//  Serial.print("potVal = ");
//  Serial.println(potVal);
  buzzDistance = (MAX_DISTANCE*potVal)/1023;
//  Serial.print("buzzDistance = ");
//  Serial.println(buzzDistance);
  if(distance <= buzzDistance){
    digitalWrite(buzzerPin, HIGH);
  }
  else{
    digitalWrite(buzzerPin, LOW);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(potPin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(LEDpin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  buzz();
  delay(500);
  ledOn();
}