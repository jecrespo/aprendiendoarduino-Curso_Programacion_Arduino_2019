class Contador {
  private:
    int N ;

  public:
    Contador() {
      N = 0 ;
    }

    void SetContador( int n) {
      N = n ;
    }

    void Incrementar() {
      N++ ;
    }

    int GetCont() {
      return (N) ;
    }
};
