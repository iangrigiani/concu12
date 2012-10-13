
#ifndef SIMULADORSALIDA_H_
#define SIMULADORSALIDA_H_

#include "Simulador.h"

class SimuladorSalida : public Simulador {


private:
		//Pipe pipeSalida;

		void liberarLugar(int numeroPosicion);

public:

	SimuladorSalida(int numeroSalida,int cantidadPosiciones);

	virtual ~SimuladorSalida() {};

//	void setPipeSalida(Pipe pipeSalida);
//	Pipe getPipeSalida();

	void simular(Pipe pipeEntrada, Pipe pipePpal);

	int getNumeroSalida();


};

#endif /* SIMULADORSALIDA_H_ */
