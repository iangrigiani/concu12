
#ifndef SIMULADORENTRADA_H_
#define SIMULADORENTRADA_H_


#include "Simulador.h"
#include "Semaforo.h"

class SimuladorEntrada : public Simulador
{

private:

	double getNumeroAleatorio();
	bool modificarPosicionCompartida(int numeroPosicion);

	void incrementarCantidadDeAutosEstacionamiento();
	void incrementarMontoRecaudado(int horas);

	int cantidadAutos;

	Semaforo semaforo;

public:

	SimuladorEntrada(int numeroEntrada,int cantidadPosiciones);

	virtual ~SimuladorEntrada() {};

	void simular(Pipe pipeEntrada, Pipe pipePpal);

	int getNumeroEntrada();


};

#endif /* SIMULADORENTRADA_H_ */