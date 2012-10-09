
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
#include "Log.h"

class Simulador
{

private:

	Entrada * entrada;
	vector< MemoriaCompartida<Posicion> > vectorMemoriaPosiciones;
	MemoriaCompartida<Administracion> administracion;
	Cronometro * cronometro;
	int cantidadAutos;
	Pipe pipePpal;
	Pipe pipeEntrada;

	double getNumeroAleatorio();
	bool modificarPosicionCompartida(int numeroPosicion);
	void inicializarMemoriaCompartidaVectorPosiciones(int cantidadPosiciones);
	void inicializarMemoriaCompartidaAdministracion();
	void incrementarCantidadDeAutos();
	void incrementarMontoRecaudado(int horas);




public:

	Simulador(int numeroEntrada,int cantidadPosiciones);
	~Simulador();
	void simular();

	void setPipePrincipal(Pipe pipePrincipal);
	Pipe getPipePrincipal();
	void setPipeEntrada(Pipe pipeEntrada);
	Pipe getPipeEntrada();

	Cronometro * getCronometro();


};

#endif /* SIMULADOR_H_ */
