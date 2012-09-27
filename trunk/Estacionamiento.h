
#ifndef ESTACIONAMIENTO_H_
#define ESTACIONAMIENTO_H_


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Administracion.h"
#include "Entrada.h"
#include "Salida.h"
using namespace std;

class Estacionamiento {

private:

	list<Entrada*> * entradas;
	list<Salida*> * salidas;
	Administracion * administracionDelEstacionamiento;
	static Estacionamiento * estacionamiento;
	Estacionamiento();

public:

	virtual ~Estacionamiento();

	static Estacionamiento* obtenerEstacionamiento();

	void setCantidadDeLugaresDisponibles(int cantidadLugares);

	Administracion * getAdministracion();

	Entrada * getEntradaAleatoria();

	Salida * getSalidaAleatoria();


};

#endif /* ESTACIONAMIENTO_H_ */
