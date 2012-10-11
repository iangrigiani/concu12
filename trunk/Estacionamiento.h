
#ifndef ESTACIONAMIENTO_H_
#define ESTACIONAMIENTO_H_


#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <sys/wait.h>
#include <sstream>
#include "Administracion.h"
#include "Cronometro.h"
#include "SimuladorEntrada.h"
#include "SimuladorSalida.h"
#include "Utils.h"
#include "Constantes.h"
#include "Log.h"
#include "Pipe.h"

using namespace std;

class Estacionamiento {

private:

	vector< MemoriaCompartida<Posicion> > vectorMemoriaPosiciones;
	vector<int> vectorMemoriaPosicionesLibres;
	MemoriaCompartida<Administracion> administracion;

	Pipe pipeConsola;
	Pipe pipePpal;
	Pipe pipeEntrada1;
	Pipe pipeEntrada2;
	Pipe pipeEntrada3;
	Pipe pipeSalida1;
	Pipe pipeSalida2;

	void crearArchivosTemporales(int cantidadLugares);

	void eliminarArchivosTemporales(int cantidadLugares);

	void crearMemoriaCompartidaPosiciones(int cantidadLugares);

	void crearVectorPosicionesLibres(int cantidadLugares);

	void crearMemoriaCompartidaAdministracion(float costoHora);

	void correrSimuladorEntrada(int numeroEntrada,Pipe pipeEntrada,int cantidadPosiciones);

	void correrSimuladorSalida(int numeroSalida, Pipe pipeSalida, int cantidadPosiciones);

	int getPosicionAleatoria();

	int getSalidaAleatoria();

	void quitarPosicionLibre(int numeroPosicion);

	void agregarPosicionLibre(int numeroPosicion);

	int busquedaBinariaVectorLibres(int inicio,int fin,int buscado);

	void liberarMemoriaCompartida(int cantidadLugares);

	int obtenerCantidadActualDeAutos();

	float obtenerMontoRecaudado();



public:

	Estacionamiento();

	virtual ~Estacionamiento();

	void run(int cantidadDeLugares, float costoHora, int tiempoEjecucion);


};

#endif /* ESTACIONAMIENTO_H_ */
