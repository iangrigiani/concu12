
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
#include "Cola.h"
#include "Mensaje.h"

using namespace std;

class Estacionamiento {

private:

	vector< MemoriaCompartida<Posicion> > vectorMemoriaPosiciones;
	vector<int> vectorMemoriaPosicionesLibres;
	MemoriaCompartida<Administracion> administracion;

	int numeroEstacionamiento;
	int numeroPpal;

	// Pipe para la comunicacion de todas las entradas y la consola con el proceso principal. Todos escriben
	// ahi y el proceso principal solo lee
	Pipe pipePpal;

	// Pipe para responder las consultas de la consola
	Pipe pipeConsola;

	// Pipes para cada entrada y salida
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


	// Metodo bloqueante que espera que finalicen los procesos de las entradas y las salidas
	void finalizarProcesos();

	// Cierra los pipes de comunicacion con las entradas y las salidas. Tambien cierra el pipe principal.
	void finalizarPipes();

	// Metodo que gestiona la creacion de los archivos temporales y las memorias compartidas. En caso de no poder realizar alguna
	// accion vuelve atras y devuelve false.
	bool crearIniciales(int costoHora);

	// Crea los archivos temporales para la creacion de la memoria y semaforo de administracion
	void crearArchivoTemporalAdministracion();

	// Elimina los archivos temporales creados
	void eliminarArchivoTemporalAdministracion();

	// Crea la memoria compartida para la administracion
	// En caso de error devuelve false
	bool crearMemoriaCompartidaAdministracion(float costoHora);

	// Metodo que llama al simulador de la entrada y libera el pipe de comunicacion
	void correrSimuladorEntrada(int numeroEntrada,Pipe pipeEntrada,int cantidadPosiciones);

	// Metodo que llama al simulador de la salida y libera el pipe de comunicacion
	void correrSimuladorSalida(int numeroSalida, Pipe pipeSalida, int cantidadPosiciones);

	// Devuelve un numero random entre 1 y 2 (numeros de salidas)
	int getSalidaAleatoria();

	// Se encarga de llamar a los metodos para liberar memoria compartida, archivos temporales y el semaforo de la administracion
	void liberarRecursos(int cantidadLugares);

	// Libero la memoria compartida de administracion
	void liberarMemoriaCompartidaAdministracion();

	// Devuelve la cantidad de autos almacenadas en la memoria compartida de la administracion
	int obtenerCantidadActualDeAutos();

	// Devuelve el monto recaudado almacenado en la memoria compartida de la administracion
	float obtenerMontoRecaudado();



public:

	Estacionamiento(int numero, int nroPpal);

	virtual ~Estacionamiento();

	// Se encarga de crear las condiciones iniciales que permitan la ejecucion de la simulacion y luego llama a los correspondientes
	// metodos para simular las entradas y salidas. Tambien se encarga de la comunicacion entre las entradas, salidas y la consola.
	// Luego de terminada la ejecucion llama a los metodos para liberar los recursos.
	void run(int cantidadDeLugares, float costoHora, int tiempoEjecucion);

};

#endif /* ESTACIONAMIENTO_H_ */
