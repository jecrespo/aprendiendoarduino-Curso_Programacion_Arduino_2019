/*
   Ejercicio01 - Test del entrono de trabajo para el curso Programación con Arduino
   Web: http://www.aprendiendoarduino.com/programacion-arduino/
   Montaje: https://aprendiendoarduino.files.wordpress.com/2017/07/curso_programacion-montaje_bb.jpg
*/

//Librerías necesarias para Ethernet Shield
#include <Ethernet.h>
#include <SPI.h>

//Librería Servo
#include <Servo.h>

//PINES
#define PIN_BOTON_A 2
#define PIN_BOTON_B 3
#define PIN_LED_1 4
#define PIN_LED_2 5 //PWM
#define PIN_LED_3 6 //PWM
#define PIN_LED_4 7
#define PIN_BUZZER 8
#define PIN_SERVO 9
#define PIN_POT A0
#define PIN_LDR A1
#define PIN_TMP36 A2

#define NOTE_C5  523  //Frecuencia de sonido del buzzer

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xYY}; //Sustituir YY por el numero de KIT de Arduino
EthernetServer server = EthernetServer(80);

Servo miservo;

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

  //Inicializo Ethernet Shield
  Serial.println(F("Inicializando red..."));

  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Fallo en la configuracion Ethernet usando DHCP. Continuando resto de Tests"));
  }
  else {
    Serial.print(F("IP asignada por DHCP: "));
    Serial.println(Ethernet.localIP());
  }

}

void loop() {
  muestraMenu();
  int respuesta = recogeRespuesta();

  switch (respuesta) {
    case 1:
      muestraIP();
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
    case 10:
      pruebaClienteWeb();
      break;
    case 11:
      pruebaServidorWeb();
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
  Serial.println(F("1 - Muestra IP Arduino"));
  Serial.println(F("2 - Prueba Botones (comprobar pulsaciones correctas)"));
  Serial.println(F("3 - Prueba LEDs (Iluminación LEDs)"));
  Serial.println(F("4 - Prueba PWM (No todos los LEDs son PWM)"));
  Serial.println(F("5 - Prueba Potenciometro (comprobar que va de 0 a 1023)"));
  Serial.println(F("6 - Prueba LDR (ver valores máximo y mínimo. Anotarlos)"));
  Serial.println(F("7 - Sonda temperatura (comprueba valores)"));
  Serial.println(F("8 - Test Servo (comprobar el movimiento completo)"));
  Serial.println(F("9 - Test Buzzer (comprobar sonido)"));
  Serial.println(F("10 - Test Cliente Web (comprobar respuesta del servidor)"));
  Serial.println(F("11 - Test Servidor Web (comprobar servidor embebido en Arduino)"));
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

void muestraIP() {
  Serial.println(F("Muestra IP Arduino."));
  Serial.print(F("IP asignada por DHCP: "));
  Serial.println(Ethernet.localIP());
}

void pruebaBotones() {
  Serial.println(F("Prueba Botones"));
  Serial.println(F("Pulsar 'q' para salir."));
  int pulsos_a = 0;
  int pulsos_b = 0;
  static boolean anterior_a = digitalRead(PIN_BOTON_A);
  static boolean anterior_b = digitalRead(PIN_BOTON_B);

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
}

void pruebaLEDs() {
  Serial.println(F("Prueba LEDs"));
  Serial.println(F("Pulsar 'q' para salir."));

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
}

void pruebaPWM() {
  Serial.println(F("Prueba PWM"));
  Serial.println(F("Pulsar 'q' para salir."));

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
}

void pruebaPotenciometro() {
  Serial.println(F("Prueba Potenciometro, ver en Serial Plotter"));
  Serial.println(F("Pulsar 'q' para salir."));

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
}

void pruebaLDR() {
  Serial.println(F("Prueba LDR, ver en Serial Plotter"));
  Serial.println(F("Pulsar 'q' para salir."));
  int maximo = 0;
  int minimo = 1023;

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
}

void pruebaTemperatura() {
  Serial.println(F("Sonda temperatura, ver en monitor serie"));
  Serial.println(F("Pulsar 'q' para salir."));

  while (true) {
    //Compruebo si llega una q
    if (Serial.available() > 0) {
      char caracter_leido = Serial.read();
      if (caracter_leido == 'q')
        break;
    }

    int sensorVal = analogRead(PIN_TMP36);
    float voltage = (sensorVal / 1023.0) * 5.0;
    float temperature = (voltage - 0.5) * 100;
    Serial.println(temperature);
    delay(500);
  }
}

void pruebaServo() {
  Serial.println(F("Test Servo"));
  Serial.println(F("Pulsar 'q' para salir."));

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
}

void pruebaBuzzer() {
  Serial.println(F("Test Buzzer"));
  Serial.println(F("Pulsar 'q' para salir."));

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
}

void  pruebaClienteWeb() {
  Serial.println("Test Cliente Web");

  EthernetClient client;
  char url_aprendiendoarduino[] = "www.aprendiendoarduino.com";

  //Mando peticiñon web
  if (client.connect(url_aprendiendoarduino, 80)) {
    Serial.println(F("connected"));
    client.println(F("GET /servicios/aprendiendoarduino/ HTTP/1.1"));
    client.println(F("Host: www.aprendiendoarduino.com"));
    client.println(F("Connection: close"));
    client.println();
  }
  else {
    Serial.println(F("connection failed"));
  }

  //Espero respuesta del servidor
  while (client.available() == 0) {
    //nada
  }

  if (client.available()) {
    Serial.println(F("Respuesta del Servidor---->"));
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
    client.stop();
  }
}

void  pruebaServidorWeb() {
  Serial.print(F("Test Servidor Web. Para probar conectarse a la IP: "));
  Serial.println(Ethernet.localIP());
  Serial.println(F("Pulsar 'q' para salir."));

  while (true) {
    //Compruebo si llega una q
    if (Serial.available() > 0) {
      char caracter_leido = Serial.read();
      if (caracter_leido == 'q')
        break;
    }

    EthernetClient client = server.available();
    if (client > 0) {
      //Leer la petición y sacarla por el puerto serie
      while (client.available() > 0) {
        char inChar = client.read();
        Serial.print(inChar);
      }
      client.println(F("HTTP/1.0 200K"));
      client.println();
      client.print(F("Servidor embebido del Arduino con IP: "));
      client.println(Ethernet.localIP());
      client.println();
      client.stop();
    }
  }
}
