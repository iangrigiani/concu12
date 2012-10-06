
#ifndef SIMULADOR_H_
#define SIMULADOR_H_


#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include <vector>
#include <sstream>
#include "Cronometro.h"
#include "MemoriaCompartida.h"
#include "Entrada.h"
#include "Auto.h"

class Simulador
{

private:

	Entrada * entrada;
	vector< MemoriaCompartida<Posicion> > vectorMemoriaPosiciones;
	Cronometro * cronometro;
	int cantidadAutos;

	double getNumeroAleatorio();


public:

	Simulador(int numeroEntrada);
	~Simulador();
	void simular();
	Cronometro * getCronometro();

	bool entrarAlEstacionamiento(Auto * automovil);
	bool salirDelEstacionamiento(Auto * automovil);


};

#endif /* SIMULADOR_H_ */
