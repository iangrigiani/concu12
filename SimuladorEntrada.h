
#ifndef SIMULADORENTRADA_H_
#define SIMULADORENTRADA_H_


#include "Simulador.h"
#include "Semaforo.h"

class SimuladorEntrada : public Simulador
{

private:

	double getNumeroAleatorio();
	bool modificarPosicionCompartida(int numeroPosicion);

	// Incrementa la cantidad de autos almacenados en la memoria compartida de la administracion
	void incrementarCantidadDeAutosEstacionamiento();

	// Incrementa el monto recaudado en la memoria compartida de la administracion
	void incrementarMontoRecaudado(int horas);

	// Variable interna para llevar la cuenta de la cantidad de procesos autos creados
	int cantidadAutosCreados;

public:

	SimuladorEntrada(int numeroEstacionamiento,int numeroEntrada,int cantidadPosiciones);

	virtual ~SimuladorEntrada() {};

	// Se encarga de realizar la simulacion que crea los procesos autos durante un tiempo determinado. Gestiona la comunicacion
	// entre los procesos autos y el proceso principal. Tambien gestiona la actualizacion de los valores de monto recaudado
	// y cantidad de autos de la memoria compartida de la administracion. Al finalizar la simulacion espera a que mueran
	// todos sus procesos hijos antes de salir.
	void simular(Pipe pipeEntrada, Pipe pipePpal);

	int getNumeroEntrada();


};

#endif /* SIMULADORENTRADA_H_ */
