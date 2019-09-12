#include "Arduino.h"

class DetectaFlanco {
  private:
    int _pin;
    boolean _anterior_estado;
    boolean _estado;

  public:
    DetectaFlanco(int pin);
    void inicio(int input);	//solo admitre los parámetros INPUT e INPUT_PULLUP
    int comprueba();
};
