
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
#include "Semaforo.h"
#include <algorithm>


using namespace std;

class Estacionamiento {

private:

	vector< MemoriaCompartida<Posicion> > vectorMemoriaPosiciones;
	vector<int> vectorMemoriaPosicionesLibres;
	MemoriaCompartida<Administracion> administracion;

	// Pipe para la comunicacion de todas las entradas y la consola con el proceso principal. Todos escriben
	// ahi y el proceso principal solo lee
	Pipe pipePpal;

	// Pipe para responder las consultas de la consola
	Pipe pipeConsola;

	Pipe pipeEntrada1;
	Pipe pipeEntrada2;
	Pipe pipeEntrada3;
	Pipe pipeSalida1;
	Pipe pipeSalida2;

	//Pids
	pid_t pEntrada1;
	pid_t pEntrada2;
	pid_t pEntrada3;

	pid_t pSalida1;
	pid_t pSalida2;
	pid_t pConsola;

	// Semaforo para la administracion
	Semaforo smfAdministracion;


	void finalizarProcesos();

	void finalizarPipes();

	void crearIniciales(int cantidadDeLugares, int costoHora);

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

	int insercionBinariaVectorLibres(int inicio,int fin,int buscado);

	void liberarMemoriaCompartida(int cantidadLugares);

	int obtenerCantidadActualDeAutos();

	float obtenerMontoRecaudado();



public:

	Estacionamiento();

	virtual ~Estacionamiento();

	void run(int cantidadDeLugares, float costoHora, int tiempoEjecucion);


};

#endif /* ESTACIONAMIENTO_H_ */
