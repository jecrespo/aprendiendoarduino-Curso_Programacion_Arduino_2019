#define PIN_BOTON_A 2
#define PIN_LED_1 4
#define PIN_LED_2 5 //PWM
#define PIN_LED_3 6 //PWM
#define PIN_LED_4 7

unsigned long duracion = 0;
boolean estado_luces = 0; //0 = apagado, 1 = encendido
byte pin_encendido = PIN_LED_1;

void setup() {
  Serial.begin(9600);

  //Inicializo Pines
  Serial.println(F("Inicializando pines digitales..."));
  pinMode(PIN_BOTON_A, INPUT_PULLUP);
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_LED_3, OUTPUT);
  pinMode(PIN_LED_4, OUTPUT);
}

void loop() {
  duracion = pulseInLong(PIN_BOTON_A, LOW, 20000000); //el programa se bloquea en este punto
  Serial.println(duracion / 1000); //en milisegundos
  delay(50);
  if (duracion == 0) {
    Serial.println("pulsacion no detectada en 20 segundos");
  }
  else if (duracion > 2000000) {
    Serial.println("pulsacion larga");
    if (estado_luces == 0) {
      estado_luces = 1;
      digitalWrite(pin_encendido, HIGH);
      Serial.println("Enciendo pin " + (String)pin_encendido);
    }
    else {
      estado_luces = 0;
      digitalWrite(pin_encendido, LOW);
      Serial.println("Apago Leds");
      pin_encendido = PIN_LED_1; //inicializo el led a iluminar
    }
  }
  else {
    Serial.println("pulsacion corta");
    if (estado_luces == 1) {
      digitalWrite(pin_encendido, LOW);
      Serial.println("Apago pin " + (String)pin_encendido);
      pin_encendido++;
      if (pin_encendido > PIN_LED_4)
        pin_encendido = PIN_LED_1;
      digitalWrite(pin_encendido, HIGH);
      Serial.println("Enciendo pin " + (String)pin_encendido);
    }
  }
}
