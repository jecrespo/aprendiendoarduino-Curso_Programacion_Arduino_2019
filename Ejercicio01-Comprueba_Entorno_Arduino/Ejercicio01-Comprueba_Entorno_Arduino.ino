/*
   Ejercicio01 - Test del entrono de trabajo para el curso Programación con Arduino
   Web: https://www.aprendiendoarduino.com/cursos/programacion-arduino-2019/
   Montaje: https://github.com/jecrespo/aprendiendoarduino-Curso_Programacion_Arduino_2019/blob/master/Esquemas%20Montajes%20Pr%C3%A1cticas%20Fritzing/Curso_Programacion-Montaje%20Arduino_bb.png
*/

//Librerías pantalla LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Librería Servo
#include <Servo.h>

//Librería DHT11
#include <SimpleDHT.h>

//PINES
#define PIN_BOTON_A 2
#define PIN_BOTON_B 3
#define PIN_LED_1 4
#define PIN_LED_2 5 //PWM
#define PIN_LED_3 6 //PWM
#define PIN_LED_4 7
#define PIN_BUZZER 8
#define PIN_SERVO 9
#define PIN_DHT11 11
#define PIN_POT A0
#define PIN_LDR A1

#define NOTE_C5  523  //Frecuencia de sonido del buzzer

//Objeto servo
Servo miservo;

//Objeto DHT11
SimpleDHT11 dht11(PIN_DHT11);

//Objeto pantalla
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  //Inicializo puerto serie
  Serial.begin(9600);

  //Inicializo Pines
  Serial.println(F("Inicializando pines digitales..."));
  pinMode(PIN_BOTON_A, INPUT_PULLUP);
  pinMode(PIN_BOTON_B, INPUT);
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_LED_3, OUTPUT);
  pinMode(PIN_LED_4, OUTPUT);

  //Inicializo Servo
  Serial.println(F("Inicializando servo..."));
  miservo.attach(PIN_SERVO);

  //Inicializo pantalla
  Serial.println(F("Inicializando pantalla..."));
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Inicializando...");
  delay(2000);
  lcd.clear();

}

void loop() {
  muestraMenu();
  int respuesta = recogeRespuesta();

  switch (respuesta) {
    case 1:
      muestraTexto();
      break;
    case 2:
      pruebaBotones();
      break;
    case 3:
      pruebaLEDs();
      break;
    case 4:
      pruebaPWM();
      break;
    case 5:
      pruebaPotenciometro();
      break;
    case 6:
      pruebaLDR();
      break;
    case 7:
      pruebaTemperatura();
      break;
    case 8:
      pruebaServo();
      break;
    case 9:
      pruebaBuzzer();
      break;
    default:
      Serial.println(F("Opcion incorrecta.\nVuelve a introducir test a realizar"));
      break;
  }
}

void muestraMenu() {
  Serial.println(F("\n------------------------"));
  Serial.println(F("MENU"));
  Serial.println(F("Selecciona el test a ejecutar"));
  Serial.println(F("1 - Muestra Texto Pantalla"));
  Serial.println(F("2 - Prueba Botones (comprobar pulsaciones correctas)"));
  Serial.println(F("3 - Prueba LEDs (Iluminación LEDs)"));
  Serial.println(F("4 - Prueba PWM (No todos los LEDs son PWM)"));
  Serial.println(F("5 - Prueba Potenciometro (comprobar que va de 0 a 1023)"));
  Serial.println(F("6 - Prueba LDR (ver valores máximo y mínimo. Anotarlos)"));
  Serial.println(F("7 - Sonda temperatura (comprueba valores)"));
  Serial.println(F("8 - Test Servo (comprobar el movimiento completo)"));
  Serial.println(F("9 - Test Buzzer (comprobar sonido)"));
  Serial.println(F("------------------------"));
}

int recogeRespuesta() {
  int segundos = 0;
  String cadena_leida = "";

  while (Serial.available() == 0) {
    if (segundos > 10) {
      segundos = 0;
      Serial.println(F("\nEsperando respuesta..."));
      muestraMenu();
    }
    Serial.print(".");
    segundos++;
    delay(1000);
  }

  if (Serial.available() > 0) {
    do {
      char caracter_leido = Serial.read();
      cadena_leida += caracter_leido;
      delay(5);
    }  while (Serial.available() > 0);

    Serial.print("\nHe Leido la cadena: " + cadena_leida);

    int respuesta = cadena_leida.toInt();
    return respuesta;
  }
}

