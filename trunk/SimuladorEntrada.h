
#ifndef SIMULADORENTRADA_H_
#define SIMULADORENTRADA_H_


#include "Simulador.h"

class SimuladorEntrada : public Simulador
{

private:

	double getNumeroAleatorio();
	bool modificarPosicionCompartida(int numeroPosicion);

	void incrementarCantidadDeAutosEstacionamiento();
	void incrementarMontoRecaudado(int horas);

	int cantidadAutos;



public:

	SimuladorEntrada(int numeroEntrada,int cantidadPosiciones);

	virtual ~SimuladorEntrada() {};

	void simular(Pipe pipeEntrada, Pipe pipePpal);

//	void setPipeEntrada(Pipe pipeEntrada);
//	Pipe getPipeEntrada();

	int getNumeroEntrada();


};

#endif /* SIMULADORENTRADA_H_ */
