//No funciona usando una función para dos botones
//Se soluciona con una clase detectaFlanco

#define PIN_BOTON_A 2
#define PIN_BOTON_B 3

void setup() {
  Serial.begin(9600);

}

void loop() {
  int flanco_A = detectaFlanco(PIN_BOTON_A);
  int flanco_B = detectaFlanco(PIN_BOTON_B);

  if (flanco_A == 1)
    Serial.println("Flanco asc A");
  if (flanco_A == -1)
    Serial.println("Flanco desc A");
  if (flanco_B == 1)
    Serial.println("Flanco asc B");
  if (flanco_B == -1)
    Serial.println("Flanco asc B");

}

int detectaFlanco(int pin) {
  //Devuelve 1 flanco ascendente, -1 flanco descendente y 0 si no hay nada
  static boolean anterior_estado = digitalRead(pin);
  boolean estado = digitalRead(pin);

  if (anterior_estado != estado) {
    if (estado == HIGH) {
      anterior_estado = estado;
      return 1;
    }
    else {
      anterior_estado = estado;
      return -1;
    }
  }
  else {
    return 0;
  }
}
