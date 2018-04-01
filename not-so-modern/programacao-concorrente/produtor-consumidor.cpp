#include <iostream>
#include <cstdlib>

// C++11 :D
//#include <thread>

#include <cpthread>

class Produtor
{
private:
  char *recurso;
  sem_t *semaforoBufferCheio;
  sem_t *semaforoBufferVazio;

  void produzir() {
  }

public:
  Produtor(sem_t *semaforoBufferCheio, sem_t *semaforoBufferVazio, char *recurso)
    : semaforoBufferCheio(semaforoBufferCheio), semaforoBufferVazio(semaforoBufferVazio), recurso(recurso) {

  }

  virtual ~Produtor(){}

  void iniciar() {
    while(true) {
      randomWait();

      sem_wait(semaforoBufferCheio);
      produzir();
      sem_post(semaforoBufferVazio);
    }
  }
};

class Consumidor
{
private:
  char *recurso;
  sem_t *semaforoBufferCheio;
  sem_t *semaforoBufferVazio;

  void consumir() {
  }

public:
  Consumidor(sem_t *semaforoBufferCheio, sem_t *semaforoBufferVazio, char *recurso)
    : semaforoBufferCheio(semaforoBufferCheio), semaforoBufferVazio(semaforoBufferVazio), recurso(recurso) {
  }

  virtual ~Consumidor(){}

  void iniciar() {
    while(true) {
      randomWait();

      sem_wait(semaforoBufferVazio);
      consumir();
      sem_post(semaforoBufferCheio);
    }
  }
};

class Processo
{
private:
  int bufferSize;
  char *buffer;
  sem_t semaforoBufferCheio;
  sem_t semaforoBufferVazio;

public:
  Processo(int bufferSize) : bufferSize(bufferSize) {
    char *buffer = new char[bufferSize];

    sem_init(&semaforoBufferCheio, 0, this->bufferSize);
    sem_init(&semaforoBufferVazio, 0, 0);
  }

  virtual ~Processo() {
    delete[] buffer;
  }

  void executar() {
    Produtor produtor(&semaforoBufferCheio, &semaforoBufferVazio, buffer);
    Consumidor consumidor(&semaforoBufferCheio, &semaforoBufferVazio, buffer);

    // TODO Iniciar threads, ao invés de código sequencial.
    produtor.iniciar();
    consumidor.iniciar();
  }

};

int main()
{
  Processo processo(5);
  
  processo.executar();

  return 0;
}
