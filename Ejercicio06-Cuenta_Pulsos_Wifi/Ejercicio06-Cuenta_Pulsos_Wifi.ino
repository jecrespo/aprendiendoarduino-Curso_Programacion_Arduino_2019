#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "ESPapXXXX";
const char *password = "passwordXXXX";

ESP8266WebServer server(80);

//Librería Pantalla
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define PIN_BOTON_B D3

//Define pantalla
#define OLED_RESET 0  // GPIO0

//Objeto Pantalla
Adafruit_SSD1306 display(OLED_RESET);

static boolean anterior_b;
int pulsos_b = 0;

unsigned long tiempo_pulsos = 0;

void setup() {
  Serial.begin(115200);

  //Configure pulsador
  pinMode(PIN_BOTON_B, INPUT);

  anterior_b = digitalRead(PIN_BOTON_B);

  //Pantalla
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)

  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  boolean estado_b = digitalRead(PIN_BOTON_B);

  if (anterior_b != estado_b) {
    if (estado_b == HIGH) {  //flanco ascendente pull-down
      Serial.println(F("Pulsado boton B"));
      pulsos_b++;
      Serial.println("Num. pulsaciones: " + (String)pulsos_b);
      int tiempo = (millis() - tiempo_pulsos);
      tiempo_pulsos = millis();
      Serial.println("Tiempo entre pulsos: " + (String)tiempo + " ms");

      //Imprimir en pantalla oled
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.setTextColor(WHITE);
      display.print("Num: ");
      display.print(pulsos_b);
      display.setCursor(0, 20);
      display.print("T: ");
      display.print(tiempo);
      display.print(" ms");
      display.display();
    }
  }
  anterior_b = estado_b;
  delay(50); //Evitar rebotes
}

void handleRoot() {
  String web = "<!DOCTYPE html>\n<html>\n<head><meta http-equiv=\"refresh\" content=\"60\" /></head>\n";
  web += "<body>\n";
  web += "<h1>Pulsaciones: </h1>" + String(pulsos_b) + "<h1>Tiempo entre pulsos: </h1>" + String(tiempo_pulsos);
  web += "</body>\n</html>\n";
  server.send(200, "text/html", web);
  server.sendHeader("refresh","60");
}
