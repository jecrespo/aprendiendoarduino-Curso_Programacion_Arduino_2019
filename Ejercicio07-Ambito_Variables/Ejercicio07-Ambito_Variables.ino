#include <Servo.h>

#define PIN_SERVO 9
#define PIN_BOTON_A 2

Servo miservo;

int grados = 10;  //Variable global

void setup() {
  serial.begin(9600) //
  pinMode(PIN_BOTON_A, INPUT_PULLUP);
  miservo.attach(PIN_SERVO);
  miservo.write(grados);
}

void loop() {
  Serial.println(grados);
  detectaFlanco();
}

void detectaFlanco() {
  static boolean anterior_a = digitalRead(PIN_BOTON_A); //Variable local
  boolean estado_a = digitalRead(PIN_BOTON_A);  //Variable local

  if (anterior_a != estado_a) {
    if (estado_a == HIGH) {  //flanco ascendente pull-up
      if (grados >= 170)
        grados = 10;
      else
        grados += 10;
      miservo.write(grados);
    }
    anterior_a = estado_a;  //
  }
}
