//Temporizador mostrar datos
#include "Timer.h"                     //http://github.com/JChristensen/Timer

//Librerías pantalla LCD
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define PIN_LDR A0
#define PIN_BOTON_A D3

long puntuacion = 0;
int pulsaciones_a;

boolean anterior_a;

//timer
Timer t;

//Objeto pantalla
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

void setup() {
  Serial.begin(9600);

  pinMode(PIN_BOTON_A, INPUT);
  anterior_a = digitalRead(PIN_BOTON_A);

  t.every(500, muestra_datos);

  //Inicializo pantalla
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
}

void loop() {
  t.update();
  
  boolean estado_a = digitalRead(PIN_BOTON_A);

  if (anterior_a != estado_a) {
    if (estado_a == LOW) {
      Serial.println(F("Pulsado boton A"));
      pulsaciones_a++;
      if (pulsaciones_a > 8) {
        puntuacion = 0;
        pulsaciones_a = 1;
      }
      int sensorValue = analogRead(PIN_LDR);
      puntuacion += sensorValue * 10;
    }
  }

  anterior_a = estado_a;
  delay(50); //Evitar rebotes
}


void muestra_datos()
{
  Serial.println("Pulsaciones: " + String(pulsaciones_a));
  Serial.println("Puntuación: " + String(puntuacion));
  Serial.println("Valor LDR: " + String(analogRead(PIN_LDR)));

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.print("Pulsaciones: ");
  display.print(pulsaciones_a);
  display.setCursor(0, 15);
  display.print("CPuntuación: ");
  display.print(puntuacion);
  display.display();
}
