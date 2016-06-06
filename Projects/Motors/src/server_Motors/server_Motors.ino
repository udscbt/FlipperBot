const char L_MOTOR = 16;
const char R_MOTOR = 14;

void setup() {
  Serial.begin(115200);
  // Initialise pins
  analogWriteRange(100);
  pinMode(R_MOTOR, OUTPUT);
  pinMode(L_MOTOR, OUTPUT);

  Serial.print("R_MOTOR = ");
  Serial.println(R_MOTOR);

  Serial.print("L_MOTOR = ");
  Serial.println(L_MOTOR);

  Serial.println("DONE");
  
  analogWrite(R_MOTOR, 1);
  analogWrite(L_MOTOR, 99);
}

void loop() {
  Serial.println("OK");
  delay(500);
}


