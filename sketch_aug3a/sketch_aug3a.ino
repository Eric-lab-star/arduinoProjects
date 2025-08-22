int dataPin = 2;
int clockPin = 3;
int latchPin = 4;


void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin,  OUTPUT);
  pinMode(latchPin,  OUTPUT);
}

void loop() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 1 << i );
    digitalWrite(latchPin, HIGH);
    delay(200);
  }
  delay(500);
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
  digitalWrite(latchPin, HIGH);
  delay(500);
}
