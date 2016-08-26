#include "Arduino.h"
#include "motors.h"

const unsigned char Motor::STOP = 0;
const unsigned char Motor::OLD_CW = 1;
const unsigned char Motor::NEW_CW = 20;
const unsigned char Motor::OLD_CCW = 99;
const unsigned char Motor::NEW_CCW = 50;

const unsigned char Motor::LEFT = 1<<4;
const unsigned char Motor::RIGHT = 1<<5;
const unsigned char Motor::OLD = 1<<6;
const unsigned char Motor::NEW = 1<<7;

Motor::Motor(const unsigned char config) : config(0)
{
  this->configure(config);
}

void Motor::configure(const unsigned char config)
{
  this->config = config;
  pinMode(this->getPin(), OUTPUT);
  stop();
}

void Motor::forward()
{
  this->direction = DIR_FORWARD;
  unsigned char value;
  if (this->checkAll(LEFT | OLD))
  {
    value = OLD_CCW;
  }
  else if (this->checkAll(LEFT | NEW))
  {
    value = NEW_CCW;
  }
  else if (this->checkAll(RIGHT | OLD))
  {
    value = OLD_CW;
  }
  else if (this->checkAll(RIGHT | NEW))
  {
    value = NEW_CW;
  }
  analogWrite(this->getPin(), value);
}

void Motor::stop()
{
  this->direction = DIR_STOP;
  analogWrite(this->getPin(), 0);
}

void Motor::backward()
{
  this->direction = DIR_BACKWARD;
  unsigned char value;
  if (this->checkAll(LEFT | OLD))
  {
    value = OLD_CW;
  }
  else if (this->checkAll(LEFT | NEW))
  {
    value = NEW_CW;
  }
  else if (this->checkAll(RIGHT | OLD))
  {
    value = OLD_CCW;
  }
  else if (this->checkAll(RIGHT | NEW))
  {
    value = NEW_CCW;
  }
  analogWrite(this->getPin(), value);
}

void Motor::value(const unsigned char dir)
{
  Serial.print("=======");
  Serial.print("MOTOR ");
  if (this->checkAll(RIGHT))
  {
    Serial.println("R");
  }
  else if (this->checkAll(LEFT))
  {
    Serial.println("L");
  }
  else
  {
    Serial.println("?");
  }

  Serial.print("TYPE ");
  if (this->checkAll(OLD))
  {
    Serial.println("O");
  }
  else if (this->checkAll(NEW))
  {
    Serial.println("N");
  }
  else
  {
    Serial.println("?");
  }

  Serial.print("DIR ");
  Serial.println((int)dir);
  
  switch (dir)
  {
    case 0:
      this->stop();
      break;
    case 1:
      this->forward();
      break;
    case (unsigned char) -1:
      this->backward();
  }
}

Motor::Direction Motor::getDirection() const
{
  return this->direction;
}
