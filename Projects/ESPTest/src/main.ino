#include "schemo.h"
#include "fbcp.common.h"
#include "FBNet.h"

#include <ESP8266WiFi.h>

const char L_MOTOR = 16;
const char R_MOTOR = 14;
const char HIT     = 05;
const char BUZ     = 04;
const char LED     = 15;



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
  Serial.setTimeout(500);
}

int i = 0;
void loop()
{
  long n = Serial.parseInt();
  if (n)
  {
	analogWrite(R_MOTOR, n);
	Serial.print("R_MOTOR: ");
	Serial.println(n);
  }
  Serial.println(i++);
  
}
