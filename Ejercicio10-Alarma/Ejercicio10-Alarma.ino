//Librería DHT11
#include <SimpleDHT.h>

#define PIN_BUZZER 8
#define PIN_DHT 11
#define UMBRAL 24
#define NOTE_C5  523  //Frecuencia de sonido del buzzer
#define PIN_LED_1 4
#define PIN_LED_2 5 //PWM
#define PIN_LED_3 6 //PWM
#define PIN_LED_4 7

boolean alarma_temperatura;

//Objeto DHT11
SimpleDHT11 dht11(PIN_DHT);

void setup() {
  Serial.begin(9600);

  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_LED_3, OUTPUT);
  pinMode(PIN_LED_4, OUTPUT);

  if (lee_temperatura () > UMBRAL) {
    alarma_temperatura = true;
    tone(PIN_BUZZER, NOTE_C5);
    Serial.println("ALARMA!!");
  }
  else {
    alarma_temperatura = false;
    noTone(PIN_BUZZER);
  }
}

void loop() {
  int temp = lee_temperatura ();
  if (temp > UMBRAL) {
    if (!alarma_temperatura) {
      alarma_temperatura = true;
      tone(PIN_BUZZER, NOTE_C5);
      Serial.println("ALARMA!!");
    }
  }
  else {
    if (alarma_temperatura) {
      alarma_temperatura = false;
      noTone(PIN_BUZZER);
      Serial.println("TEMPERATURA NORMAL");
    }
  }
  //Actualizar LEDs
  Serial.println(temp);
  switch (temp) {
    case 27:
      digitalWrite(PIN_LED_4, HIGH);
      digitalWrite(PIN_LED_3, HIGH);
      digitalWrite(PIN_LED_2, HIGH);
      digitalWrite(PIN_LED_1, HIGH);
      break;
    case 26:
      digitalWrite(PIN_LED_3, HIGH);
      digitalWrite(PIN_LED_2, HIGH);
      digitalWrite(PIN_LED_1, HIGH);
      break;
    case 25:
      digitalWrite(PIN_LED_2, HIGH);
      digitalWrite(PIN_LED_1, HIGH);
    case 24:
      digitalWrite(PIN_LED_1, HIGH);
      break;
  }
  if (temp < 24) {
    digitalWrite(PIN_LED_1, LOW);
    digitalWrite(PIN_LED_2, LOW);
    digitalWrite(PIN_LED_3, LOW);
    digitalWrite(PIN_LED_4, LOW);
  }
}

float lee_temperatura () {

  byte temperature = 0;
  byte humidity = 0;
  int err = dht11.read(&temperature, &humidity, NULL);

  if (err != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err=");
    temperature = 0;
  }
  else {
    Serial.print("Temperatura: ");
    Serial.print(temperature);
    Serial.print(", Humedad:");
    Serial.println(humidity);
  }
  delay(1500);
  return temperature;
}
