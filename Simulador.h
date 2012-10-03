
#ifndef SIMULADOR_H_
#define SIMULADOR_H_


#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include "Cronometro.h"
#include "Estacionamiento.h"

class Simulador
{

private:
		Estacionamiento * estacionamiento;
		Cronometro * cronometro;
		int cantidadAutos;
		double getNumeroAleatorio();


public:
	Simulador();
	~Simulador();
	void simular();
	Cronometro * getCronometro();

	bool entrarAlEstacionamiento(Auto * automovil);
	bool salirDelEstacionamiento(Auto * automovil);


};

#endif /* SIMULADOR_H_ */
