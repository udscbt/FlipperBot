#include "schemo.h"
#include "fbcp.common.h"
#include "FBNet.h"	

#include <ESP8266WiFi.h>

const char L_MOTOR = 16;
const char R_MOTOR = 14;
const char HIT     = 05;
const char BUZ     = 04;
const char LED     = 13;



void setup()
{
  Serial.begin(115200);
  delay(10);
  
  pinMode(LED, OUTPUT);
  pinMode(BUZ, OUTPUT);
  analogWriteRange(100);
  pinMode(R_MOTOR, OUTPUT);
  analogWrite(R_MOTOR, 0);
  pinMode(L_MOTOR, OUTPUT);
  analogWrite(L_MOTOR, 0);
  pinMode(HIT, INPUT);
}

void loop()
{
  
  analogWrite(R_MOTOR, 1);
  analogWrite(L_MOTOR, 99);
  Serial.println("Forward");
  delay(1000);
  analogWrite(R_MOTOR, 0);
  analogWrite(L_MOTOR, 0);
  Serial.println("Stop");
  digitalWrite(LED, HIGH);
  digitalWrite(BUZ, LOW);
  delay(500);
  digitalWrite(LED, LOW);
  digitalWrite(BUZ, HIGH);
  delay(500);
  analogWrite(R_MOTOR, 99);
  analogWrite(L_MOTOR, 1);
  Serial.println("Backward");
  delay(1000);
}
