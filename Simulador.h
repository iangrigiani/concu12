
#ifndef SIMULADOR_H_
#define SIMULADOR_H_

#include "Estacionamiento.h"

class Simulador
{

private:
		Estacionamiento * estacionamiento;


public:
	Simulador();
	~Simulador();
	void simular();



};

#endif /* SIMULADOR_H_ */
