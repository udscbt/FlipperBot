/*************************************
 * Auto-generated ScheMo source file *
 *************************************
 * Original file name: main/main.ino
 * New file name: ./main/main.ino
 * Identified jobs: 0
 *   Named: 0
 *   Unnamed: 0
 * Identified tasks: 0
 *   Named: 0
 *   Unnamed: 0
 */

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
            }

void loop()
{
  
                        
  digitalWrite(LED, HIGH);
  digitalWrite(BUZ, LOW);
  delay(500);
  digitalWrite(LED, LOW);
  digitalWrite(BUZ, HIGH);
  delay(500);
}
