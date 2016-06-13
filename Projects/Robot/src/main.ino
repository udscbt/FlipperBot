#include <ESP8266WiFi.h>

const char L_MOTOR = 16;
const char R_MOTOR = 14;

void leftMotor(int dir)
{
  if (dir > 0)
  {
    analogWrite(L_MOTOR, 99);
  }
  else if (dir < 0)
  {
    analogWrite(L_MOTOR, 1);
  }
  else
  {
    analogWrite(L_MOTOR, 0);
  }
}

void rightMotor(int dir)
{
  if (dir > 0)
  {
    analogWrite(R_MOTOR, 1);
  }
  else if (dir < 0)
  {
    analogWrite(R_MOTOR, 99);
  }
  else
  {
    analogWrite(L_MOTOR, 0);
  }
}

void setup()
{
  // Initialise serial communication
  Serial.begin(115200);
  delay(10);

  Serial.println("Starting");

  // Initialise pins
  analogWriteRange(100);
  pinMode(R_MOTOR, OUTPUT);
  analogWrite(R_MOTOR, 0);
  pinMode(L_MOTOR, OUTPUT);
  analogWrite(L_MOTOR, 0);
}

void loop()
{
  Serial.println("Starto!");
  leftMotor(1);
  rightMotor(-1);
  delay(2000);
  Serial.println("Continuu!");
  leftMotor(-1);
  rightMotor(1);
  delay(2000);
}
