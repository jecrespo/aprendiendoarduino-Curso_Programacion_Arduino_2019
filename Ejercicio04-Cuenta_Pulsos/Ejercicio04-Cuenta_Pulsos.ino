#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

#define PIN_BOTON_A 2
#define PIN_BOTON_B 3

//Variables globales
int pulsos_a = 0;
int pulsos_b = 0;
boolean anterior_a;
boolean anterior_b;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(PIN_BOTON_A, INPUT_PULLUP);
  pinMode(PIN_BOTON_B, INPUT);

  anterior_a = digitalRead(PIN_BOTON_A);
  anterior_b = digitalRead(PIN_BOTON_B);
}

void loop() {
  boolean estado_a = digitalRead(PIN_BOTON_A);
  boolean estado_b = digitalRead(PIN_BOTON_B);

  if (anterior_a != estado_a) {
    if (estado_a == LOW) {  //flanco descendente pull-up
      pulsos_a++;
      imprimePulsaciones();
    }
  }

  if (anterior_b != estado_b) {
    if (estado_b == HIGH) {  //flanco ascendente pull-down
      pulsos_b++;
      imprimePulsaciones();
    }
  }
  anterior_a = estado_a;
  anterior_b = estado_b;
  delay(50); //Evitar rebotes
}

void imprimePulsaciones() {
  Serial.println("Contador A: " + (String)pulsos_a + "\tContador B: " + (String)pulsos_b);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Contador A: ");
  lcd.print(pulsos_a);
  lcd.setCursor(0, 1);
  lcd.print("Contador B:");
  lcd.print(pulsos_b);
}
