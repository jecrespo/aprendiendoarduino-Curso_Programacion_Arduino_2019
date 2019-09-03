#include <Servo.h>

#define PIN_SERVO 9
#define PIN_POT A0

Servo miservo;

void setup() {
  miservo.attach(PIN_SERVO);
}

void loop() {
  int val = analogRead(PIN_POT);
  val = map(val, 0, 1023, 10, 170);
  miservo.write(val);
  delay(15);
}
