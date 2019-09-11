/*
  Menu interactivo
*/

//Librerías pantalla LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int led = 0;

#define PIN_LED_1 4
#define PIN_LED_2 5 //PWM
#define PIN_LED_3 6 //PWM
#define PIN_LED_4 7
#define PIN_BUZZER 8

#define NOTE_C5  523  //Frecuencia de sonido del buzzer

int leds[] = {PIN_LED_1, PIN_LED_2, PIN_LED_3, PIN_LED_4};

//Objeto pantalla
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  Serial.begin(9600);

  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_LED_3, OUTPUT);
  pinMode(PIN_LED_4, OUTPUT);

  //Inicializo pantalla
  Serial.println(F("Inicializando pantalla..."));
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Inicializando...");
  delay(2000);
  lcd.clear();

  String titulo = "Menu Interactivo con Arduino";
  for (int i = 0; i < titulo.length(); i++) {
    Serial.print("-");
  }
  Serial.print("\n" + titulo + "\n");
  for (int i = 0; i < titulo.length(); i++) {
    Serial.print("-");
  }
  Serial.println();
}

void loop() {
  int respuesta;
  do {
    respuesta = menu();
    switch (respuesta) {
      case 1:
        funcion1(led);
        break;
      case 2:
        funcion2(led);
        break;
      case 3:
        funcion3();
        break;
      case 4:
        Serial.println("Adios...");
        while (true) {
          //se queda aqui el programa
        }
        break;
      default:
        Serial.println("Opcion de menu incorrecta!!!!!");
        break;
    }
    limpia_terminal();
  } while ((respuesta < 1) || (respuesta > 4));
}

int menu() {
  String leido = "";

  Serial.println("Elige opcion a ejecutar:");
  Serial.println("1 - Encender siguiente led");
  Serial.println("2 - Led Encendido");
  Serial.println("3 - Sonar Buzzer 5 seg.");
  Serial.println("4 - Fin");

  while (Serial.available() == 0) {
    //Ho hago nada
  }

  do {
    char caracter_leido;
    caracter_leido = Serial.read();
    leido += caracter_leido;
    delay(5);
  }  while (Serial.available() > 0);

  if (leido.toInt() != 0)
    return leido.toInt();
  else return 0;

}

void limpia_terminal() {    //Solo v�lido para usar con putty
  Serial.write(27);       // ESC command
  Serial.print("[2J");    // clear screen command
  Serial.write(27);
  Serial.print("[H");     // cursor to home command
}

void funcion1(int& num) {
  if (num > 3)
    num = 0;

  if (num == 0)
    digitalWrite(leds[3], LOW);
  else
    digitalWrite(leds[num - 1], LOW);

  digitalWrite(leds[num], HIGH);
  num++;
}

void funcion2(int num) {
  lcd.setCursor(0, 0);
  lcd.print("Led Ecendido: ");
  lcd.print(num);
}

void funcion3() {
  tone(PIN_BUZZER, NOTE_C5);
  delay(5000);
  noTone(PIN_BUZZER);
}
