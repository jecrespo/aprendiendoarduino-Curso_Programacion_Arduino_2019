//Temporizador mostrar datos
#include <MsTimer2.h>
//Librerías pantalla LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define PIN_LDR A1
#define PIN_BOTON_A 2

long puntuacion = 0;
int pulsaciones_a;

boolean anterior_a;

//Objeto pantalla
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  Serial.begin(9600);

  pinMode(PIN_BOTON_A, INPUT_PULLUP);
  anterior_a = digitalRead(PIN_BOTON_A);

  MsTimer2::set(500, muestra_datos); // 500ms period
  MsTimer2::start();

  //Inicializo pantalla
  lcd.init();
  lcd.backlight();
}

void loop() {
  boolean estado_a = digitalRead(PIN_BOTON_A);

  if (anterior_a != estado_a) {
    if (estado_a == LOW) {  //flanco descendente pull-up
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

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pulsaciones: ");
  lcd.print(pulsaciones_a);
  lcd.setCursor(0, 1);
  lcd.print("CPuntuación: ");
  lcd.print(puntuacion);
}
