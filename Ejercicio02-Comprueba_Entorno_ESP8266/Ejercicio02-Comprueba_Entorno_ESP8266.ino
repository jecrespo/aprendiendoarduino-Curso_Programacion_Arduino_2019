/*
   Ejercicio02 - Test del entorno ESP8266 de trabajo para el curso Programación con Arduino
   Web: https://www.aprendiendoarduino.com/cursos/programacion-arduino-2019/
   Montaje: https://github.com/jecrespo/aprendiendoarduino-Curso_Programacion_Arduino_2019/blob/master/Esquemas%20Montajes%20Pr%C3%A1cticas%20Fritzing/Curso_Programacion-Montaje%20Wemos_bb.png
*/

//Librería Wifi
#include "ESP8266WiFi.h"

//Librería Pantalla
#include <Wire.h>
#include <Adafruit_SSD1306.h>

//Librería LEDs RGB
#include <Adafruit_NeoPixel.h>

//Define pines
#define PIN_BOTON_B D3
#define PIN_RELE D1
#define PIN_RGB D4
#define LED_NUM 7

//Define pantalla
#define OLED_RESET 0  // GPIO0

//Objeto Pantalla
Adafruit_SSD1306 display(OLED_RESET);

//Objeto Leds
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_NUM, PIN_RGB, NEO_GRB + NEO_KHZ800);

//Datos WiFi
const char* ssid = "your-ssid";
const char* password = "your-password";

// Create an instance of the server
WiFiServer server(80);

boolean led_state = LOW;

void setup() {
  Serial.begin(115200);

  leds.begin(); // This initializes the NeoPixel library.

  //Configure LED BUILTIN
  pinMode(LED_BUILTIN, OUTPUT);
  
  //Configure pulsador
  pinMode(PIN_BOTON_B, INPUT);

  //Pantalla
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)

  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  muestraMenu();
  int respuesta = recogeRespuesta();

  switch (respuesta) {
    case 1:
      muestraIP();
      break;
    case 2:
      pruebaClienteWeb();
      break;
    case 3:
      pruebaServidorWeb();
      break;
    case 4:
      pruebaBoton();
      break;
    case 5:
      pruebaRele();
      break;
    case 6:
      pruebaLedsRGB();
      break;
    case 7:
      pruebaPantalla();
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
  Serial.println(F("2 - Test Cliente Web (comprobar respuesta del servidor)"));
  Serial.println(F("3 - Test Servidor Web (comprobar servidor embebido en Arduino)"));
  Serial.println(F("4 - Prueba Pulsador (comprobar pulsaciones correctas)"));
  Serial.println(F("5 - Prueba Relé Shield"));
  Serial.println(F("6 - Prueba Neopixel Shield"));
  Serial.println(F("7 - Prueba Pantalla Oled Shield"));
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

    Serial.println("\nHe Leido la cadena: " + cadena_leida);

    int respuesta = cadena_leida.toInt();
    return respuesta;
  }
}

void muestraIP() {
  // Print the IP address
  Serial.print("IP asignada: ");
  Serial.println(WiFi.localIP());
}

void pruebaClienteWeb() {
  const uint16_t port = 80;
  const char * host = "www.aprendiendoarduino.com"; // ip or dns

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(5000);
    return;
  }

  // This will send the request to the server
  client.println("GET /servicios/aprendiendoarduino/ HTTP/1.0");
  client.println("Host: www.aprendiendoarduino.com");
  client.println();

  int timeout_flag = 0;

  while (client.available() == 0) {
    static int count = 0;
    Serial.print(".");
    delay(250);
    if (count > 12) //waiting more than 3 seconds
      timeout_flag = 1;
    break;
  }
  Serial.println();

  if (timeout_flag == 0) {
    Serial.println("Connection Successful. Server Answer:");
    //read all lines from server
    do {
      String line = client.readStringUntil('\r'); //read line by line
      Serial.print(line);
    } while (client.available() > 0);
    Serial.println();
  }
  else {
    Serial.println("Connection error. Server doesn't respond");
  }

  Serial.println("closing connection");
  client.stop();
}