void muestraTexto() {
  Serial.println(F("Muestra Texto en Pantalla LCD 16x2."));
  lcd.setCursor(0, 0);
  lcd.print("Prueba de Pantalla");
  lcd.setCursor(0, 1);
  lcd.print("Completada...");
  Serial.println(F("Pulsar 'q' para salir."));
  while (true) {
    //Compruebo si llega una q
    if (Serial.available() > 0) {
      char caracter_leido = Serial.read();
      if (caracter_leido == 'q')
        break;
    }
  }
  lcd.clear();
}

void pruebaBotones() {
  Serial.println(F("Prueba Botones"));
  Serial.println(F("Pulsar 'q' para salir."));
  int pulsos_a = 0;
  int pulsos_b = 0;
  static boolean anterior_a = digitalRead(PIN_BOTON_A);
  static boolean anterior_b = digitalRead(PIN_BOTON_B);
  lcd.setCursor(0, 0);
  lcd.print("Prueba Botones");
  lcd.setCursor(0, 1);
  lcd.print("q para salir");


  while (true) {
    //Compruebo si llega una q
    if (Serial.available() > 0) {
      char caracter_leido = Serial.read();
      if (caracter_leido == 'q')
        break;
    }

    boolean estado_a = digitalRead(PIN_BOTON_A);
    boolean estado_b = digitalRead(PIN_BOTON_B);

    if (anterior_a != estado_a) {
      if (estado_a == LOW) {  //flanco descendente pull-up
        Serial.println(F("Pulsado boton A"));
        pulsos_a++;
        Serial.println("Numero pulsaciones boton A: " + (String)pulsos_a);
      }
    }

    if (anterior_b != estado_b) {
      if (estado_b == HIGH) {  //flanco ascendente pull-down
        Serial.println(F("Pulsado boton B"));
        pulsos_b++;
        Serial.println("Numero pulsaciones boton B: " + (String)pulsos_b);
      }
    }
    anterior_a = estado_a;
    anterior_b = estado_b;
    delay(50); //Evitar rebotes
  }
  lcd.clear();
}

void pruebaLEDs() {
  Serial.println(F("Prueba LEDs"));
  Serial.println(F("Pulsar 'q' para salir."));
  lcd.setCursor(0, 0);
  lcd.print("Prueba Leds");
  lcd.setCursor(0, 1);
  lcd.print("q para salir");

  while (true) {
    //Compruebo si llega una q
    if (Serial.available() > 0) {
      char caracter_leido = Serial.read();
      if (caracter_leido == 'q')
        break;
    }

    for (int i = PIN_LED_1; i <= PIN_LED_4; i++) {
      Serial.println("Probando LED en PIN " + (String)i);
      digitalWrite(i, HIGH);
      delay(500);
      digitalWrite(i, LOW);
      delay(500);
    }
    for (int i = PIN_LED_4; i >= PIN_LED_1; i--) {
      Serial.println("Probando LED en PIN " + (String)i);
      digitalWrite(i, HIGH);
      delay(500);
      digitalWrite(i, LOW);
      delay(500);
    }
  }
  lcd.clear();
}

void pruebaPWM() {
  Serial.println(F("Prueba PWM"));
  Serial.println(F("Pulsar 'q' para salir."));
  lcd.setCursor(0, 0);
  lcd.print("Prueba PWM");
  lcd.setCursor(0, 1);
  lcd.print("q para salir");

  while (true) {
    //Compruebo si llega una q
    if (Serial.available() > 0) {
      char caracter_leido = Serial.read();
      if (caracter_leido == 'q')
        break;
    }

    for (int i = PIN_LED_1; i <= PIN_LED_4; i++) {
      Serial.println("Probando PWM en LED PIN " + (String)i);
      for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
        analogWrite(i, fadeValue);
        delay(30);
      }
      for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
        analogWrite(i, fadeValue);
        // wait for 30 milliseconds to see the dimming effect
        delay(30);
      }
    }
  }
  lcd.clear();
}

