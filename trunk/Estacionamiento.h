
#ifndef ESTACIONAMIENTO_H_
#define ESTACIONAMIENTO_H_


#include <stdio.h>
#include "Administracion.h"

class Estacionamiento {

private:

	//TODO: ver declaracion de listas
	//List <Barrera> entradas;
	//List <Barrera> salidas;
	Administracion * administracionDelEstacionamiento;
	static Estacionamiento * estacionamiento;
	Estacionamiento();

public:

	virtual ~Estacionamiento();

	static Estacionamiento* obtenerEstacionamiento();

	Administracion * obtenerAdministracion();

};

#endif /* ESTACIONAMIENTO_H_ */
