
#ifndef SIMULADORSALIDA_H_
#define SIMULADORSALIDA_H_

#include "Simulador.h"

class SimuladorSalida : public Simulador {


private:

		void liberarLugar(int numeroPosicion);
		void decrementarCantidadAutosEstacionamiento();

public:

	SimuladorSalida(int numeroEstacionamiento,int numeroSalida,int cantidadPosiciones);

	virtual ~SimuladorSalida() {};

	void simular(Pipe pipeEntrada, Pipe pipePpal);

	int getNumeroSalida();

	// Se encarga de liberar los semaforos de la memoria compartida de posiciones
	void liberarSemaforos();
};

#endif /* SIMULADORSALIDA_H_ */