void pruebaServidorWeb() {
  Serial.print(F("Test Servidor Web. Para probar conectarse a la IP: "));
  Serial.println(WiFi.localIP());
  Serial.println(F("Pulsar 'q' para salir."));

  while (true) {
    //Compruebo si llega una q
    if (Serial.available() > 0) {
      char caracter_leido = Serial.read();
      if (caracter_leido == 'q')
        break;
    }

    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client) {
      continue;
    }

    // Wait until the client sends some data
    Serial.println("new client");
    while (!client.available()) {
      delay(1);
    }

    // Read the first line of the request
    String req = client.readStringUntil('\r');
    Serial.println(req);
    client.flush();

    // Match the request
    if (req.indexOf("GET / HTTP/1.1") != -1)
      muestraWeb(client);
    else if (req.indexOf("POST / HTTP/1.1") != -1) {
      Serial.println("------> Cambio estado LED");
      led_state = !led_state;
      digitalWrite(LED_BUILTIN, led_state);  //cambio el estado del led
      muestraWeb(client);
    }
    else {
      Serial.println("invalid request");
      client.stop();
    }

    client.flush();

    delay(1);
    Serial.println("Client disonnected");
  }
}

void pruebaBoton() {
  Serial.println(F("Prueba Pulsador"));
  Serial.println(F("Pulsar 'q' para salir."));
  int pulsos_b = 0;
  static boolean anterior_b = digitalRead(PIN_BOTON_B);

  while (true) {
    //Compruebo si llega una q
    if (Serial.available() > 0) {
      char caracter_leido = Serial.read();
      if (caracter_leido == 'q')
        break;
    }

    boolean estado_b = digitalRead(PIN_BOTON_B);

    if (anterior_b != estado_b) {
      if (estado_b == HIGH) {  //flanco ascendente pull-down
        Serial.println(F("Pulsado boton B"));
        pulsos_b++;
        Serial.println("Numero pulsaciones boton B: " + (String)pulsos_b);
      }
    }
    anterior_b = estado_b;
    delay(50); //Evitar rebotes
  }
}

void pruebaRele() {
  pinMode(PIN_RELE, OUTPUT);
  Serial.println(F("Prueba Relé"));
  Serial.println(F("Pulsar 'q' para salir."));

  while (true) {
    //Compruebo si llega una q
    if (Serial.available() > 0) {
      char caracter_leido = Serial.read();
      if (caracter_leido == 'q')
        break;
    }
    digitalWrite(PIN_RELE, HIGH);
    delay(2000);
    digitalWrite(PIN_RELE, LOW);
    delay(2000);
  }
}

void pruebaLedsRGB() {
  Serial.println(F("Prueba Leds RGB"));
  Serial.println(F("Pulsar 'q' para salir."));

  while (true) {
    //Compruebo si llega una q
    if (Serial.available() > 0) {
      char caracter_leido = Serial.read();
      if (caracter_leido == 'q')
        break;
    }

    led_set(10, 0, 0);//red
    led_set(0, 0, 0);

    led_set(0, 10, 0);//green
    led_set(0, 0, 0);

    led_set(0, 0, 10);//blue
    led_set(0, 0, 0);
  }
}

void pruebaPantalla() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.print("Test de Pantalla!!!");
  display.display();
  delay(2000);
  uint8_t color = 1;
  for (int16_t i = 0; i < display.height() / 2; i += 3) {
    // alternate colors
    display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, color % 2);
    display.display();
    delay(1);
    color++;
  }
  delay(2000);
  display.clearDisplay();
  display.display();
}

void muestraWeb(WiFiClient client) {
  client.println("HTTP/1.0 200K");
  client.println();
  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<body>");
  if (led_state == HIGH) client.println("<p>LED ENCENDIDO</p>");
  else client.println("<p>LED APAGADO</p>");
  client.println("<form action=\"\" method=\"post\">");
  if (led_state == HIGH) client.println("<input type=\"submit\" value=\"Apaga Led\" />");
  else client.println("<input type=\"submit\" value=\"Enciende Led\" />");
  client.println("</form>");
  client.println("</body>");
  client.println("</html>");
  client.println();
  client.stop();
  client.flush();
}

void led_set(uint8 R, uint8 G, uint8 B) {
  for (int i = 0; i < LED_NUM; i++) {
    leds.setPixelColor(i, leds.Color(R, G, B));
    leds.show();
    delay(50);
  }
}
