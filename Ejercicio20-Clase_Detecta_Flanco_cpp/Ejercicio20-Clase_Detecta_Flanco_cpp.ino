#include "Contador.h"
#include "DetectaFlanco.h"

#define PIN_BOTON_A 2
#define PIN_BOTON_B 3

Contador C1, C2;
DetectaFlanco DF1(PIN_BOTON_A), DF2(PIN_BOTON_B);

void setup() {
  Serial.begin(9600);
  DF1.inicio(INPUT_PULLUP);
  DF2.inicio(INPUT);
  C1.SetContador(10);
  C2.SetContador(100);
}

void loop() {
  if (DF1.comprueba() == -1) {
    C1.Incrementar() ;
    Serial.println("C1 = " + (String)C1.GetCont());
  }

  if (DF2.comprueba() == 1) {
    C2.Incrementar() ;
    Serial.println("C2 = " + (String)C2.GetCont());
  }

  delay(50); //Evitar rebotes
}
