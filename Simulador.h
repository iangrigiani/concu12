
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

	bool entrarAlEstacionamiento(Auto * automovil);
	bool salirDelEstacionamiento(Auto * automovil);


};

#endif /* SIMULADOR_H_ */