void pruebaPotenciometro() {
  Serial.println(F("Prueba Potenciometro, ver en Serial Plotter"));
  Serial.println(F("Pulsar 'q' para salir."));
  lcd.setCursor(0, 0);
  lcd.print("Prueba Potenciometro");
  lcd.setCursor(0, 1);
  lcd.print("q para salir");

  while (true) {
    //Compruebo si llega una q
    if (Serial.available() > 0) {
      char caracter_leido = Serial.read();
      if (caracter_leido == 'q')
        break;
    }

    Serial.println(analogRead(PIN_POT));
    delay(500);
  }
  lcd.clear();
}

void pruebaLDR() {
  Serial.println(F("Prueba LDR, ver en Serial Plotter"));
  Serial.println(F("Pulsar 'q' para salir."));
  int maximo = 0;
  int minimo = 1023;
  lcd.setCursor(0, 0);
  lcd.print("Prueba LDR");
  lcd.setCursor(0, 1);
  lcd.print("q para salir");

  while (true) {
    //Compruebo si llega una q
    if (Serial.available() > 0) {
      char caracter_leido = Serial.read();
      if (caracter_leido == 'q')
        break;
    }

    int sensorValue = analogRead(PIN_LDR);
    maximo = max(maximo, sensorValue);
    minimo = min(minimo, sensorValue);
    Serial.print(sensorValue);
    Serial.print(",");
    Serial.print(maximo);
    Serial.print(",");
    Serial.println(minimo);
    delay(500);
  }
  lcd.clear();
}

void pruebaTemperatura() {
  Serial.println(F("Sonda temperatura, ver en monitor serie"));
  Serial.println(F("Pulsar 'q' para salir."));
  lcd.setCursor(0, 0);
  lcd.print("Prueba Temperatura");
  lcd.setCursor(0, 1);
  lcd.print("q para salir");

  while (true) {
    //Compruebo si llega una q
    if (Serial.available() > 0) {
      char caracter_leido = Serial.read();
      if (caracter_leido == 'q')
        break;
    }
    byte temperature = 0;
    byte humidity = 0;
    int err = dht11.read(&temperature, &humidity, NULL);

    if (err != SimpleDHTErrSuccess) {
      Serial.print("Read DHT11 failed, err=");
      Serial.println(err);
    }
    else {
      Serial.print("Temperatura: ");
      Serial.print(temperature);
      Serial.print(", Humedad:");
      Serial.println(humidity);
    }
    delay(1500);
  }
  lcd.clear();
}

void pruebaServo() {
  Serial.println(F("Test Servo"));
  Serial.println(F("Pulsar 'q' para salir."));
  lcd.setCursor(0, 0);
  lcd.print("Prueba Servo");
  lcd.setCursor(0, 1);
  lcd.print("q para salir");

  while (true) {
    //Compruebo si llega una q
    if (Serial.available() > 0) {
      char caracter_leido = Serial.read();
      if (caracter_leido == 'q')
        break;
    }

    for (int pos = 20; pos <= 160; pos += 5) {
      // in steps of 1 degree
      miservo.write(pos);
      delay(30);
    }
    Serial.println(F("Giro de 20 a 160 grados"));

    for (int pos = 160; pos >= 20; pos -= 5) {
      miservo.write(pos);
      delay(30);
    }
    Serial.println(F("Giro de 160 a 20 grados"));
  }
  lcd.clear();
}

void pruebaBuzzer() {
  Serial.println(F("Test Buzzer"));
  Serial.println(F("Pulsar 'q' para salir."));
  lcd.setCursor(0, 0);
  lcd.print("Prueba Buzzer");
  lcd.setCursor(0, 1);
  lcd.print("q para salir");

  while (true) {
    static boolean output = HIGH;

    //Compruebo si llega una q
    if (Serial.available() > 0) {
      char caracter_leido = Serial.read();
      if (caracter_leido == 'q') {
        noTone(PIN_BUZZER);
        break;
      }
    }

    output ? tone(PIN_BUZZER, NOTE_C5) : noTone(PIN_BUZZER);
    output ? Serial.println(F("beep")) : Serial.println(F("no beep"));
    output = !output;
    delay(1000);
  }
  lcd.clear();
}
