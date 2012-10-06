
#ifndef ESTACIONAMIENTO_H_
#define ESTACIONAMIENTO_H_


#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <sys/wait.h>
#include <sstream>
#include "Administracion.h"
#include "Entrada.h"
#include "Salida.h"
#include "Simulador.h"
#include "Fifo.h"

using namespace std;

class Estacionamiento {

private:

	vector< MemoriaCompartida<Posicion> > vectorMemoriaPosiciones;
	vector< MemoriaCompartida<Posicion> > vectorMemoriaPosicionesLibres;
	MemoriaCompartida<Administracion> administracion;

	void crearArchivosTemporales(int cantidadLugares);
	void eliminarArchivosTemporales(int cantidadLugares);
	void crearMemoriaCompartidaPosiciones(int cantidadLugares);
	void crearMemoriaCompartidaPosicionesLibres(int cantidadLugares);
	void crearMemoriaCompartidaAdministracion(int costoHora);



public:

	Estacionamiento();

	virtual ~Estacionamiento();

	void run(int cantidadDeLugares, int costoHora, int tiempoEjecucion);

	Entrada * getEntradaAleatoria();

	Salida * getSalidaAleatoria();


};

#endif /* ESTACIONAMIENTO_H_ */
