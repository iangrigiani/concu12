
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
#include "Pipe.h"
#include "Constantes.h"
#include "Utils.h"

class Simulador
{

private:

	Entrada * entrada;
	vector< MemoriaCompartida<Posicion> > vectorMemoriaPosiciones;
	Cronometro * cronometro;
	int cantidadAutos;
	Pipe pipePpal;
	Pipe pipeEntrada;

	double getNumeroAleatorio();


public:

	Simulador(int numeroEntrada);
	~Simulador();
	void simular();

	void setPipePrincipal(Pipe pipePrincipal);
	Pipe getPipePrincipal();
	void setPipeEntrada(Pipe pipeEntrada);
	Pipe getPipeEntrada();


	Cronometro * getCronometro();

	bool entrarAlEstacionamiento(Auto * automovil);
	bool salirDelEstacionamiento(Auto * automovil);


};

#endif /* SIMULADOR_H_ */
