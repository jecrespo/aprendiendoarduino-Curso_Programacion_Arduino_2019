//Librería LEDs RGB
#include <Adafruit_NeoPixel.h>

//Define pines
#define PIN_BOTON_B D3
#define PIN_RGB D4
#define LED_NUM 7

//Objeto Leds
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_NUM, PIN_RGB, NEO_GRB + NEO_KHZ800);

void setup() {
  //Inicializo puerto serie
  Serial.begin(115200);

  //Configure pulsador
  pinMode(PIN_BOTON_B, INPUT);

  leds.begin(); // This initializes the NeoPixel library.

  //Inicializo Pines
  Serial.println(F("Inicializando Dado digital..."));
  led_set(10, 0, 0);//red
  delay(250);
  led_set(0, 10, 0);//green
  delay(250);
  led_set(0, 0, 10);//blue
  delay(250);
  led_set(10, 10, 0);
  delay(250);
  led_set(10, 0, 10);
  delay(250);
  led_set(0, 10, 10);
  delay(250);
  led_set(10, 10, 10);
  delay(250);

  randomSeed(analogRead(A0)); //genero una nueva semilla de un pin analógico al aire
}

void loop() {
  if (detectaFlanco(PIN_BOTON_B) == 1) {

    byte randNumber = random(1, LED_NUM);
    Serial.println(randNumber);
    iluminaLed(randNumber);
  }
  delay(50); //evitar rebotes
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
    anterior_estado = estado;
    return 0;
  }
}

void iluminaLed(int num) {
  for (int velocidad = 20; velocidad <= 200; velocidad += 20) {
    for (int i = 1; i < LED_NUM; i++) {
      leds.setPixelColor(i, leds.Color(0, 10, 0));
      leds.show();
      delay(velocidad);
      leds.setPixelColor(i, leds.Color(0, 0, 0));
      leds.show();
    }
  }
  for (int i = 1; i <= num; i++) {
    leds.setPixelColor(i, leds.Color(0, 10, 0));
    leds.show();
    delay(250);
    if (i == num)
      break;
    leds.setPixelColor(i, leds.Color(0, 0, 0));
    leds.show();
  }
}

void led_set(uint8 R, uint8 G, uint8 B) {
  for (int i = 1; i < LED_NUM; i++) {
    leds.setPixelColor(i, leds.Color(R, G, B));
  }
  leds.show();
}
