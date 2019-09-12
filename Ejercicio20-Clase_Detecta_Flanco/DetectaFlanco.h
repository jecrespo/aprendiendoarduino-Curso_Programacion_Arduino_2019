class DetectaFlanco {
  private:
    int _pin;
    boolean _anterior_estado;
    boolean _estado;

  public:
    DetectaFlanco(int pin) {
      _pin = pin ;
    }

    void inicio(int input) {  //solo admitre los parámetros INPUT e INPUT_PULLUP
      pinMode(_pin, input);
      _anterior_estado = digitalRead(_pin);
    }

    int comprueba() {
      _estado = digitalRead(_pin);

      if (_anterior_estado != _estado) {
        if (_estado == HIGH) {
          _anterior_estado = _estado;
          return 1; //Flanco Ascendente
        }
        else {
          _anterior_estado = _estado;
          return -1; //Flanco Descendente
        }
      }
      else
        return 0;
    }
};
