
#ifndef ESTACIONAMIENTO_H_
#define ESTACIONAMIENTO_H_


#include <stdio.h>
#include <list>
#include "Administracion.h"
#include "Entrada.h"
#include "Salida.h"

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

};

#endif /* ESTACIONAMIENTO_H_ */
